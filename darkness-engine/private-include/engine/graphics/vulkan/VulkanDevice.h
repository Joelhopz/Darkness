#pragma once

#include "engine/graphics/vulkan/VulkanInstance.h"
#include "engine/graphics/vulkan/VulkanHeaders.h"
#include "engine/graphics/vulkan/VulkanCommon.h"
#include "engine/graphics/Common.h"
#include "tools/ByteRange.h"
#include <memory>
#include <map>
#include <string>
#include <queue>
//struct ID3D12Device;

namespace platform
{
    class Window;
}

namespace engine
{
    class Buffer;
    class BufferSRV;
    class BufferUAV;
    class BufferIBV;
    class BufferCBV;
    class BufferVBV;
    class Texture;
    class Queue;
    class Device;
    class CommandList;
    class DescriptorHandle;
    struct NullResources;
    enum class Format;
    struct TextureDescription;

    namespace implementation
    {
        class CommandAllocatorImpl;
        class CommandListImpl;
        struct QueueInfo
        {
            int queueFamilyIndex;
            int queueIndex;
        };
        static const int InvalidFamilyIndex = -1;

        class DeviceImpl;
        namespace device_implementation
        {
            static std::map<const DeviceImpl*, std::vector<VulkanQueue>> queues;
        }

        class DeviceImpl
        {
        public:
            DeviceImpl(std::shared_ptr<platform::Window> window);
            ~DeviceImpl();

            DeviceImpl(const DeviceImpl&) = delete;
            DeviceImpl(DeviceImpl&&) = delete;
            DeviceImpl& operator=(const DeviceImpl&) = delete;
            DeviceImpl& operator=(DeviceImpl&&) = delete;

            void nullResources(std::shared_ptr<NullResources> nullResources);
            NullResources& nullResources();

            void recycleUploadBuffers(uint32_t frameNumber) {};

            const VkPhysicalDevice physicalDevice() const;
            const VkDevice& device() const;
            const VkSurfaceKHR& surface() const;
            const std::vector<VulkanQueue>& deviceQueues() const;
            std::shared_ptr<CommandAllocatorImpl> allocator() const;

            Texture createTexture(const Device& device, Queue& queue, const TextureDescription& desc);
            void uploadBuffer(CommandList& commandList, BufferSRV& buffer, const tools::ByteRange& data, uint32_t startElement = 0);
            void uploadBuffer(CommandListImpl& commandList, BufferSRV& buffer, const tools::ByteRange& data, uint32_t startElement = 0);
            void uploadBuffer(CommandList& commandList, BufferCBV& buffer, const tools::ByteRange& data, uint32_t startElement = 0);
            void uploadBuffer(CommandListImpl& commandList, BufferCBV& buffer, const tools::ByteRange& data, uint32_t startElement = 0);
            void uploadBuffer(CommandList& commandList, BufferIBV& buffer, const tools::ByteRange& data, uint32_t startElement = 0);
            void uploadBuffer(CommandListImpl& commandList, BufferIBV& buffer, const tools::ByteRange& data, uint32_t startElement = 0);
            void uploadBuffer(CommandList& commandList, BufferVBV& buffer, const tools::ByteRange& data, uint32_t startElement = 0);
            void uploadBuffer(CommandListImpl& commandList, BufferVBV& buffer, const tools::ByteRange& data, uint32_t startElement = 0);

            const platform::Window& window() const;
            int width() const;
            int height() const;

            void createRTV(Buffer& buffer, DescriptorHandle& rtvHandle);
            QueueInfo createQueue(CommandListType type) const;
            void destroyQueue(QueueInfo queue) const;

            std::shared_ptr<CommandAllocatorImpl> createCommandAllocator();
            void freeCommandAllocator(std::shared_ptr<CommandAllocatorImpl> allocator);

            void waitForIdle();
        private:
            static VulkanInstance m_instance;
            VkPhysicalDevice m_physicalDevice;
            std::shared_ptr<VkDevice> m_device;
            std::shared_ptr<VkSurfaceKHR> m_surface;
            std::shared_ptr<platform::Window> m_window;
            const std::vector<const char*> m_requiredExtensions;
            std::shared_ptr<CommandAllocatorImpl> m_allocator;

            std::shared_ptr<Buffer> m_uploadBuffer;
            std::shared_ptr<NullResources> m_nullResources;

            bool isSuitableDevice(const VkPhysicalDevice& device) const;
            bool checkExtensionSupport(const VkPhysicalDevice& device) const;
            std::vector<VulkanQueue> enumerateQueues(const VkPhysicalDevice& device) const;
            void createLogicalDevice();
            void createSurface();

            //do some kind of CommandAllocator list that DX12CommandList constructor can ask for
            std::queue<std::shared_ptr<CommandAllocatorImpl>> m_commandAllocators;
            std::queue<std::vector<std::shared_ptr<CommandAllocatorImpl>>> m_inFlightCommandAllocators; // these are not referenced by command buffers but can still be in flight

            std::vector<std::shared_ptr<CommandAllocatorImpl>> m_returnedCommandAllocators;
            std::vector<std::shared_ptr<CommandAllocatorImpl>> m_inUseCommandAllocators; // these are referenced by command buffers

        };
    }
}
