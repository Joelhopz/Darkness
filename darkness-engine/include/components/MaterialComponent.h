#pragma once

#include "engine/EngineComponent.h"
#include "engine/graphics/Resources.h"
#include "engine/graphics/Device.h"
#include "tools/image/Image.h"
#include "tools/Property.h"
#include "tools/hash/Hash.h"
#include "platform/File.h"

namespace engine
{
    class MaterialComponent : public EngineComponent
    {
        Property m_color;

        bool m_hasAlbedo;
        bool m_hasRoughness;
        bool m_hasNormal;
        bool m_hasMetalness;
        bool m_hasOcclusion;

        Property m_albedoPath;
        Property m_normalPath;
        Property m_roughnessPath;
        Property m_roughnessStrength;
        Property m_metalnessPath;
        Property m_metalnessStrength;
        Property m_occlusionPath;
        Property m_occlusionStrength;
        Property m_materialScaleX;
        Property m_materialScaleY;
        Property m_transparent;
        Property m_alphaclipped;

        bool m_cpuDirty[static_cast<int>(TextureType::COUNT)];
        bool m_gpuDirty[static_cast<int>(TextureType::COUNT)];

        std::shared_ptr<image::ImageIf> m_albedo;
        std::shared_ptr<image::ImageIf> m_roughness;
        std::shared_ptr<image::ImageIf> m_normal;
        std::shared_ptr<image::ImageIf> m_metalness;
        std::shared_ptr<image::ImageIf> m_occlusion;

        std::unique_ptr<TextureSRV> m_albedoSrv;
        std::unique_ptr<TextureSRV> m_roughnessSrv;
        std::unique_ptr<TextureSRV> m_normalSrv;
        std::unique_ptr<TextureSRV> m_metalnessSrv;
        std::unique_ptr<TextureSRV> m_occlusionSrv;

        bool m_albedoChanged;
        bool m_roughnessChanged;
        bool m_normalChanged;
        bool m_metalnessChanged;
        bool m_occlusionChanged;

        ResourceKey m_albedoKey;
        ResourceKey m_roughnessKey;
        ResourceKey m_normalKey;
        ResourceKey m_metalnessKey;
        ResourceKey m_occlusionKey;

        TextureSRV createTexture(ResourceKey key, Device& device, image::ImageIf* image, bool srgb = false)
        {
            return device.createTextureSRV(key, TextureDescription()
                .usage(ResourceUsage::CpuToGpu)
                .name("color")
                .width(static_cast<uint32_t>(image->width()))
                .height(static_cast<uint32_t>(image->height()))
                .format(srgb ? srgbFormat(image->format()) : image->format())
                .arraySlices(static_cast<uint32_t>(image->arraySlices()))
                .mipLevels(static_cast<uint32_t>(image->mipCount()))
                .setInitialData(TextureDescription::InitialData(
                    std::vector<uint8_t>(image->data(), image->data() + image->bytes()),
                    static_cast<uint32_t>(image->width()), static_cast<uint32_t>(image->width() * image->height()))));
        }

    public:

        std::shared_ptr<EngineComponent> clone() const override
        {
            auto res = std::make_shared<engine::MaterialComponent>(
                m_albedoPath.value<std::string>(),
                m_roughnessPath.value<std::string>(),
                m_normalPath.value<std::string>(),
                m_metalnessPath.value<std::string>(),
                m_occlusionPath.value<std::string>());
            res->m_hasAlbedo = m_hasAlbedo;
            res->m_hasRoughness = m_hasRoughness;
            res->m_hasNormal = m_hasNormal;
            res->m_hasMetalness = m_hasMetalness;
            res->m_hasOcclusion = m_hasOcclusion;

            res->roughnessStrength(roughnessStrength());
            res->metalnessStrength(metalnessStrength());
            res->occlusionStrength(occlusionStrength());
            res->materialScaleX(materialScaleX());
            res->materialScaleY(materialScaleY());
            res->transparent(transparent());
            res->alphaclipped(alphaclipped());
            res->name(m_name);
            return res;
        }

