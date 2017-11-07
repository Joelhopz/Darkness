#include "engine/EngineCore.h"
#include "tools/image/Image.h"

namespace engine
{
#if 0
    const std::vector<Vertex> mesh = {
        { { -0.5f, -0.5f, 0.0f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f } },
        { { 0.5f, -0.5f, 0.0f },{ 0.0f, 1.0f, 0.0f },{ 1.0f, 0.0f } },
        { { 0.5f, 0.5f, 0.0f },{ 0.0f, 0.0f, 1.0f },{ 1.0f, 1.0f } },
        { { -0.5f, 0.5f, 0.0f },{ 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f } } //,

        /*{ { -0.5f, -0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f } },
        { { 0.5f, -0.5f, -0.5f },{ 0.0f, 1.0f, 0.0f },{ 1.0f, 0.0f } },
        { { 0.5f, 0.5f, -0.5f },{ 0.0f, 0.0f, 1.0f },{ 1.0f, 1.0f } },
        { { -0.5f, 0.5f, -0.5f },{ 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f } }*/
    };

    const std::vector<uint32_t> meshIndices = {
        0, 1, 2, 2, 3, 0  //,
        //4, 5, 6, 6, 7, 4
    };
#endif

    EngineCore::EngineCore(
        Device& device,
        ShaderStorage& shaderStorage,
        const std::string& shaderRootPath,
        bool fullscreen,
        bool vsync,
        EngineCore* oldEngineCore)
        : m_device{ device }
        , m_currentVertexUsage{ 0 }
        , m_currentIndiceUsage{ 0 }
        , m_modelCount{ 0 }
        , swapChain{ device.createSwapChain(fullscreen, vsync, oldEngineCore ? oldEngineCore->swapChain.get() : nullptr) }
        , core{ device, shaderStorage, shaderRootPath }

        , vertexBuffer{ device.createBuffer(BufferDescription()
            .elements(InitialMaxVertexCount)
            .elementSize(sizeof(Vertex))
			.name("vertexBuffer")
			.usage(ResourceUsage::CpuToGpu)) }

		, vertexBufferView{ device.createBufferVBV(vertexBuffer) }

        , indexBuffer{ device.createBuffer(BufferDescription()
            .elements(InitialMaxVertexCount)
            .elementSize(sizeof(uint32_t))
			.name("vertexStaging")
			.usage(ResourceUsage::CpuToGpu)) }

		, indexBufferView{ device.createBufferIBV(indexBuffer) }

        , imageSource{ image::Image::createImage("C:\\work\\testdata\\bctest\\test2_PNG_BC7_8.DDS")}
        , image{ m_device.createTextureSRV(TextureDescription()
			.usage(ResourceUsage::CpuToGpu)
			.name("color")
			.width(static_cast<uint32_t>(imageSource->width()))
			.height(static_cast<uint32_t>(imageSource->height()))
			.format(imageSource->format())
			.arraySlices(static_cast<uint32_t>(imageSource->arraySlices()))
			.mipLevels(static_cast<uint32_t>(imageSource->mipCount()))
			.setInitialData(TextureDescription::InitialData(
				std::vector<uint8_t>(imageSource->data(), imageSource->data() + imageSource->bytes()),
				static_cast<uint32_t>(imageSource->width()),
				static_cast<uint32_t>(imageSource->width()) * static_cast<uint32_t>(imageSource->height())))) }

        , depthBuffer{ device.createTexture(TextureDescription()
            .width(swapChain->size().width)
            .height(swapChain->size().height)
			.name("staging")
            .format(Format::Format_D32_FLOAT)
            ) }
        , depthView{ std::make_shared<TextureDSV>(device.createTextureDSV(depthBuffer)) }

