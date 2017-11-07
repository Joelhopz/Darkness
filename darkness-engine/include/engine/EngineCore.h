#pragma once

#include "engine/graphics/Graphics.h"
#include "engine/graphics/Common.h"
#include "engine/rendering/Core.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/rendering/Mesh.h"
#include "components/MeshRenderer.h"
#include "tools/image/Image.h"
#include <memory>
#include <utility>

namespace engine
{
    class Device;

    constexpr const size_t InitialMaxVertexCount = 20000;
    constexpr const size_t InitialMaxIndiceCount = 20000;

    class EngineCore
    {
    public:
        EngineCore(
            Device& device, 
            ShaderStorage& shaderStorage, 
            const std::string& shaderRootPath,
            bool fullscreen = false,
            bool vsync = true,
            EngineCore* oldEngineCore = nullptr);
        EngineCore& operator=(EngineCore&&) = delete;

        bool needRefresh() const;
        void insertMesh(std::shared_ptr<MeshRenderer> meshRenderer);
        size_t modelCount() const;

    private:
        Device& m_device;
        size_t m_currentVertexUsage;
        size_t m_currentIndiceUsage;
        size_t m_modelCount;

    public:
        std::shared_ptr<engine::SwapChain> swapChain;
        engine::Core core;
        Buffer vertexBuffer;
        Buffer indexBuffer;
		BufferVBV vertexBufferView;
		BufferIBV indexBufferView;
        std::unique_ptr<engine::image::ImageIf> imageSource;
        TextureSRV image;
        Texture depthBuffer;
        std::shared_ptr<TextureDSV> depthView;
        Sampler sampler;
        std::vector<engine::CommandList> commandLists;

    private:
        void updateCommandlists();

        std::pair<size_t, size_t> allocate(size_t vertexCount, size_t indiceCount)
        {
            ASSERT(m_currentVertexUsage + vertexCount < InitialMaxVertexCount);
            ASSERT(m_currentIndiceUsage + indiceCount < InitialMaxIndiceCount);
            std::pair<size_t, size_t> result{ m_currentVertexUsage, m_currentIndiceUsage };
            m_currentVertexUsage += vertexCount;
            m_currentIndiceUsage += indiceCount;
            return result;
        }
        
    };
}