        MaterialComponent()
            : m_color{ this, "Color", Vector3f{ 1.0f, 1.0f, 1.0f } }
            , m_hasAlbedo{ false }
            , m_hasRoughness{ false }
            , m_hasNormal{ false }
            , m_hasMetalness{ false }
            , m_hasOcclusion{ false }
            , m_albedoPath{ this, "Albedo", std::string(""), [this]() { this->m_cpuDirty[static_cast<int>(TextureType::Albedo)] = true; this->m_albedoChanged = true; m_hasAlbedo = checkMaterialFile(m_albedoPath, "albedo"); } }
            , m_normalPath{ this, "Normal", std::string(""), [this]() { this->m_cpuDirty[static_cast<int>(TextureType::Normal)] = true; this->m_normalChanged = true; m_hasNormal = checkMaterialFile(m_normalPath, "normal"); } }
            , m_roughnessPath{ this, "Roughness", std::string(""), [this]() { this->m_cpuDirty[static_cast<int>(TextureType::Roughness)] = true; this->m_roughnessChanged = true; m_hasRoughness = checkMaterialFile(m_roughnessPath, "roughness"); } }
            , m_roughnessStrength{ this, "Roughness Strength", 1.0f }
            , m_metalnessPath{ this, "Metalness", std::string(""), [this]() { this->m_cpuDirty[static_cast<int>(TextureType::Metalness)] = true; this->m_metalnessChanged = true; m_hasMetalness = checkMaterialFile(m_metalnessPath, "metalness"); } }
            , m_metalnessStrength{ this, "Metalness Strength", 1.0f }
            , m_occlusionPath{ this, "Occlusion", std::string(""), [this]() { this->m_cpuDirty[static_cast<int>(TextureType::Occlusion)] = true; this->m_occlusionChanged = true; m_hasOcclusion = checkMaterialFile(m_occlusionPath, "occlusion"); } }
            , m_occlusionStrength{ this, "Occlusion Strength", 1.0f }
            , m_materialScaleX{ this, "Scale X", 1.0f }
            , m_materialScaleY{ this, "Scale y", 1.0f }
            , m_transparent{ this, "Transparent", ButtonToggle::NotPressed }
            , m_alphaclipped{ this, "Alphaclipped", ButtonToggle::NotPressed }
            , m_cpuDirty{ true, true, true, true, true, true, true, true, true, true, true, true, true }
            , m_gpuDirty{ false, false, false, false, false, false, false, false, false, false, false, false, false }
            , m_albedoChanged{ true }
            , m_roughnessChanged{ true }
            , m_normalChanged{ true }
            , m_metalnessChanged{ true }
            , m_occlusionChanged{ true }
        {
            m_name = "MaterialComponent";
        }

        MaterialComponent(
            const std::string& albedoPath,
            const std::string& roughnessPath,
            const std::string& normalPath,
            const std::string& metalnessPath,
            const std::string& occlusionPath)
            : m_color{ this, "Color", Vector3f{ 1.0f, 1.0f, 1.0f } }
            , m_hasAlbedo{ false }
            , m_hasRoughness{ false }
            , m_hasNormal{ false }
            , m_hasMetalness{ false }
            , m_hasOcclusion{ false }
            , m_albedoPath{ this, "Albedo", albedoPath, [this]() { this->m_cpuDirty[static_cast<int>(TextureType::Albedo)] = true; this->m_albedoChanged = true; m_hasAlbedo = checkMaterialFile(m_albedoPath, "albedo"); } }
            , m_normalPath{ this, "Normal", normalPath, [this]() { this->m_cpuDirty[static_cast<int>(TextureType::Normal)] = true; this->m_normalChanged = true; m_hasNormal = checkMaterialFile(m_normalPath, "normal"); } }
            , m_roughnessPath{ this, "Roughness", roughnessPath, [this]() { this->m_cpuDirty[static_cast<int>(TextureType::Roughness)] = true; this->m_roughnessChanged = true; m_hasRoughness = checkMaterialFile(m_roughnessPath, "roughness"); } }
            , m_roughnessStrength{ this, "Roughness Strength", 1.0f }
            , m_metalnessPath{ this, "Metalness", metalnessPath, [this]() { this->m_cpuDirty[static_cast<int>(TextureType::Metalness)] = true; this->m_metalnessChanged = true; m_hasMetalness = checkMaterialFile(m_metalnessPath, "metalness"); } }
            , m_metalnessStrength{ this, "Metalness Strength", 1.0f }
            , m_occlusionPath{ this, "Occlusion", occlusionPath, [this]() { this->m_cpuDirty[static_cast<int>(TextureType::Occlusion)] = true; this->m_occlusionChanged = true; m_hasOcclusion = checkMaterialFile(m_occlusionPath, "occlusion"); } }
            , m_occlusionStrength{ this, "Occlusion Strength", 1.0f }
            , m_materialScaleX{ this, "Scale X", 1.0f }
            , m_materialScaleY{ this, "Scale y", 1.0f }
            , m_transparent{ this, "Transparent", ButtonToggle::NotPressed }
            , m_alphaclipped{ this, "Alphaclipped", ButtonToggle::NotPressed }
            , m_cpuDirty{ true, true, true, true, true, true, true, true, true, true, true, true, true }
            , m_gpuDirty{ false, false, false, false, false, false, false, false, false, false, false, false, false }
            , m_albedoChanged{ true }
            , m_roughnessChanged{ true }
            , m_normalChanged{ true }
            , m_metalnessChanged{ true }
            , m_occlusionChanged{ true }
        {
            m_name = "MaterialComponent";
            m_hasAlbedo = checkMaterialFile(m_albedoPath, "albedo");
            m_hasNormal = checkMaterialFile(m_normalPath, "normal");
            m_hasRoughness = checkMaterialFile(m_roughnessPath, "roughness");
            m_hasMetalness = checkMaterialFile(m_metalnessPath, "metalness");
            m_hasOcclusion = checkMaterialFile(m_occlusionPath, "occlusion");
        }

