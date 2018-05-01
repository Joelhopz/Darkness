#include "ModelTask.h"
#include "platform/Platform.h"
#include "protocols/network/ResourceProtocols.pb.h"
#include "engine/rendering/Material.h"
#include "tools/PathTools.h"
#include "tools/MeshTools.h"
#include "tools/Clusterize.h"
#include "engine/primitives/Quaternion.h"
#include "engine/primitives/Vector3.h"
#include "engine/rendering/SubMesh.h"
#include "engine/rendering/Mesh.h"
#include "engine/Scene.h"
#include "engine/network/MqMessage.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "tootlelib.h"

#include <memory>
#include <string>

using namespace engine;
using namespace std;
using namespace Assimp;

namespace resource_task
{
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
        const string& destinationPath,
        aiNode* node,
        shared_ptr<SceneNode> parent,
        aiMatrix4x4 transform = aiMatrix4x4())
    {
        std::string dstPath = destinationPath;
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
            transformComponent->scale(Vector3f{ scale.x, scale.y, scale.z } *importScale);
            transformComponent->rotation(Quaternionf{ rotation.x, rotation.y, rotation.z, rotation.w } *importRotation);
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

            engine::Material& mat = mesh.subMeshes()[meshId].out_material;

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

            auto albedoPath = hasAlbedo ? resolveMaterialPath(dstPath, albedo.filePath) : "";
            auto roughnessPath = hasRoughness ? resolveMaterialPath(dstPath, roughness.filePath) : "";
            auto normalPath = hasNormal ? resolveMaterialPath(dstPath, normal.filePath) : "";
            auto metalnessPath = hasMetalness ? resolveMaterialPath(dstPath, metalness.filePath) : "";
            auto occlusionPath = hasOcclusion ? resolveMaterialPath(dstPath, occlusion.filePath) : "";

            auto material = make_shared<MaterialComponent>(
                hasAlbedo ? resolveMaterialPath(dstPath, albedo.filePath) : "",
                hasRoughness ? resolveMaterialPath(dstPath, roughness.filePath) : "",
                hasNormal ? resolveMaterialPath(dstPath, normal.filePath) : "",
                hasMetalness ? resolveMaterialPath(dstPath, metalness.filePath) : "",
                hasOcclusion ? resolveMaterialPath(dstPath, occlusion.filePath) : "",
                hasAlbedo ? albedo.uvIndex : 0,
                hasRoughness ? roughness.uvIndex : 0,
                hasNormal ? normal.uvIndex : 0,
                hasMetalness ? metalness.uvIndex : 0,
                hasOcclusion ? occlusion.uvIndex : 0
                );
            meshNode->addComponent(material);
        }

        for (unsigned int i = 0; i < node->mNumChildren; ++i)
        {
            createScene(mesh, importScale, importRotation, destinationPath, node->mChildren[i], thisNode, nodeTransform);
        }
    }

    void updateProgress(
        const std::string& hostId,
        const std::string& taskId,
        zmq::socket_t* socket,
        float progress,
        const std::string& message)
    {
        ProcessorTaskMessageType type;
        type.set_type(ProcessorTaskMessageType::TaskProgressMessage);
        vector<char> type_message(type.ByteSizeLong());
        if (type_message.size() > 0)
            type.SerializeToArray(&type_message[0], type_message.size());

        ProcessorTaskMessageProgress prog;
        prog.set_taskid(taskId);
        prog.set_progress(progress);
        prog.set_message(message);
        vector<char> progData(prog.ByteSizeLong());
        if (progData.size() > 0)
            prog.SerializeToArray(&progData[0], progData.size());

        MqMessage msg;
        msg.emplace_back(hostId);
        msg.emplace_back("");
        msg.emplace_back(std::move(type_message));
        msg.emplace_back(std::move(progData));
        msg.send(*socket);
    }

    ProcessorTaskModelResponse ModelTask::process(
        ProcessorTaskModelRequest& request,
        const std::string& hostId,
        zmq::socket_t* socket)
    {
        updateProgress(hostId, request.taskid(), socket, 0.0f, "Reading model file");

        Importer importer;
        auto scene = importer.ReadFileFromMemory(request.modeldata().data(), request.modeldata().size(),
            aiProcess_CalcTangentSpace |
            aiProcess_Triangulate |
            aiProcess_JoinIdenticalVertices |
            aiProcess_SortByPType |
            aiProcess_GenSmoothNormals);

        if(!scene)
            return {};

        std::vector<char> modelData;
        std::vector<char> prefabData;

        auto debugMsg = [](size_t meshNum, size_t numMeshes, const std::string& msg)->std::string
        {
            string resmsg = msg;
            resmsg += " ";
            resmsg += to_string(meshNum + 1);
            resmsg += "/";
            resmsg += to_string(numMeshes);
            return resmsg;
        };
        auto progress = [](size_t meshNum, size_t numMeshes)->float
        {
            return static_cast<float>(meshNum) / static_cast<float>(numMeshes);
        };

        if (scene->HasMeshes())
        {
            engine::Mesh mesh;
            mesh.setFilename("");

            float pieceProgress = 1.0f / static_cast<float>(scene->mNumMeshes);
            float sectionProgress = pieceProgress / 9.0f;

            for (size_t meshNum = 0; meshNum < scene->mNumMeshes; ++meshNum)
            {
                // PROGRESS: START
                
                updateProgress(hostId, request.taskid(), 
                    socket, progress(meshNum, scene->mNumMeshes),
                    debugMsg(meshNum, scene->mNumMeshes, "Processing submesh vertice"));

                auto assmesh = scene->mMeshes[meshNum];
                engine::SubMesh subMesh;

                engine::BoundingBox meshBoundingBox;
                meshBoundingBox.min = { std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
                meshBoundingBox.max = { std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min() };

                // vertices
                std::vector<Vector3f> tempVerts(assmesh->mNumVertices);
                if (assmesh->HasFaces())
                {
                    subMesh.out_position.resize(assmesh->mNumVertices);

                    for (int vert = 0; vert < static_cast<int>(assmesh->mNumVertices); ++vert)
                    {
                        /*subMesh.out_position[vert][0] = assmesh->mVertices[vert].x;
                        subMesh.out_position[vert][1] = assmesh->mVertices[vert].y;
                        subMesh.out_position[vert][2] = assmesh->mVertices[vert].z;*/

                        tempVerts[vert][0] = assmesh->mVertices[vert].x;
                        tempVerts[vert][1] = assmesh->mVertices[vert].y;
                        tempVerts[vert][2] = assmesh->mVertices[vert].z;

                        if (assmesh->mVertices[vert].x < meshBoundingBox.min.x) meshBoundingBox.min.x = assmesh->mVertices[vert].x;
                        if (assmesh->mVertices[vert].y < meshBoundingBox.min.y) meshBoundingBox.min.y = assmesh->mVertices[vert].y;
                        if (assmesh->mVertices[vert].z < meshBoundingBox.min.z) meshBoundingBox.min.z = assmesh->mVertices[vert].z;

                        if (assmesh->mVertices[vert].x > meshBoundingBox.max.x) meshBoundingBox.max.x = assmesh->mVertices[vert].x;
                        if (assmesh->mVertices[vert].y > meshBoundingBox.max.y) meshBoundingBox.max.y = assmesh->mVertices[vert].y;
                        if (assmesh->mVertices[vert].z > meshBoundingBox.max.z) meshBoundingBox.max.z = assmesh->mVertices[vert].z;
                    }

                    // determine biggest distance from origo
                    float biggestDistance = 0.0f;
                    for (auto&& vert : tempVerts)
                    {
                        if (fabs(vert.x) > biggestDistance)
                            biggestDistance = fabs(vert.x);
                        if (fabs(vert.y) > biggestDistance)
                            biggestDistance = fabs(vert.y);
                        if (fabs(vert.z) > biggestDistance)
                            biggestDistance = fabs(vert.z);
                    }

                    ASSERT(biggestDistance <= 10000.0f, "Currently importers biggest vertex position is +-1000: vertex position distance: %f", biggestDistance);
                    float multiplier = 10000.0f / 65535.0f;
                    float distanceQuantized = std::ceilf(biggestDistance / multiplier);
                    float divider = multiplier * distanceQuantized;

                    // scale all the verts with that value
                    for (int i = 0; i < static_cast<int>(assmesh->mNumVertices); ++i)
                    {
                        Vector3f scaledVert = ((tempVerts[i] / divider) + 1.0f) / 2.0f;
                        uint32_t xpart = static_cast<uint32_t>(scaledVert.x * 65535.0);
                        uint32_t ypart = static_cast<uint32_t>(scaledVert.y * 65535.0);
                        uint32_t zpart = static_cast<uint32_t>(scaledVert.z * 65535.0);
                        uint32_t wpart = static_cast<uint32_t>(distanceQuantized);
                        subMesh.out_position[i].x = ((xpart & 0xffff) << 16) | (ypart & 0xffff);
                        subMesh.out_position[i].y = ((zpart & 0xffff) << 16) | (wpart & 0xffff);
                    }

                    // calculare error
                    float maxError = 0.0f;
                    for (int i = 0; i < static_cast<int>(assmesh->mNumVertices); ++i)
                    {
                        uint32_t xpart = (subMesh.out_position[i].x & 0xffff0000) >> 16;
                        uint32_t ypart = (subMesh.out_position[i].x & 0xffff);
                        uint32_t zpart = (subMesh.out_position[i].y & 0xffff0000) >> 16;
                        uint32_t wpart = (subMesh.out_position[i].y & 0xffff);

                        float mult = static_cast<float>(wpart) * multiplier;
                        Vector3f vert{
                            (((static_cast<float>(xpart) / 65535.0f) * 2.0f) - 1.0f) * mult,
                            (((static_cast<float>(ypart) / 65535.0f) * 2.0f) - 1.0f) * mult,
                            (((static_cast<float>(zpart) / 65535.0f) * 2.0f) - 1.0f) * mult
                        };
                        if (fabs(vert.x - tempVerts[i].x) > maxError)
                            maxError = fabs(vert.x - tempVerts[i].x);
                    }
                    LOG_INFO("Model vertex packing max error: %f", maxError);
                }

                /*emit assetWorkProgress(m_sourceFilePath, pieceProgress + sectionProgress);
                subModelStatusStr = "Processing submodel: ";
                subModelStatusStr += QString::number(meshNum, 10);
                subModelStatusStr += ", extracting normals";
                emit assetWorkStatusChange(m_sourceFilePath, subModelStatusStr);*/

                subMesh.boundingBox = meshBoundingBox;

                // normals
                if (assmesh->HasNormals())
                {
                    subMesh.out_normal.resize(assmesh->mNumVertices);
                    for (int vert = 0; vert < static_cast<int>(assmesh->mNumVertices); ++vert)
                    {
                        auto normal = packNormalOctahedron(Vector3f(assmesh->mNormals[vert].x, assmesh->mNormals[vert].y, assmesh->mNormals[vert].z));
                        subMesh.out_normal[vert][0] = normal.x;
                        subMesh.out_normal[vert][1] = normal.y;
                    }
                }

                /*emit assetWorkProgress(m_sourceFilePath, pieceProgress + (sectionProgress * 2.0f));
                subModelStatusStr = "Processing submodel: ";
                subModelStatusStr += QString::number(meshNum, 10);
                subModelStatusStr += ", extracting tangents";
                emit assetWorkStatusChange(m_sourceFilePath, subModelStatusStr);*/

                // tangents and bitangents
                if (assmesh->HasTangentsAndBitangents())
                {
                    subMesh.out_tangent.resize(assmesh->mNumVertices);
                    for (int vert = 0; vert < static_cast<int>(assmesh->mNumVertices); ++vert)
                    {
                        auto normal = packNormalOctahedron(Vector3f(assmesh->mTangents[vert].x, assmesh->mTangents[vert].y, assmesh->mTangents[vert].z));
                        subMesh.out_tangent[vert][0] = normal.x;
                        subMesh.out_tangent[vert][1] = normal.y;
                    }
                }
                else
                {
                    // this is really unfortunate. so we'll just make shit up.
                    subMesh.out_tangent.resize(assmesh->mNumVertices);
                    for (int i = 0; i < static_cast<int>(assmesh->mNumVertices); ++i)
                    {
                        Vector3f normal = Vector3f(assmesh->mNormals[i].x, assmesh->mNormals[i].y, assmesh->mNormals[i].z);
                        Vector3f tangent = normal.cross(Vector3f(0.0f, 0.0f, 1.0f)).normalize();

                        auto tang = packNormalOctahedron(Vector3f(tangent.x, tangent.y, tangent.z));
                        subMesh.out_tangent[i][0] = tang.x;
                        subMesh.out_tangent[i][1] = tang.y;
                    }
                }

                /*emit assetWorkProgress(m_sourceFilePath, pieceProgress + (sectionProgress * 3.0f));
                subModelStatusStr = "Processing submodel: ";
                subModelStatusStr += QString::number(meshNum, 10);
                subModelStatusStr += ", extracting uv";
                emit assetWorkStatusChange(m_sourceFilePath, subModelStatusStr);*/

                // uv
                subMesh.out_uv.resize(assmesh->GetNumUVChannels());
                for (int uvindex = 0; uvindex < subMesh.out_uv.size(); ++uvindex)
                {
                    ASSERT(assmesh->mNumUVComponents[uvindex] == 2, "We support only 2 component UV maps currently");
                    subMesh.out_uv[uvindex].resize(assmesh->mNumVertices);
                    for (int i = 0; i < static_cast<int>(assmesh->mNumVertices); ++i)
                    {
                        // TODO: support for UV channels
                        subMesh.out_uv[uvindex][i][0] = assmesh->mTextureCoords[uvindex][i].x;
                        subMesh.out_uv[uvindex][i][1] = assmesh->mTextureCoords[uvindex][i].y;
                    }
                }


                /*emit assetWorkProgress(m_sourceFilePath, pieceProgress + (sectionProgress * 4.0f));
                subModelStatusStr = "Processing submodel: ";
                subModelStatusStr += QString::number(meshNum, 10);
                subModelStatusStr += ", extracting colors";
                emit assetWorkStatusChange(m_sourceFilePath, subModelStatusStr);*/

                // colors
                if (assmesh->HasVertexColors(0))
                {
                    subMesh.out_colors.resize(assmesh->mNumVertices);
                    for (int i = 0; i < static_cast<int>(assmesh->mNumVertices); ++i)
                    {
                        subMesh.out_colors[i][0] = static_cast<unsigned char>(assmesh->mColors[0][i].r * 255.0f);
                        subMesh.out_colors[i][1] = static_cast<unsigned char>(assmesh->mColors[0][i].g * 255.0f);
                        subMesh.out_colors[i][2] = static_cast<unsigned char>(assmesh->mColors[0][i].b * 255.0f);
                        subMesh.out_colors[i][3] = static_cast<unsigned char>(assmesh->mColors[0][i].a * 255.0f);
                    }
                }
                else
                {
                    subMesh.out_colors.resize(assmesh->mNumVertices);
                    for (int i = 0; i < static_cast<int>(assmesh->mNumVertices); ++i)
                    {
                        subMesh.out_colors[i][0] = 0;
                        subMesh.out_colors[i][1] = 0;
                        subMesh.out_colors[i][2] = 0;
                        subMesh.out_colors[i][3] = 0;
                    }
                }

                /*emit assetWorkProgress(m_sourceFilePath, pieceProgress + (sectionProgress * 5.0f));
                subModelStatusStr = "Processing submodel: ";
                subModelStatusStr += QString::number(meshNum, 10);
                subModelStatusStr += ", extracting indices";
                emit assetWorkStatusChange(m_sourceFilePath, subModelStatusStr);*/

                // indices
                unsigned int gindex = 0;
                for (int i = 0; i < static_cast<int>(assmesh->mNumFaces); ++i)
                {
                    auto face = &assmesh->mFaces[i];
                    for (int a = 0; a < static_cast<int>(face->mNumIndices); a++)
                    {
                        subMesh.out_indices.emplace_back(face->mIndices[a]);
                        ++gindex;
                    }
                }

                updateProgress(hostId, request.taskid(),
                    socket, progress(meshNum, scene->mNumMeshes),
                    debugMsg(meshNum, scene->mNumMeshes, "Clustering submesh"));

                engine::Clusterize clusterizer;
                auto newIndices = clusterizer.clusterize3(tempVerts, subMesh.out_indices);
                subMesh.out_indices = newIndices;

                // create adjacency data
                /*emit assetWorkProgress(m_sourceFilePath, pieceProgress + (sectionProgress * 5.0f));
                subModelStatusStr = "Processing submodel: ";
                subModelStatusStr += QString::number(meshNum, 10);
                subModelStatusStr += ", creating adjacency data";
                emit assetWorkStatusChange(m_sourceFilePath, subModelStatusStr);*/

                updateProgress(hostId, request.taskid(),
                    socket, progress(meshNum, scene->mNumMeshes),
                    debugMsg(meshNum, scene->mNumMeshes, "Generating submesh adjacency"));

                subMesh.out_adjacency = meshGenerateAdjacency(subMesh.out_indices, tempVerts);

                // perform mesh optimization and clusterizing
#if 0
                emit assetWorkProgress(m_sourceFilePath, pieceProgress + (sectionProgress * 6.0f));
                subModelStatusStr = "Processing submodel: ";
                subModelStatusStr += QString::number(meshNum, 10);
                subModelStatusStr += ", clusterizing mesh";
                emit assetWorkStatusChange(m_sourceFilePath, subModelStatusStr);

                m_imageMutex.lock();
                unsigned int faceCount = static_cast<unsigned int>(subMesh.out_indices.size() / 3);
                std::vector<unsigned int> faceClusters(faceCount + 1);
                std::vector<unsigned int> faceRemap(faceCount);
                auto clusterizerResult = TootleClusterMesh(
                    subMesh.out_position.data(),
                    subMesh.out_indices.data(),
                    static_cast<unsigned int>(subMesh.out_position.size()),
                    faceCount,
                    3 * sizeof(float),
                    0,
                    &subMesh.out_indices[0],
                    &faceClusters[0],
                    nullptr //&faceRemap[0]
                );
                ASSERT(clusterizerResult == TOOTLE_OK, "AMD Tootle failed to clusterize mesh!");

                emit assetWorkProgress(m_sourceFilePath, pieceProgress + (sectionProgress * 7.0f));
                subModelStatusStr = "Processing submodel: ";
                subModelStatusStr += QString::number(meshNum, 10);
                subModelStatusStr += ", optimizing vertex cache";
                emit assetWorkStatusChange(m_sourceFilePath, subModelStatusStr);

                auto vertexCacheOptimizeResult = TootleVCacheClusters(
                    subMesh.out_indices.data(),
                    faceCount,
                    static_cast<unsigned int>(subMesh.out_position.size()),
                    TOOTLE_DEFAULT_VCACHE_SIZE,
                    faceClusters.data(),
                    &subMesh.out_indices[0],
                    nullptr, //&faceRemap[0],
                    TOOTLE_VCACHE_AUTO
                );
                ASSERT(vertexCacheOptimizeResult == TOOTLE_OK, "AMD Tootle could not optimize vertex cache");


                emit assetWorkProgress(m_sourceFilePath, pieceProgress + (sectionProgress * 8.0f));
                subModelStatusStr = "Processing submodel: ";
                subModelStatusStr += QString::number(meshNum, 10);
                subModelStatusStr += ", optimizing overdraw";
                emit assetWorkStatusChange(m_sourceFilePath, subModelStatusStr);

                std::vector<unsigned int> clusterRemap(faceClusters.back());
                auto optimizeOverdrawResult = TootleOptimizeOverdraw(
                    subMesh.out_position.data(),
                    subMesh.out_indices.data(),
                    static_cast<unsigned int>(subMesh.out_position.size()),
                    faceCount,
                    3 * sizeof(float),
                    nullptr,
                    0,
                    TootleFaceWinding::TOOTLE_CCW,
                    faceClusters.data(),
                    &subMesh.out_indices[0],
                    &clusterRemap[0],
                    TOOTLE_OVERDRAW_AUTO
                );
                ASSERT(optimizeOverdrawResult == TOOTLE_OK, "AMD Tootle could not optimize for overdraw");

                // gather clusters

                emit assetWorkProgress(m_sourceFilePath, pieceProgress + (sectionProgress * 9.0f));
                subModelStatusStr = "Processing submodel: ";
                subModelStatusStr += QString::number(meshNum, 10);
                subModelStatusStr += ", gathering clusters";
                emit assetWorkStatusChange(m_sourceFilePath, subModelStatusStr);

                //       face id     , face count
                std::map<unsigned int, unsigned int> clusters;
                int clusterId = -1;
                int lastIndex = -1;
                int clusterFaces = 0;
                for (unsigned int index = 0; index < faceClusters.size() - 1; ++index)
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

                emit assetWorkProgress(m_sourceFilePath, pieceProgress + (sectionProgress * 10.0f));
                subModelStatusStr = "Processing submodel: ";
                subModelStatusStr += QString::number(meshNum, 10);
                subModelStatusStr += ", creating bounding box";
                emit assetWorkStatusChange(m_sourceFilePath, subModelStatusStr);


                subMesh.clusterId.resize(clusters.size());
                subMesh.clusterIndexCount.resize(clusters.size());
                subMesh.out_clusterBounds.resize(clusters.size());

                int index = 0;
                int vertexIndex = 0;
                for (auto&& c : clusters)
                {
                    subMesh.clusterId[index] = c.first;
                    subMesh.clusterIndexCount[index] = c.second * 3;

                    BoundingBox bb;
                    bb.min = { std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
                    bb.max = { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest() };
                    for (unsigned int i = 0; i < c.second; ++i)
                    {
                        for (int a = 0; a < 3; ++a)
                        {
                            Vector3f pos = subMesh.out_position[subMesh.out_indices[vertexIndex]];
                            if (pos.x < bb.min.x) bb.min.x = pos.x;
                            if (pos.y < bb.min.y) bb.min.y = pos.y;
                            if (pos.z < bb.min.z) bb.min.z = pos.z;
                            if (pos.x > bb.max.x) bb.max.x = pos.x;
                            if (pos.y > bb.max.y) bb.max.y = pos.y;
                            if (pos.z > bb.max.z) bb.max.z = pos.z;
                            ++vertexIndex;
                        }
                    }

                    subMesh.out_clusterBounds[index] = bb;
                    ++index;
                }
#else
                /*emit assetWorkProgress(m_sourceFilePath, pieceProgress + (sectionProgress * 6.0f));
                subModelStatusStr = "Processing submodel: ";
                subModelStatusStr += QString::number(meshNum, 10);
                subModelStatusStr += ", optimizing mesh";
                emit assetWorkStatusChange(m_sourceFilePath, subModelStatusStr);*/

                /*m_imageMutex.lock();

                unsigned int clusters = 0;
                auto tootleRes = TootleOptimize(
                subMesh.out_position.data(),
                subMesh.out_indices.data(),
                static_cast<unsigned int>(subMesh.out_position.size()),
                static_cast<unsigned int>(subMesh.out_indices.size() / 3),
                3 * sizeof(float),
                TOOTLE_DEFAULT_VCACHE_SIZE,
                nullptr,
                0,
                TootleFaceWinding::TOOTLE_CCW,
                &subMesh.out_indices[0],
                &clusters,
                TootleVCacheOptimizer::TOOTLE_VCACHE_AUTO);
                ASSERT(tootleRes == TOOTLE_OK, "AMD Tootle encountered an error");

                m_imageMutex.unlock();*/

                /*emit assetWorkProgress(m_sourceFilePath, pieceProgress + (sectionProgress * 7.0f));
                subModelStatusStr = "Processing submodel: ";
                subModelStatusStr += QString::number(meshNum, 10);
                subModelStatusStr += ", clusterizing mesh";
                emit assetWorkStatusChange(m_sourceFilePath, subModelStatusStr);*/

                updateProgress(hostId, request.taskid(),
                    socket, progress(meshNum, scene->mNumMeshes),
                    debugMsg(meshNum, scene->mNumMeshes, "Creating submesh clusters & BB"));

                auto clusterCount = subMesh.out_indices.size() / ClusterMaxSize;
                auto extraIndices = subMesh.out_indices.size() - (clusterCount * ClusterMaxSize);
                if (extraIndices > 0)
                    ++clusterCount;

                subMesh.clusterId.resize(clusterCount);
                subMesh.clusterIndexCount.resize(clusterCount);
                subMesh.out_clusterBounds.resize(clusterCount);

                for (int i = 0; i < clusterCount; ++i)
                {
                    subMesh.clusterId[i] = i * ClusterMaxSize;
                    subMesh.clusterIndexCount[i] = ClusterMaxSize;

                    if (i == clusterCount - 1 && extraIndices > 0)
                        subMesh.clusterIndexCount[i] = static_cast<uint32_t>(extraIndices);

                    engine::BoundingBox bb;
                    bb.min = { std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
                    bb.max = { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest() };

                    auto vertexIndex = subMesh.clusterId[i];
                    for (unsigned int v = 0; v < subMesh.clusterIndexCount[i]; ++v)
                    {
                        Vector3f pos = tempVerts[subMesh.out_indices[vertexIndex]];
                        if (pos.x < bb.min.x) bb.min.x = pos.x;
                        if (pos.y < bb.min.y) bb.min.y = pos.y;
                        if (pos.z < bb.min.z) bb.min.z = pos.z;
                        if (pos.x > bb.max.x) bb.max.x = pos.x;
                        if (pos.y > bb.max.y) bb.max.y = pos.y;
                        if (pos.z > bb.max.z) bb.max.z = pos.z;
                        ++vertexIndex;
                    }

                    subMesh.out_clusterBounds[i] = bb;
                }
#endif
                updateProgress(hostId, request.taskid(),
                    socket, progress(meshNum, scene->mNumMeshes),
                    debugMsg(meshNum, scene->mNumMeshes, "Parsing submesh materials"));

                /*emit assetWorkProgress(m_sourceFilePath, pieceProgress + (sectionProgress * 8.0f));
                subModelStatusStr = "Processing submodel: ";
                subModelStatusStr += QString::number(meshNum, 10);
                subModelStatusStr += ", processing materials";
                emit assetWorkStatusChange(m_sourceFilePath, subModelStatusStr);*/

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
                    aiTextureType_REFLECTION,
                    aiTextureType_UNKNOWN
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
                            subMesh.out_material.textures.emplace_back(MaterialTexture{
                                std::string(&path.data[0], path.length),
                                mapping(textureMapping),
                                mappingMode(mode),
                                textureType(texType),
                                textureOp(op),
                                uvIndex
                                });
                            hadMaterial = true;
                            //qDebug() << "Mesh has material: " << std::string(&path.data[0], path.length).data();
                            LOG("Mesh has material: %s", std::string(&path.data[0], path.length).c_str());
                        }
                    }
                }
                if (!hadMaterial)
                {
                    //qDebug() << "No material for mesh found.";
                    LOG("No material for mesh found.");
                }

                mesh.subMeshes().emplace_back(subMesh);
            }
            mesh.saveToMemory(modelData);

            if (scene->mRootNode)
            {
                shared_ptr<SceneNode> node = make_shared<SceneNode>();

                /*auto filename = pathExtractFilename(stdSourcePath);
                auto ext = pathExtractExtension(filename);
                auto plainFilename = filename.substr(0, filename.length() - ext.length() - 1);*/
                node->name(request.assetname());

                shared_ptr<Transform> rootTransform = make_shared<Transform>();
                node->addComponent(rootTransform);

                Vector3f scale{ request.scalex(), request.scaley(), request.scalez() };
                Quaternionf rotation{ request.rotationx(), request.rotationy(), request.rotationz(), request.rotationw() };

                Matrix4f transform;
                createScene(mesh, scale, rotation, request.modeltargetpath(), scene->mRootNode, node);

                rapidjson::StringBuffer strBuffer;
                rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(strBuffer);
                node->serialize(writer);

                /*ofstream outFile;
                outFile.open(pathReplaceExtension(contentSourcePath, "prefab"));
                outFile.write(strBuffer.GetString(), strBuffer.GetSize());
                outFile.close();*/
                prefabData.resize(strBuffer.GetSize());
                memcpy(&prefabData[0], strBuffer.GetString(), prefabData.size());
            }
        }

        ProcessorTaskModelResponse response;
        response.set_modeldata(modelData.data(), modelData.size());
        response.set_prefabdata(prefabData.data(), prefabData.size());
        response.set_taskid(request.taskid());
        return response;
    }
}
