#include "AssetImporter.h"
#include "tools/Debug.h"
#include "tools/image/ImageIf.h"
#include "tools/AssetTools.h"
#include "tools/Settings.h"
#include "tools/PathTools.h"
#include "AssetTools.h"
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QThreadPool>

#include <fstream>
#include <vector>
#include <set>
#include <map>

#include "engine/rendering/Mesh.h"
#include "engine/rendering/Material.h"
#include "engine/Scene.h"
#include "engine/primitives/Matrix4.h"
#include "engine/primitives/Quaternion.h"
#include "components/Transform.h"
#include "components/MeshRendererComponent.h"
#include "tools/Serialization.h"
#include "tools/ToolsCommon.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "ispc_texcomp.h"

#include "tootlelib.h"

#define  FREEIMAGE_LIB
#include "FreeImage.h"
#include "Utilities.h"

#include "tools/image/Image.h"

#include "Compressonator/Header/Compressonator.h"

#define MaximumImageSize 8192

using namespace std;
using namespace Assimp;
using namespace engine;
using namespace engine::image;

void freeImageOutput(FREE_IMAGE_FORMAT fif, const char *msg)
{
    qDebug() << msg;
}

AssetImporter::AssetImporter(
    const QString& contentPath,
    const QString& processedAssetsPath)
    : m_contentPath{ contentPath }
    , m_processedAssetPath{ processedAssetsPath }
{
    TootleInit();
    FreeImage_Initialise();
    FreeImage_SetOutputMessage(freeImageOutput);
    GlobalAssetNotificationService.registerListener(this, [this](const engine::AssetNotification& notification)
    {
        auto reproPath = pathExtractFolder(notification.destinationPath);
        this->processItem(
            QString::fromStdString(notification.sourcePath),
            QString::fromStdString(reproPath), 
            {}, {}, "",
            false);
    });
	auto error = CMP_InitializeBCLibrary();
}

AssetImporter::~AssetImporter()
{
    GlobalAssetNotificationService.unregisterListener(this);
	auto error = CMP_ShutdownBCLibrary();
    TootleCleanup();
}

void AssetImporter::processItem(
    const QString& sourceFilePath,
    const QString& targetPath,
    const engine::Vector3f& scale,
    const engine::Quaternionf& rotation,
    const std::string& preferredEncoding,
    bool performCopy)
{
    if (sourceFilePath == targetPath)
        return;

    AssetImporterWorker* worker = new AssetImporterWorker(
        m_workerImageMutex,
        m_workerCompressMutex,
        m_contentPath, 
        m_processedAssetPath, 
        sourceFilePath, 
        targetPath,
        scale,
        rotation,
        preferredEncoding,
        performCopy);

    QObject::connect(
        worker, SIGNAL(assetWorkStarted(QString)),
        this, SLOT(onAssetWorkStarted(QString)), Qt::QueuedConnection);
    QObject::connect(
        worker, SIGNAL(assetWorkStatusChange(QString, QString)),
        this, SLOT(onAssetWorkStatusChange(QString, QString)), Qt::QueuedConnection);
    QObject::connect(
        worker, SIGNAL(assetWorkProgress(QString, float)),
        this, SLOT(onAssetWorkProgress(QString, float)), Qt::QueuedConnection);
    QObject::connect(
        worker, SIGNAL(assetWorkStopped(QString, const AssetImporterWorker*)),
        this, SLOT(onAssetWorkStopped(QString, const AssetImporterWorker*)), Qt::QueuedConnection);

    QThreadPool::globalInstance()->start(worker);
}