        , sampler{ device.createSampler(SamplerDescription()) }
    {
		/*core.mainPipeline = m_device.createPipeline<shaders::HLSLTest>(shaderStorage);
		core.mainPipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleStrip);
		core.mainPipeline.setRasterizerState(RasterizerDescription().frontCounterClockwise(false));
		core.mainPipeline.setSampler(sampler);
		core.mainPipeline.setDepthBufferView(depthView);
		core.mainPipeline.vs. = image;

        core.mainPipeline.setDepthBufferView(depthView);
        core.mainPipeline.setSampler(sampler);
        core.mainPipeline.setTextureSRV(imageView);
        core.mainPipeline.finalize();

        {
            CommandList commandList{ device.createCommandList() };
            commandList.begin();
            commandList.transitionTexture(imageStaging, ImageLayout::Preinitialized, ImageLayout::TransferSrcOptimal);
            commandList.transitionTexture(image, ImageLayout::Preinitialized, ImageLayout::TransferDstOptimal);
            commandList.copyTexture(imageStaging, image);
            commandList.transitionTexture(image, ImageLayout::TransferDstOptimal, ImageLayout::ShaderReadOnlyOptimal);

            commandList.transitionTexture(depthBuffer, ImageLayout::Undefined, ImageLayout::DepthStencilAttachmentOptimal);
            commandList.end();

            Fence fence = device.createFence();
			m_device.queue().submit(commandList, fence);
            fence.blockUntilSignaled();
        }


        void* data = vertexStagingBuffer.map(m_device);
        memset(data, 0, InitialMaxVertexCount * sizeof(Vertex));
        vertexStagingBuffer.unmap(m_device);

        data = indexStagingBuffer.map(m_device);
        memset(data, 0, InitialMaxIndiceCount * sizeof(uint32_t));
        indexStagingBuffer.unmap(m_device);

        auto cmdBuffer = device.createCommandList();
        cmdBuffer.begin();
        cmdBuffer.copyBuffer(vertexStagingBuffer, vertexBuffer, InitialMaxVertexCount);
        cmdBuffer.copyBuffer(indexStagingBuffer, indexBuffer, InitialMaxIndiceCount);
        cmdBuffer.end();
        Fence fence = device.createFence();
        m_device.queue().submit(cmdBuffer, fence);
        fence.blockUntilSignaled();

        updateCommandlists();*/
    }


    void EngineCore::insertMesh(std::shared_ptr<MeshRenderer> meshRenderer)
    {
        /*meshRenderer->refresh();

        auto allocation = allocate(meshRenderer->vertices().size(), meshRenderer->indices().size());
        meshRenderer->vertexBufferPosition(allocation.first);
        meshRenderer->indexBufferPosition(allocation.second);
        meshRenderer->hasBufferPositions(true);

        void* data = vertexStagingBuffer.map(m_device);
        memcpy(reinterpret_cast<unsigned char*>(data) + (allocation.first * sizeof(Vertex)),
            meshRenderer->vertices().data(), meshRenderer->vertices().size() * sizeof(Vertex));
        vertexStagingBuffer.unmap(m_device);

        data = indexStagingBuffer.map(m_device);
        memcpy(reinterpret_cast<unsigned char*>(data) + (allocation.second * sizeof(uint32_t)),
            meshRenderer->indices().data(), meshRenderer->indices().size() * sizeof(uint32_t));
        indexStagingBuffer.unmap(m_device);

        auto cmdBuffer = m_device.createCommandList();
        cmdBuffer.begin();
        cmdBuffer.copyBuffer(vertexStagingBuffer, vertexBuffer, InitialMaxVertexCount);
        cmdBuffer.copyBuffer(indexStagingBuffer, indexBuffer, InitialMaxIndiceCount);
        cmdBuffer.end();
        Fence fence = m_device.createFence();
        m_device.queue().submit(cmdBuffer, fence);
        fence.blockUntilSignaled();

        updateCommandlists();

        ++m_modelCount;*/
    }

    size_t EngineCore::modelCount() const
    {
        return m_modelCount;
    }

    void EngineCore::updateCommandlists()
    {
        /*commandLists.clear();
        commandLists.emplace_back(m_device.createCommandList());
        commandLists.emplace_back(m_device.createCommandList());
        int index = 0;
        for (auto& commandBuffer : commandLists)
        {
            //commandBuffer.reset(core.mainPipeline->pipeline());
            commandBuffer.begin();
            //commandBuffer.beginRenderPass(core.mainPipeline->pipeline(), index);

            
            commandBuffer.bindVertexBuffer(vertexBufferView);
            commandBuffer.bindIndexBuffer(indexBufferView);
            commandBuffer.bindPipe(core.mainPipeline);

            //commandBuffer.bindDescriptorSets(core.mainPipeline, uniformDescriptor);
            commandBuffer.drawIndexed(static_cast<uint32_t>(m_currentIndiceUsage), 1, 0, 0, 0);

            commandBuffer.endRenderPass();
            commandBuffer.end();
            ++index;
        }*/
    }

    bool EngineCore::needRefresh() const
    {
        return swapChain->needRefresh() || m_device.queue().needRefresh();
    }
}