        TextureSRV& albedo() { return *m_albedoSrv; }
        TextureSRV& normal() { return *m_normalSrv; }
        TextureSRV& roughness() { return *m_roughnessSrv; }
        TextureSRV& metalness() { return *m_metalnessSrv; }
        TextureSRV& occlusion() { return *m_occlusionSrv; }

        Vector3f color() const { return m_color.value<Vector3f>(); }
        void color(const Vector3f& col) { m_color.value<Vector3f>(col); }

        float roughnessStrength() const { return m_roughnessStrength.value<float>(); }
        float metalnessStrength() const { return m_metalnessStrength.value<float>(); }
        float occlusionStrength() const { return m_occlusionStrength.value<float>(); }

        float materialScaleX() const { return m_materialScaleX.value<float>(); }
        float materialScaleY() const { return m_materialScaleY.value<float>(); }

        void roughnessStrength(float val) { m_roughnessStrength.value<float>(val); }
        void metalnessStrength(float val) { m_metalnessStrength.value<float>(val); }
        void occlusionStrength(float val) { m_occlusionStrength.value<float>(val); }

        void materialScaleX(float val) { m_materialScaleX.value<float>(val); }
        void materialScaleY(float val) { m_materialScaleY.value<float>(val); }

        bool albedoChanged(bool clear = false)        { bool res = m_albedoChanged; if(clear) m_albedoChanged = false; return res; }
        bool roughnessChanged(bool clear = false)    { bool res = m_roughnessChanged; if(clear) m_roughnessChanged = false; return res; }
        bool normalChanged(bool clear = false)        { bool res = m_normalChanged; if(clear) m_normalChanged = false; return res; }
        bool metalnessChanged(bool clear = false)    { bool res = m_metalnessChanged; if(clear) m_metalnessChanged = false; return res; }
        bool occlusionChanged(bool clear = false)    { bool res = m_occlusionChanged; if(clear) m_occlusionChanged = false; return res; }

        bool hasAlbedo() { return m_hasAlbedo; }
        bool hasRoughness() { return m_hasRoughness; }
        bool hasNormal() { return m_hasNormal; }
        bool hasMetalness() { return m_hasMetalness; }
        bool hasOcclusion() { return m_hasOcclusion; }

        ResourceKey albedoKey() { return m_albedoKey; }
        ResourceKey roughnessKey() { return m_roughnessKey; }
        ResourceKey normalKey() { return m_normalKey; }
        ResourceKey metalnessKey() { return m_metalnessKey; }
        ResourceKey occlusionKey() { return m_occlusionKey; }

        bool transparent() const { return static_cast<bool>(m_transparent.value<ButtonToggle>()); }
        void transparent(bool transparent) { m_transparent.value<ButtonToggle>(static_cast<ButtonToggle>(transparent)); }

        bool alphaclipped() const { return static_cast<bool>(m_alphaclipped.value<ButtonToggle>()); }
        void alphaclipped(bool alphaclipped) { m_alphaclipped.value<ButtonToggle>(static_cast<ButtonToggle>(alphaclipped)); }
        
    private:
        bool checkMaterialFile(const Property& path, const char* materialType)
        {
            auto pathStr = path.value<std::string>();
            bool hasPath = pathStr != "";

            bool foundFile = false;
            if (hasPath)
                foundFile = engine::fileExists(pathStr);

            if (hasPath && !foundFile)
                LOG_WARNING("Missing %s material file: %s", materialType, pathStr.c_str());

            return hasPath && foundFile;
        }

    public:
        void invalidateGpu()
        {
            for(int i = 0; i < static_cast<int>(TextureType::COUNT); ++i)
                m_gpuDirty[i] = true;
        }