void AssetImporter::createCubemap(
    const QList<QString>& sourceFilePath,
    const QList<QString>& targetFilePath)
{
    ASSERT(sourceFilePath.size() == 6, "Cubemap create got invalid number of images");

    // reorder images so as to match dx cubemap slice order
    std::vector<std::string> sourceFilePathOrdered(6);
    std::vector<std::string> targetFilePathOrdered(6);
    for (int i = 0; i < sourceFilePath.size(); ++i)
    {
        auto srcPath = sourceFilePath[i].toStdString();
        auto dstPath = targetFilePath[i].toStdString();

        bool positive = srcPath.find("pos") != std::string::npos;
        bool negative = srcPath.find("neg") != std::string::npos;
        bool x = srcPath.find("x") != std::string::npos;
        bool y = srcPath.find("y") != std::string::npos;
        bool z = srcPath.find("z") != std::string::npos;

        if ((positive && negative) || (!positive && !negative) ||
            (!x && !y && !z))
        {
            LOG_WARNING("Could not figure out cubemap side order. filename should contain either 'pos' or 'neg' and 'x', 'y', 'z' characters as part of the filename. Bailing on the ordering.");
            sourceFilePathOrdered.clear();
            targetFilePathOrdered.clear();
            for (auto&& item : sourceFilePath) { sourceFilePathOrdered.emplace_back(item.toStdString()); }
            for (auto&& item : targetFilePath) { targetFilePathOrdered.emplace_back(item.toStdString()); }
            break;
        }

        if (positive && x) { sourceFilePathOrdered[0] = srcPath; targetFilePathOrdered[0] = dstPath; }
        if (negative && x) { sourceFilePathOrdered[1] = srcPath; targetFilePathOrdered[1] = dstPath; }
        if (positive && y) { sourceFilePathOrdered[3] = srcPath; targetFilePathOrdered[3] = dstPath; } // NOTICE: switched y sides
        if (negative && y) { sourceFilePathOrdered[2] = srcPath; targetFilePathOrdered[2] = dstPath; }
        if (positive && z) { sourceFilePathOrdered[4] = srcPath; targetFilePathOrdered[4] = dstPath; }
        if (negative && z) { sourceFilePathOrdered[5] = srcPath; targetFilePathOrdered[5] = dstPath; }
    }

    std::vector<std::shared_ptr<image::ImageIf>> images;
    for (auto&& src : targetFilePathOrdered)
    {
        images.emplace_back(image::Image::createImage(src));
    }

    // test that they're all same sized and have same amount of mips
    int mips = static_cast<int>(images[0]->mipCount());
    int width = static_cast<int>(images[0]->width());
    int height = static_cast<int>(images[0]->height());
    engine::Format format = images[0]->format();
    for(auto&& image : images)
    {
        ASSERT(image->width() == width, "Cubemap images are differing by size (width)");
        ASSERT(image->height() == height, "Cubemap images are differing by size (height)");
        ASSERT(image->mipCount() == mips, "Cubemap images are differing by mip count");
        ASSERT(image->format() == format, "Cubemap images are differing by format");
    }

    // we'll use the parent folder name for the new cubemap filename
    auto filePath = pathExtractFolder(sourceFilePathOrdered[0], false);
    auto newFileName = pathExtractFilename(filePath);
    newFileName += ".dds";
    auto newFilePath = pathJoin(filePath, newFileName);

    // processed path
    auto processedFilePath = pathExtractFolder(targetFilePathOrdered[0], false);
    auto processedNewFilePath = pathJoin(processedFilePath, newFileName);

    auto newImage = image::Image::createImage(newFilePath, format, width, height, 6, mips);
    newImage->reserve();
    for (int mip = 0; mip < mips; ++mip)
    {
        auto dstSurfacePosX = newImage->map(mip, 0);
        auto dstSurfaceNegX = newImage->map(mip, 1);
        auto dstSurfacePosY = newImage->map(mip, 2);
        auto dstSurfaceNegY = newImage->map(mip, 3);
        auto dstSurfacePosZ = newImage->map(mip, 4);
        auto dstSurfaceNegZ = newImage->map(mip, 5);

        auto srcSurfacePosX = images[0]->map(mip, 0);
        auto srcSurfaceNegX = images[1]->map(mip, 0);
        auto srcSurfacePosY = images[2]->map(mip, 0);
        auto srcSurfaceNegY = images[3]->map(mip, 0);
        auto srcSurfacePosZ = images[4]->map(mip, 0);
        auto srcSurfaceNegZ = images[5]->map(mip, 0);

        memcpy(const_cast<uint8_t*>(dstSurfacePosX.data), srcSurfacePosX.data, dstSurfacePosX.sizeBytes);
        memcpy(const_cast<uint8_t*>(dstSurfaceNegX.data), srcSurfaceNegX.data, dstSurfaceNegX.sizeBytes);
        memcpy(const_cast<uint8_t*>(dstSurfacePosY.data), srcSurfacePosY.data, dstSurfacePosY.sizeBytes);
        memcpy(const_cast<uint8_t*>(dstSurfaceNegY.data), srcSurfaceNegY.data, dstSurfaceNegY.sizeBytes);
        memcpy(const_cast<uint8_t*>(dstSurfacePosZ.data), srcSurfacePosZ.data, dstSurfacePosZ.sizeBytes);
        memcpy(const_cast<uint8_t*>(dstSurfaceNegZ.data), srcSurfaceNegZ.data, dstSurfaceNegZ.sizeBytes);
    }
    newImage->save();
    QFile::copy(QString::fromStdString(newFilePath), QString::fromStdString(processedNewFilePath));
}

void AssetImporter::onAssetWorkStarted(QString sourceFilePath) const
{
    emit assetWorkStarted(sourceFilePath);
}

void AssetImporter::onAssetWorkStatusChange(QString sourceFilePath, QString msg) const
{
    emit assetWorkStatusChange(sourceFilePath, msg);
}

void AssetImporter::onAssetWorkProgress(QString sourceFilePath, float progress) const
{
    emit assetWorkProgress(sourceFilePath, progress);
}

void AssetImporter::onAssetWorkStopped(QString sourceFilePath, const AssetImporterWorker* worker) const
{
    emit assetWorkStopped(sourceFilePath);
    //QObject::disconnect(worker, SIGNAL(assetWorkStopped(QString, AssetImporterWorker*)),
    //    this, SLOT(onAssetWorkStopped(QString, AssetImporterWorker*)));
}


AssetImporterWorker::AssetImporterWorker(
    QMutex& imageMutex,
    QMutex& compressMutex,
    const QString& contentPath,
    const QString& processedAssetsPath,
    const QString& sourceFilePath,
    const QString& targetPath,
    const engine::Vector3f& scale,
    const engine::Quaternionf& rotation,
    const std::string& preferredEncoding,
    bool performCopy)
    : m_imageMutex{ imageMutex }
    , m_compressMutex{ compressMutex }
    , m_contentPath{ contentPath }
    , m_processedAssetPath{ processedAssetsPath }
    , m_sourceFilePath{ sourceFilePath }
    , m_targetPath{ targetPath }
    , m_scale{ scale }
    , m_rotation{ rotation }
    , m_preferredEncoding{ preferredEncoding }
    , m_performCopy{ performCopy }
{
}

void AssetImporterWorker::run()
{
    emit assetWorkStarted(m_sourceFilePath);
    QString targetFilePath = m_targetPath + QDir::separator() + QFileInfo(m_sourceFilePath).fileName();

    copyAsset(m_sourceFilePath, targetFilePath, m_performCopy);
}

void AssetImporterWorker::copyAsset(
    const QString& sourcePath,
    const QString& destinationPath,
    bool performCopy) const
{
    QString nativeSourceFilePath = QDir::toNativeSeparators(sourcePath);
    QString nativeDestinationFilePath = QDir::toNativeSeparators(destinationPath);

    auto asseUnderProcessed = assetFilePathUnderProcessed(m_contentPath, m_processedAssetPath, nativeDestinationFilePath);

    processAsset(
        nativeSourceFilePath, 
        asseUnderProcessed,
        nativeDestinationFilePath,
        
        nativeDestinationFilePath,  // reprocessSourcePath
        asseUnderProcessed,// reprocessDestinationPath
        performCopy); 

    if(performCopy)
        QFile::copy(nativeSourceFilePath, nativeDestinationFilePath);
}

void AssetImporterWorker::processAsset(
    const QString& sourcePath,
    const QString& destinationPath,
    const QString& contentPath,
    const QString& reprocessSourcePath,
    const QString& reprocessDestinationPath,
    bool performCopy) const
{
    if (engine::isModelFormat(sourcePath.toStdString()))
    {
        processModel(sourcePath, destinationPath, contentPath);
    }
    else if (engine::isImageFormat(sourcePath.toStdString()))
    {
        processImage(sourcePath, destinationPath, reprocessSourcePath, reprocessDestinationPath, performCopy);
    }
    emit assetWorkStopped(m_sourceFilePath, this);
}

TextureMapping mapping(aiTextureMapping map)
{
    switch (map)
    {
        case aiTextureMapping_UV: return TextureMapping::UV;
        case aiTextureMapping_SPHERE: return TextureMapping::Sphere;
        case aiTextureMapping_CYLINDER: return TextureMapping::Cylinder;
        case aiTextureMapping_BOX: return TextureMapping::Box;
        case aiTextureMapping_PLANE: return TextureMapping::Plane;
        case aiTextureMapping_OTHER: return TextureMapping::UV;
    }
    return TextureMapping::UV;
}

TextureMapMode mappingMode(aiTextureMapMode mode)
{
    switch (mode)
    {
        case aiTextureMapMode_Wrap: return TextureMapMode::Wrap;
        case aiTextureMapMode_Clamp: return TextureMapMode::Clamp;
        case aiTextureMapMode_Decal: return TextureMapMode::Decal;
        case aiTextureMapMode_Mirror: return TextureMapMode::Mirror;
    }
    return TextureMapMode::Wrap;
}

TextureOp textureOp(aiTextureOp op)
{
    switch (op)
    {
        case aiTextureOp_Multiply: return TextureOp::Multiply;
        case aiTextureOp_Add: return TextureOp::Add;
        case aiTextureOp_Subtract: return TextureOp::Subtract;
        case aiTextureOp_Divide: return TextureOp::Divide;
        case aiTextureOp_SmoothAdd: return TextureOp::SmoothAdd;
        case aiTextureOp_SignedAdd: return TextureOp::SignedAdd;
    }
    return TextureOp::Multiply;
}

TextureType textureType(aiTextureType type)
{
    switch (type)
    {
    case aiTextureType_DIFFUSE: return TextureType::Albedo;
    case aiTextureType_SPECULAR: return TextureType::Roughness;
    case aiTextureType_AMBIENT: return TextureType::Ambient;
    case aiTextureType_EMISSIVE: return TextureType::Emissive;
    case aiTextureType_HEIGHT: return TextureType::Height;
    case aiTextureType_NORMALS: return TextureType::Normal;
    case aiTextureType_SHININESS: return TextureType::Shininess;
    case aiTextureType_OPACITY: return TextureType::Opacity;
    case aiTextureType_DISPLACEMENT: return TextureType::Displacement;
    case aiTextureType_LIGHTMAP: return TextureType::Lightmap;
    case aiTextureType_REFLECTION: return TextureType::Reflection;
    default: return TextureType::Albedo;
    }
    return TextureType::Albedo;
}

bool findTexture(std::vector<engine::MaterialTexture> list, engine::TextureType type, engine::MaterialTexture& texture)
{
	for (auto&& tex : list)
	{
		if (tex.type == type)
		{
			texture = tex;
			return true;
		}
	}
	return false;
}

std::string resolveMaterialPath(const std::string& modelPath, const std::string& texturePath)
{
	if (texturePath == "")
		return "";

	auto modelRootPath = pathExtractFolder(modelPath, true);
	auto textureFilename = pathExtractFilename(texturePath);

	// check if the texture is in the same folder as the model
	if (pathExists(pathJoin(modelRootPath, textureFilename)))
	{
		return pathJoin(modelRootPath, textureFilename);
	}

	// check if the texture path is relative to the model
	if (pathExists(pathJoin(modelRootPath, texturePath)))
	{
		return pathJoin(modelRootPath, texturePath);
	}

	// check if the texture path is an absolute path
	if (pathExists(texturePath))
	{
		return texturePath;
	}

	// this is a small hack. allthough if we didn't find the texture anyway
	// it doesn't hurt to try
	if (pathExists(pathJoin(modelRootPath, pathReplaceExtension(textureFilename, "png"))))
	{
		return pathJoin(modelRootPath, pathReplaceExtension(textureFilename, "png"));
	}

	return "";
}

void createScene(
    engine::Mesh& mesh, 
    const engine::Vector3f& importScale,
    const engine::Quaternionf& importRotation,
    const QString& destinationPath, 
    aiNode* node, 
    shared_ptr<SceneNode> parent, 
    aiMatrix4x4 transform = aiMatrix4x4())
{
	std::string dstPath = destinationPath.toStdString();
	std::string nodeName = node->mName.C_Str();
	
	aiMatrix4x4 nodeTransform = node->mTransformation * transform;
	aiVector3t<float> scale;
	aiVector3t<float> position;
	aiQuaterniont<float> rotation;
	nodeTransform.Decompose(scale, rotation, position);

	shared_ptr<SceneNode> thisNode = nullptr;
	if (//(nodeName.find("$Assimp") == std::string::npos) &&
		(nodeName.find("RootNode") == std::string::npos))
	{
		thisNode = make_shared<SceneNode>();
		thisNode->name(nodeName);
		parent->addChild(thisNode);

		auto transformComponent = make_shared<Transform>();
		transformComponent->position(Vector3f{ position.x, position.y, position.z });
		transformComponent->scale(Vector3f{ scale.x, scale.y, scale.z } * importScale);
		transformComponent->rotation(Quaternionf{ rotation.x, rotation.y, rotation.z, rotation.w } * importRotation);
		thisNode->addComponent(transformComponent);
	}
	else
		thisNode = parent;
    
    for (unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        unsigned int meshId = node->mMeshes[i];
        shared_ptr<SceneNode> meshNode = make_shared<SceneNode>();
        meshNode->name(node->mName.C_Str());
        thisNode->addChild(meshNode);
        
        auto transformComponent = make_shared<Transform>();
        meshNode->addComponent(transformComponent);

        auto meshRenderer = make_shared<MeshRendererComponent>(dstPath, meshId);
        meshNode->addComponent(meshRenderer);

		engine::Material& mat = mesh.subMeshes()[meshId].material;
		
		engine::MaterialTexture albedo; 
		bool hasAlbedo = findTexture(mat.textures, TextureType::Albedo, albedo);

		engine::MaterialTexture roughness;
		bool hasRoughness = findTexture(mat.textures, TextureType::Roughness, roughness);

		engine::MaterialTexture normal;
		bool hasNormal = findTexture(mat.textures, TextureType::Normal, normal);
		
		engine::MaterialTexture metalness;
		bool hasMetalness = findTexture(mat.textures, TextureType::Metalness, metalness);

        engine::MaterialTexture occlusion;
        bool hasOcclusion = findTexture(mat.textures, TextureType::Occlusion, occlusion);

		if(hasAlbedo || hasRoughness || hasNormal || hasMetalness || hasOcclusion)
		{
			auto albedoPath = hasAlbedo ? resolveMaterialPath(dstPath, albedo.filePath) : "";
			auto roughnessPath = hasRoughness ? resolveMaterialPath(dstPath, roughness.filePath) : "";
			auto normalPath = hasNormal ? resolveMaterialPath(dstPath, normal.filePath) : "";
			auto metalnessPath = hasMetalness ? resolveMaterialPath(dstPath, metalness.filePath) : "";
            auto occlusionPath = hasOcclusion ? resolveMaterialPath(dstPath, occlusion.filePath) : "";

			if (albedoPath != "" ||
                roughnessPath != "" ||
				normalPath != "" ||
				metalnessPath != "" ||
                occlusionPath != "")
			{
				auto material = make_shared<MaterialComponent>(
					albedoPath,
                    roughnessPath,
					normalPath,
					metalnessPath,
                    occlusionPath
					);
				meshNode->addComponent(material);
			}
		}
    }

    for (unsigned int i = 0; i < node->mNumChildren; ++i)
    {
        createScene(mesh, importScale, importRotation, destinationPath, node->mChildren[i], thisNode, nodeTransform);
    }
}