    public:
        void cpuRefresh(Device& device)
        {
            if (m_cpuDirty[static_cast<int>(TextureType::Albedo)])
            {
                m_cpuDirty[static_cast<int>(TextureType::Albedo)] = false;
                if (hasAlbedo())
                {
                    auto path = m_albedoPath.value<std::string>();
                    m_albedoKey = tools::hash(path);
                    m_albedo = device.createImage(
                        m_albedoKey,
                        path);
                }
                else
                    m_albedo = nullptr;
                if(m_albedo)
                    m_gpuDirty[static_cast<int>(TextureType::Albedo)] = true;
            }

            if (m_cpuDirty[static_cast<int>(TextureType::Roughness)])
            {
                m_cpuDirty[static_cast<int>(TextureType::Roughness)] = false;
                if (hasRoughness())
                {
                    auto path = m_roughnessPath.value<std::string>();
                    m_roughnessKey = tools::hash(path);
                    m_roughness = device.createImage(m_roughnessKey, path);
                }
                else
                    m_roughness = nullptr;
                if(m_roughness)
                    m_gpuDirty[static_cast<int>(TextureType::Roughness)] = true;
            }

            if (m_cpuDirty[static_cast<int>(TextureType::Normal)])
            {
                m_cpuDirty[static_cast<int>(TextureType::Normal)] = false;
                if (hasNormal())
                {
                    auto path = m_normalPath.value<std::string>();
                    m_normalKey = tools::hash(path);
                    m_normal = device.createImage(m_normalKey, path);
                }
                else
                    m_normal = nullptr;
                if(m_normal)
                    m_gpuDirty[static_cast<int>(TextureType::Normal)] = true;
            }

            if (m_cpuDirty[static_cast<int>(TextureType::Metalness)])
            {
                m_cpuDirty[static_cast<int>(TextureType::Metalness)] = false;
                if (hasMetalness())
                {
                    auto path = m_metalnessPath.value<std::string>();
                    m_metalnessKey = tools::hash(path);
                    m_metalness = device.createImage(m_metalnessKey, path);
                }
                else
                    m_metalness = nullptr;
                if(m_metalness)
                    m_gpuDirty[static_cast<int>(TextureType::Metalness)] = true;
            }

            if (m_cpuDirty[static_cast<int>(TextureType::Occlusion)])
            {
                m_cpuDirty[static_cast<int>(TextureType::Occlusion)] = false;
                if (hasOcclusion())
                {
                    auto path = m_occlusionPath.value<std::string>();
                    m_occlusionKey = tools::hash(path);
                    m_occlusion = device.createImage(m_occlusionKey, path);
                }
                else
                    m_occlusion = nullptr;
                if (m_occlusion)
                    m_gpuDirty[static_cast<int>(TextureType::Occlusion)] = true;
            }
        }

        bool gpuRefresh(Device& device)
        {
            bool change = false;
            if (m_gpuDirty[static_cast<int>(TextureType::Albedo)])
            {
                m_gpuDirty[static_cast<int>(TextureType::Albedo)] = false;
                change = true;
                if (hasAlbedo())
                {
                    m_albedoSrv = std::make_unique<TextureSRV>(
                        createTexture(m_albedoKey, device, m_albedo.get(), true));
                }
                else
                    m_albedoSrv = nullptr;
            }

            if (m_gpuDirty[static_cast<int>(TextureType::Roughness)])
            {
                m_gpuDirty[static_cast<int>(TextureType::Roughness)] = false;
                change = true;
                if (hasRoughness())
                {
                    m_roughnessSrv = std::make_unique<TextureSRV>(
                        createTexture(m_roughnessKey, device, m_roughness.get()));
                }
                else
                    m_roughnessSrv = nullptr;
            }

            if (m_gpuDirty[static_cast<int>(TextureType::Normal)])
            {
                m_gpuDirty[static_cast<int>(TextureType::Normal)] = false;
                change = true;
                if (hasNormal())
                {
                    m_normalSrv = std::make_unique<TextureSRV>(
                        createTexture(m_normalKey, device, m_normal.get()));
                }
                else
                    m_normalSrv = nullptr;
            }

            if (m_gpuDirty[static_cast<int>(TextureType::Metalness)])
            {
                m_gpuDirty[static_cast<int>(TextureType::Metalness)] = false;
                change = true;
                if (hasMetalness())
                {
                    m_metalnessSrv = std::make_unique<TextureSRV>(
                        createTexture(m_metalnessKey, device, m_metalness.get()));
                }
                else
                    m_metalnessSrv = nullptr;
            }

            if (m_gpuDirty[static_cast<int>(TextureType::Occlusion)])
            {
                m_gpuDirty[static_cast<int>(TextureType::Occlusion)] = false;
                change = true;
                if (hasOcclusion())
                {
                    m_occlusionSrv = std::make_unique<TextureSRV>(
                        createTexture(m_occlusionKey, device, m_occlusion.get()));
                }
                else
                    m_occlusionSrv = nullptr;
            }
            return change;
        }
    };
}