void AssetImporterWorker::processModel(
    const QString& sourcePath,
    const QString& destinationPath,
    const QString& contentPath) const
{
    QDir targetpath(QFileInfo(destinationPath).absolutePath());
    if (!targetpath.exists())
    {
        targetpath.mkpath(".");
    }

	std::string stdSourcePath = sourcePath.toStdString();
    std::string contentSourcePath = contentPath.toStdString();

    Importer importer;
    auto scene = importer.ReadFile(
        stdSourcePath,
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType |
        aiProcess_GenSmoothNormals);

    if (!scene)
        return;

    if (scene->HasMeshes())
    {
        engine::Mesh mesh;
        mesh.setFilename(destinationPath.toStdString());

        for (size_t i = 0; i < scene->mNumMeshes; ++i)
        {
            auto assmesh = scene->mMeshes[i];
            engine::SubMesh subMesh;

            // vertices
            if (assmesh->HasFaces())
            {
                subMesh.position.resize(assmesh->mNumVertices);
                for (int i = 0; i < static_cast<int>(assmesh->mNumVertices); ++i)
                {
                    subMesh.position[i][0] = assmesh->mVertices[i].x;
                    subMesh.position[i][1] = assmesh->mVertices[i].y;
                    subMesh.position[i][2] = assmesh->mVertices[i].z;
                }
            }

            // normals
            if (assmesh->HasNormals())
            {
                subMesh.normal.resize(assmesh->mNumVertices);
                for (int i = 0; i < static_cast<int>(assmesh->mNumVertices); ++i)
                {
                    subMesh.normal[i][0] = assmesh->mNormals[i].x;
                    subMesh.normal[i][1] = assmesh->mNormals[i].y;
                    subMesh.normal[i][2] = assmesh->mNormals[i].z;
                }
            }

            // tangents and bitangents
            if (assmesh->HasTangentsAndBitangents())
            {
                subMesh.tangent.resize(assmesh->mNumVertices);
                for (int i = 0; i < static_cast<int>(assmesh->mNumVertices); ++i)
                {
                    subMesh.tangent[i][0] = assmesh->mTangents[i].x;
                    subMesh.tangent[i][1] = assmesh->mTangents[i].y;
                    subMesh.tangent[i][2] = assmesh->mTangents[i].z;
                }
            }
            else
            {
                // this is really unfortunate. so we'll just make shit up.
                subMesh.tangent.resize(assmesh->mNumVertices);
                for (int i = 0; i < static_cast<int>(assmesh->mNumVertices); ++i)
                {
                    Vector3f normal = Vector3f(assmesh->mNormals[i].x, assmesh->mNormals[i].y, assmesh->mNormals[i].z);
                    Vector3f tangent = normal.cross(Vector3f(0.0f, 0.0f, 1.0f)).normalize();

                    subMesh.tangent[i][0] = tangent.x;
                    subMesh.tangent[i][1] = tangent.y;
                    subMesh.tangent[i][2] = tangent.z;
                }
            }

            // uv
            if (assmesh->HasTextureCoords(0))
            {
                subMesh.uv.resize(assmesh->mNumVertices);
                for (int i = 0; i < static_cast<int>(assmesh->mNumVertices); ++i)
                {
                    // TODO: support for UV channels
                    subMesh.uv[i][0] = assmesh->mTextureCoords[0][i].x;
                    subMesh.uv[i][1] = assmesh->mTextureCoords[0][i].y;
                }
            }
            else
            {
                subMesh.uv.resize(assmesh->mNumVertices);
            }

            // colors
            if (assmesh->HasVertexColors(0))
            {
                subMesh.colors.resize(assmesh->mNumVertices);
                for (int i = 0; i < static_cast<int>(assmesh->mNumVertices); ++i)
                {
                    subMesh.colors[i][0] = assmesh->mColors[0][i].r;
                    subMesh.colors[i][1] = assmesh->mColors[0][i].g;
                    subMesh.colors[i][2] = assmesh->mColors[0][i].b;
                    subMesh.colors[i][3] = assmesh->mColors[0][i].a;
                }
            }
            else
            {
                subMesh.colors.resize(assmesh->mNumVertices);
                for (int i = 0; i < static_cast<int>(assmesh->mNumVertices); ++i)
                {
                    subMesh.colors[i][0] = 0.0f;
                    subMesh.colors[i][1] = 0.0f;
                    subMesh.colors[i][2] = 0.0f;
                    subMesh.colors[i][3] = 0.0f;
                }
            }

            // indices
			unsigned int gindex = 0;
            for (int i = 0; i < static_cast<int>(assmesh->mNumFaces); ++i)
            {
				auto face = &assmesh->mFaces[i];
				for (int a = 0; a < static_cast<int>(face->mNumIndices); a++)
				{
					subMesh.indices.emplace_back(face->mIndices[a]);
					//LOG("face: %i, localindex: %i, globalindex %i, val: %u", i, a, gindex, face->mIndices[a]);
					++gindex;
				}

                /*auto face = &assmesh->mFaces[i];
                if (face->mNumIndices == 3)
                {
                    for (int a = 0; a < static_cast<int>(face->mNumIndices); a++)
                    {
                        subMesh.indices.emplace_back(face->mIndices[a]);
                    }
                }*/
            }

            // perform mesh optimization and clusterizing
#if 1
            m_imageMutex.lock();
            unsigned int faceCount = static_cast<unsigned int>(subMesh.indices.size() / 3);
            std::vector<unsigned int> faceClusters(faceCount + 1);
            std::vector<unsigned int> faceRemap(faceCount);
            auto clusterizerResult = TootleClusterMesh(
                subMesh.position.data(),
                subMesh.indices.data(),
                static_cast<unsigned int>(subMesh.position.size()),
                faceCount,
                3 * sizeof(float),
                0,
                &subMesh.indices[0],
                &faceClusters[0],
                nullptr //&faceRemap[0]
            );
            ASSERT(clusterizerResult == TOOTLE_OK, "AMD Tootle failed to clusterize mesh!");

            auto vertexCacheOptimizeResult = TootleVCacheClusters(
                subMesh.indices.data(),
                faceCount,
                static_cast<unsigned int>(subMesh.position.size()),
                TOOTLE_DEFAULT_VCACHE_SIZE,
                faceClusters.data(),
                &subMesh.indices[0],
                nullptr, //&faceRemap[0],
                TOOTLE_VCACHE_AUTO
            );
            ASSERT(vertexCacheOptimizeResult == TOOTLE_OK, "AMD Tootle could not optimize vertex cache");

            std::vector<unsigned int> clusterRemap(faceClusters.back());
            auto optimizeOverdrawResult = TootleOptimizeOverdraw(
                subMesh.position.data(),
                subMesh.indices.data(),
                static_cast<unsigned int>(subMesh.position.size()),
                faceCount,
                3 * sizeof(float),
                nullptr,
                0,
                TootleFaceWinding::TOOTLE_CCW,
                faceClusters.data(),
                &subMesh.indices[0],
                &clusterRemap[0],
                TOOTLE_OVERDRAW_AUTO
            );
            ASSERT(optimizeOverdrawResult == TOOTLE_OK, "AMD Tootle could not optimize for overdraw");

            // gather clusters

            //       face id     , face count
            std::map<unsigned int, unsigned int> clusters;
            int clusterId = -1;
            int lastIndex = -1;
            int clusterFaces = 0;
            for (unsigned int index = 0; index < faceClusters.size()-1; ++index)
            {
                if (clusterId == -1)
                {
                    clusterId = faceClusters[index];
                    lastIndex = index;
                }
                else
                {
                    if (clusterId != faceClusters[index])
                    {
                        clusters[clusterId] = index - lastIndex;
                        clusterId = faceClusters[index];
                        lastIndex = index;
                    }
                }

                if (index == faceClusters.size() - 2)
                {
                    // last cluster
                    clusters[clusterId] = index - lastIndex + 1;
                }
            }
            m_imageMutex.unlock();

            subMesh.clusterId.resize(clusters.size());
            subMesh.clusterIndexCount.resize(clusters.size());
            subMesh.clusterBounds.resize(clusters.size());

            SubMesh::BoundingBox meshBoundingBox;
            meshBoundingBox.min = { std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
            meshBoundingBox.max = { std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min() };
            int index = 0;
            int vertexIndex = 0;
            for (auto&& c : clusters)
            {
                subMesh.clusterId[index] = c.first;
                subMesh.clusterIndexCount[index] = c.second * 3;

                SubMesh::BoundingBox bb;
                bb.min = { std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
                bb.max = { std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min() };
                for (unsigned int i = 0; i < c.second; ++i)
                {
                    for (int a = 0; a < 3; ++a)
                    {
                        Vector3f pos = subMesh.position[subMesh.indices[vertexIndex]];
                        if (pos.x < bb.min.x) bb.min.x = pos.x;
                        if (pos.y < bb.min.y) bb.min.y = pos.y;
                        if (pos.z < bb.min.z) bb.min.z = pos.z;
                        if (pos.x > bb.max.x) bb.max.x = pos.x;
                        if (pos.y > bb.max.y) bb.max.y = pos.y;
                        if (pos.z > bb.max.z) bb.max.z = pos.z;

                        if (pos.x < meshBoundingBox.min.x) meshBoundingBox.min.x = pos.x;
                        if (pos.y < meshBoundingBox.min.y) meshBoundingBox.min.y = pos.y;
                        if (pos.z < meshBoundingBox.min.z) meshBoundingBox.min.z = pos.z;
                        if (pos.x > meshBoundingBox.max.x) meshBoundingBox.max.x = pos.x;
                        if (pos.y > meshBoundingBox.max.y) meshBoundingBox.max.y = pos.y;
                        if (pos.z > meshBoundingBox.max.z) meshBoundingBox.max.z = pos.z;
                        ++vertexIndex;
                    }
                }
                subMesh.clusterBounds[index] = bb;
                ++index;
            }
            subMesh.boundingBox = meshBoundingBox;

            LOG("clustered");

#else
            unsigned int clusters = 0;
            auto tootleRes = TootleOptimize(
                subMesh.position.data(),
                subMesh.indices.data(),
                static_cast<unsigned int>(subMesh.position.size()),
                static_cast<unsigned int>(subMesh.indices.size() / 3),
                3 * sizeof(float),
                TOOTLE_DEFAULT_VCACHE_SIZE,
                nullptr,
                0,
                TootleFaceWinding::TOOTLE_CCW,
                &subMesh.indices[0],
                &clusters,
                TootleVCacheOptimizer::TOOTLE_VCACHE_TIPSY);
            ASSERT(tootleRes == TOOTLE_OK, "AMD Tootle encountered an error");
#endif

            auto material = scene->mMaterials[assmesh->mMaterialIndex];

            std::vector<aiTextureType> textureTypes = {
                aiTextureType_DIFFUSE,
                aiTextureType_SPECULAR,
                aiTextureType_AMBIENT,
                aiTextureType_EMISSIVE,
                aiTextureType_HEIGHT,
                aiTextureType_NORMALS,
                aiTextureType_SHININESS,
                aiTextureType_OPACITY,
                aiTextureType_DISPLACEMENT,
                aiTextureType_LIGHTMAP,
                aiTextureType_REFLECTION
            };

            bool hadMaterial = false;
            for (auto texType : textureTypes)
            {
                for (unsigned int texIndex = 0; texIndex < material->GetTextureCount(texType); ++texIndex)
                {
                    aiString path;
                    aiTextureMapping textureMapping;
                    unsigned int uvIndex;
                    float blend;
                    aiTextureOp op;
                    aiTextureMapMode mode;
                    if (material->GetTexture(texType, texIndex, &path, &textureMapping,
                        &uvIndex, &blend, &op, &mode) == aiReturn_SUCCESS)
                    {
                        subMesh.material.textures.emplace_back(MaterialTexture{
                            std::string(&path.data[0], path.length),
                            mapping(textureMapping),
                            mappingMode(mode),
                            textureType(texType),
                            textureOp(op)
                        });
                        hadMaterial = true;
                        qDebug() << "Mesh has material: " << std::string(&path.data[0], path.length).data();
                    }
                }
            }
            if (!hadMaterial)
            {
                qDebug() << "No material for mesh found.";
            }

			/*subMesh.material.textures.emplace_back(MaterialTexture{
				"C:\\work\\testdata\\test3.dds",
				engine::TextureMapping::UV,
				engine::TextureMapMode::Clamp,
				engine::TextureType::Diffuse,
				engine::TextureOp::Add
			});*/
            

            mesh.subMeshes().emplace_back(subMesh);
        }
        mesh.save();

		if (scene->mRootNode)
		{
			shared_ptr<SceneNode> node = make_shared<SceneNode>();

			auto filename = pathExtractFilename(stdSourcePath);
			auto ext = pathExtractExtension(filename);
			auto plainFilename = filename.substr(0, filename.length() - ext.length() - 1);
			node->name(plainFilename);

			shared_ptr<Transform> rootTransform = make_shared<Transform>();
			node->addComponent(rootTransform);

			Matrix4f transform;
			createScene(mesh, m_scale, m_rotation, destinationPath, scene->mRootNode, node);

			rapidjson::StringBuffer strBuffer;
			rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(strBuffer);
			node->serialize(writer);

			ofstream outFile;
			outFile.open(pathReplaceExtension(contentSourcePath, "prefab"));
			outFile.write(strBuffer.GetString(), strBuffer.GetSize());
			outFile.close();
		}

    }

}

void processFreeImageData(uint8_t* data, uint32_t width, uint32_t height)
{
	uint8_t tr = 0;
	uint8_t tg = 0;
	uint8_t tb = 0;
	uint8_t ta = 0;

	uint8_t* ptr = data;
	uint8_t* lastPtr = ptr + (width * (height-1) * 4);
	auto half = height / 2;
	for (int y = 0; y < static_cast<int>(half); ++y)
	{
		uint8_t* lastStart = lastPtr;
		for (int x = 0; x <static_cast<int>(width); ++x)
		{
			uint8_t* col = ptr;
			uint8_t b = *col; ++col;
			uint8_t g = *col; ++col;
			uint8_t r = *col; ++col;
			uint8_t a = *col; ++col;
			
			uint8_t* last = lastStart;
			uint8_t lb = *last; ++last;
			uint8_t lg = *last; ++last;
			uint8_t lr = *last; ++last;
			uint8_t la = *last; ++last;

			tr = r;
			tg = g;
			tb = b;
			ta = a;

			b = lr;
			g = lg;
			r = lb;
			a = la;

			lb = tr;
			lg = tg;
			lr = tb;
			la = ta;

			*ptr = b; ++ptr;
			*ptr = g; ++ptr;
			*ptr = r; ++ptr;
			*ptr = a; ++ptr;

			*lastStart = lb; ++lastStart;
			*lastStart = lg; ++lastStart;
			*lastStart = lr; ++lastStart;
			*lastStart = la; ++lastStart;
		}
		lastPtr -= width * 4;
	}
}

std::vector<TextureType> possibleTextureTypes(FREE_IMAGE_TYPE type, unsigned int bpp)
{
	switch (type)
	{
		case FIT_UNKNOWN: ASSERT(false, "Unknown image type!");
		case FIT_BITMAP: 
        {
            if (bpp == 32 || bpp == 24)
                return { TextureType::Albedo };
            else if (bpp == 8)
                return { TextureType::Ambient, TextureType::Height };
        }
		case FIT_UINT16: return { TextureType::Shininess, TextureType::Ambient, TextureType::Displacement, TextureType::Emissive, TextureType::Height, TextureType::Lightmap, TextureType::Opacity, TextureType::Reflection, TextureType::Roughness };
		case FIT_INT16: return { TextureType::Shininess, TextureType::Ambient, TextureType::Displacement, TextureType::Emissive, TextureType::Height, TextureType::Lightmap, TextureType::Opacity, TextureType::Reflection, TextureType::Roughness };
		case FIT_UINT32: return { TextureType::Shininess, TextureType::Ambient, TextureType::Displacement, TextureType::Emissive, TextureType::Height, TextureType::Lightmap, TextureType::Opacity, TextureType::Reflection, TextureType::Roughness };
		case FIT_INT32: return { TextureType::Shininess, TextureType::Ambient, TextureType::Displacement, TextureType::Emissive, TextureType::Height, TextureType::Lightmap, TextureType::Opacity, TextureType::Reflection, TextureType::Roughness };
		case FIT_FLOAT: return { TextureType::Shininess, TextureType::Ambient, TextureType::Displacement, TextureType::Emissive, TextureType::Height, TextureType::Lightmap, TextureType::Opacity, TextureType::Reflection, TextureType::Roughness };
		case FIT_DOUBLE: return { TextureType::Shininess, TextureType::Ambient, TextureType::Displacement, TextureType::Emissive, TextureType::Height, TextureType::Lightmap, TextureType::Opacity, TextureType::Reflection, TextureType::Roughness };
		case FIT_COMPLEX: return { TextureType::Shininess, TextureType::Ambient, TextureType::Displacement, TextureType::Emissive, TextureType::Height, TextureType::Lightmap, TextureType::Opacity, TextureType::Reflection, TextureType::Roughness };
		case FIT_RGB16: return { TextureType::Albedo, TextureType::Normal, TextureType::Hdr };
		case FIT_RGBA16: return { TextureType::Albedo, TextureType::Normal, TextureType::Hdr };
		case FIT_RGBF: return { TextureType::Hdr, TextureType::Albedo, TextureType::Normal };
		case FIT_RGBAF: return { TextureType::Hdr, TextureType::Albedo, TextureType::Normal };

	}
	return { TextureType::Albedo };
}

CMP_FORMAT compressonatorFormat(FREE_IMAGE_TYPE type, unsigned int bytesPerPixel)
{
	switch (type)
	{
	case FIT_UNKNOWN: ASSERT(false, "Unknown image type!");
	case FIT_BITMAP:
    {
        if (bytesPerPixel == 24)
            return CMP_FORMAT_BGR_888;
        else if (bytesPerPixel == 32)
            return CMP_FORMAT_BGRA_8888;
        else if (bytesPerPixel == 8)
            return CMP_FORMAT_R_8;
        ASSERT(false, "Unsupported bytesPerPixel: %u", bytesPerPixel);
    }
	case FIT_UINT16: return CMP_FORMAT_R_16;
	case FIT_INT16: return CMP_FORMAT_R_16;
	case FIT_UINT32: return CMP_FORMAT_Unknown;
	case FIT_INT32: return CMP_FORMAT_Unknown;
	case FIT_FLOAT: return CMP_FORMAT_R_32F;
	case FIT_DOUBLE: return CMP_FORMAT_Unknown;
	case FIT_COMPLEX: return CMP_FORMAT_Unknown;
	case FIT_RGB16: return CMP_FORMAT_Unknown;
	case FIT_RGBA16: return CMP_FORMAT_RGBA_16;
	case FIT_RGBF: return CMP_FORMAT_RGB_32F;
	case FIT_RGBAF: return CMP_FORMAT_RGBA_32F;

	}
	return CMP_FORMAT_BGRA_16;
}

struct ImporterSurfaceWork
{
    const char* message;
    size_t allBytes;
    size_t bytesSoFar;
    size_t thisBlockBytes;
};

bool feedback(float fProgress, DWORD_PTR pUser1, DWORD_PTR pUser2)
{
	//LOG("Compression progress: %f", fProgress);
    auto worker = reinterpret_cast<AssetImporterWorker*>(pUser1);
    auto surfaceWork = reinterpret_cast<ImporterSurfaceWork*>(pUser2);

    auto progressBytes = static_cast<float>(surfaceWork->thisBlockBytes) * (fProgress / 100.0f);
    auto progress = (static_cast<float>(surfaceWork->bytesSoFar) + progressBytes) / static_cast<float>(surfaceWork->allBytes);

    worker->reportProgress(progress, surfaceWork->message);
	return false;
}

void AssetImporterWorker::reportProgress(float progress, const char* filename)
{
    std::string fi = filename;
    QString source = QString::fromStdString(fi);
    emit assetWorkProgress(source, progress);
}

CMP_FORMAT compressonatorBlockFormat(engine::Format format)
{
    switch (format)
    {
    case Format::Format_BC1_UNORM: return CMP_FORMAT_BC1;
    case Format::Format_BC3_UNORM: return CMP_FORMAT_BC3;
    case Format::Format_BC4_UNORM: return CMP_FORMAT_BC4;
    case Format::Format_BC5_UNORM: return CMP_FORMAT_BC5;
    case Format::Format_BC6H_UF16: return CMP_FORMAT_BC6H;
    case Format::Format_BC7_UNORM: return CMP_FORMAT_BC7;
    default: return CMP_FORMAT_BC1;
    }
}

void AssetImporterWorker::processImage(
    const QString& sourcePath,
    const QString& destinationPath,
    const QString& reprocessSourcePath,
    const QString& reprocessDestinationPath,
    bool performCopy,
	engine::Format encodeType) const
{
    auto srcPath = sourcePath.toStdString();
    auto dstPath = destinationPath.toStdString();

    

    emit assetWorkProgress(m_sourceFilePath, 0.0f);
    emit assetWorkStatusChange(m_sourceFilePath, "Decoding image");

    m_imageMutex.lock();
    FREE_IMAGE_FORMAT imageFormat = FreeImage_GetFileType(srcPath.data());
    FIBITMAP* bitmap = FreeImage_Load(imageFormat, srcPath.data(), 0);

	// extract everything
    unsigned int width = FreeImage_GetWidth(bitmap);
    unsigned int height = FreeImage_GetHeight(bitmap);

	auto imageType = FreeImage_GetImageType(bitmap);
    unsigned int bpp = FreeImage_GetBPP(bitmap);
	auto possibleTargetFormats = possibleEncodingFormatRGB(possibleTextureTypes(imageType, bpp));
	
	unsigned int redMask = FreeImage_GetRedMask(bitmap);
	unsigned int greenMask = FreeImage_GetGreenMask(bitmap);
	unsigned int blueMask = FreeImage_GetBlueMask(bitmap);
	bool transparent = FreeImage_IsTransparent(bitmap);
	
	tools::Settings settings(pathReplaceExtension(dstPath, "json"));
	auto settings_width = settings.get<int>("width", static_cast<int>(width));
	auto settings_height = settings.get<int>("height", static_cast<int>(height));
	auto settings_size = settings.get<int>("size", static_cast<int>(width));
	auto settings_textureType = settings.get<std::string>("textureType", textureTypeToString(possibleTextureTypeFromFormat(possibleTargetFormats[0])));
	auto settings_textureEncoding = settings.get<std::string>("textureEncoding", formatToString(possibleTargetFormats[0]));
    auto settings_generateMips = settings.get<bool>("generateMips", true);
    if (performCopy)
    {
        settings.set<std::string>("sourcePath", reprocessSourcePath.toStdString());
        settings.set<std::string>("destinationPath", reprocessDestinationPath.toStdString());
    }

	encodeType = formatFromString(settings_textureEncoding);

    auto ext = pathExtractExtension(sourcePath.toStdString());
    if (ext == "dds")
    {
        // just copy over
        QString nativeSourceFilePath = QDir::toNativeSeparators(sourcePath);
        QString nativeDestinationFilePath = QDir::toNativeSeparators(destinationPath);
        if (performCopy)
            QFile::copy(nativeSourceFilePath, nativeDestinationFilePath);
        m_imageMutex.unlock();
        return;
    }

    /*FIBITMAP* scaledImage = bitmap;
    if ((width != height) ||
        (width % 4 != 0) ||
        (width > MaximumImageSize) || (height > MaximumImageSize))
    {
        if (width > MaximumImageSize)
            width = MaximumImageSize;
        if (height > MaximumImageSize)
            height = MaximumImageSize;

        unsigned int biggerValue = height > width ? height : width;

        int potSize = 1;
        while (static_cast<unsigned int>(potSize) < biggerValue) potSize *= 2;

        scaledImage = FreeImage_Rescale(bitmap, potSize, potSize, FREE_IMAGE_FILTER::FILTER_BILINEAR);
        width = potSize;
        height = potSize;
    }
    ASSERT(width == height);
    ASSERT(width % 4 == 0);

    FIBITMAP* srcSurface = scaledImage;
    if (!FreeImage_FlipVertical(srcSurface))
        LOG("Could not vertically invert image");*/

    if (width > MaximumImageSize)
        width = MaximumImageSize;
    if (width % 4 != 0 || !isPowerOfTwo(width))
    {
        int potSize = 1;
        while (static_cast<unsigned int>(potSize) < width) potSize *= 2;
        width = potSize;
    }

    if (height > MaximumImageSize)
        height = MaximumImageSize;
    if (height % 4 != 0 || !isPowerOfTwo(height))
    {
        int potSize = 1;
        while (static_cast<unsigned int>(potSize) < height) potSize *= 2;
        height = potSize;
    }


    m_imageMutex.unlock();

    m_compressMutex.lock();
    
    auto imageBytes = 0;
    auto w = width;
    auto h = height;
    while (w > 1 && h > 1)
    {
        imageBytes += static_cast<int>(engine::formatBytes(encodeType, w, h));
        w /= 2;
        h /= 2;
        if (!settings_generateMips)
            break;
    }
    vector<uint8_t> dstBuffer(imageBytes);

    if (encodeType == engine::Format::Format_BC1_UNORM) { emit assetWorkStatusChange(m_sourceFilePath, "Encoding image to BC1"); }
    else if (encodeType == engine::Format::Format_BC3_UNORM) { emit assetWorkStatusChange(m_sourceFilePath, "Encoding image to BC3"); }
    else if (encodeType == engine::Format::Format_BC4_UNORM) { emit assetWorkStatusChange(m_sourceFilePath, "Encoding image to BC4"); }
    else if (encodeType == engine::Format::Format_BC5_UNORM) { emit assetWorkStatusChange(m_sourceFilePath, "Encoding image to BC5"); }
    else if (encodeType == engine::Format::Format_BC6H_UF16) { emit assetWorkStatusChange(m_sourceFilePath, "Encoding image to BC6H"); }
    else if (encodeType == engine::Format::Format_BC7_UNORM) { emit assetWorkStatusChange(m_sourceFilePath, "Encoding image to BC7"); }

    size_t currentDstBytes = 0;
    int mipCount = 1;
    int workWidth = width;
    int workHeight = height;
    while (workWidth > 1 && workHeight > 1)
    {
        m_compressMutex.unlock();
        m_imageMutex.lock();
        auto mipScaled = FreeImage_Rescale(bitmap, workWidth, workHeight, FREE_IMAGE_FILTER::FILTER_LANCZOS3);
        rgba_surface surface;
        surface.width = workWidth;
        surface.height = workHeight;
        surface.stride = FreeImage_GetPitch(mipScaled);
        surface.ptr = reinterpret_cast<uint8_t*>(FreeImage_GetBits(mipScaled));
        auto dibSize = FreeImage_GetDIBSize(mipScaled);
        m_imageMutex.unlock();
        m_compressMutex.lock();

        auto surfaceBytes = engine::formatBytes(encodeType, workWidth, workHeight);

        CMP_Texture dstTexture;
        dstTexture.dwSize = sizeof(CMP_Texture);
        dstTexture.dwWidth = workWidth;
        dstTexture.dwHeight = workHeight;
        dstTexture.dwPitch = surface.stride;
        dstTexture.format = compressonatorBlockFormat(encodeType);
        dstTexture.nBlockHeight = 4;
        dstTexture.nBlockWidth = 4;
        dstTexture.nBlockDepth = 1;
        dstTexture.dwDataSize = static_cast<CMP_DWORD>(surfaceBytes);
        dstTexture.pData = dstBuffer.data() + currentDstBytes;

        CMP_Texture srcTexture;
        srcTexture.dwSize = sizeof(CMP_Texture);
        srcTexture.dwWidth = workWidth;
        srcTexture.dwHeight = workHeight;
        srcTexture.dwPitch = surface.stride;
        srcTexture.format = compressonatorFormat(imageType, bpp);
        srcTexture.dwDataSize = dibSize;
        srcTexture.pData = surface.ptr;

        CMP_CompressOptions options = {};
        options.dwSize = sizeof(CMP_CompressOptions);

        auto fileStd = m_sourceFilePath.toStdString();
        const char* file = fileStd.data();

        ImporterSurfaceWork work;
        work.message = file;
        work.allBytes = dstBuffer.size();
        work.bytesSoFar = currentDstBytes;
        work.thisBlockBytes = surfaceBytes;

        CMP_ConvertTexture(&srcTexture, &dstTexture, &options, feedback, reinterpret_cast<DWORD_PTR>(this), reinterpret_cast<DWORD_PTR>(&work));

        m_compressMutex.unlock();
        m_imageMutex.lock();
        FreeImage_Unload(mipScaled);
        m_imageMutex.unlock();
        m_compressMutex.lock();

        if (!settings_generateMips)
            break;

        workWidth /= 2;
        workHeight /= 2;

        currentDstBytes += surfaceBytes;
        if (workWidth > 2 && workHeight > 2)
            ++mipCount;
    }

    emit assetWorkStatusChange(m_sourceFilePath, "Saving processed image");

    QDir targetpath(QFileInfo(destinationPath).absolutePath());
    if (!targetpath.exists())
    {
        targetpath.mkpath(".");
    }

    {
        auto image = engine::image::Image::createImage(destinationPath.toStdString(), encodeType, width, height, 1, mipCount);
        image->save(reinterpret_cast<const char*>(dstBuffer.data()), dstBuffer.size());
    }

    m_compressMutex.unlock();

    m_imageMutex.lock();
    FreeImage_Unload(bitmap);
    m_imageMutex.unlock();
}
