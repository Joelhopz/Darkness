#include "engine/graphics/vulkan/VulkanDevice.h"
#include "engine/graphics/vulkan/VulkanHeaders.h"
#include "engine/graphics/vulkan/VulkanResources.h"
#include "engine/graphics/vulkan/VulkanSwapChain.h"
#include "engine/graphics/vulkan/VulkanDescriptorHandle.h"
#include "engine/graphics/vulkan/VulkanCommandAllocator.h"

#include "engine/graphics/Resources.h"
#include "engine/graphics/Device.h"
#include "engine/graphics/CommandList.h"
#ifdef _WIN32
#include "platform/window/WindowsWindow.h"
#endif
#include "tools/Debug.h"
#include <vector>
#include <algorithm>
#include <set>
#include "tools/image/Image.h"

using namespace platform::implementation;
using namespace engine::implementation::device_implementation;
using namespace std;
using namespace tools;

namespace engine
{
    namespace implementation
    {
        void createWindowSurface(
            VkInstance instance,
            const VkWin32SurfaceCreateInfoKHR& createInfo,
            VkSurfaceKHR* surface)
        {
            auto createWin32SurfaceKHR = 
                reinterpret_cast<PFN_vkCreateWin32SurfaceKHR>(
                reinterpret_cast<void*>(vkGetInstanceProcAddr(instance, "vkCreateWin32SurfaceKHR")));

            ASSERT(createWin32SurfaceKHR != nullptr);
            ASSERT(createWin32SurfaceKHR(instance, &createInfo, nullptr, surface) == VK_SUCCESS);
        }

        VulkanInstance DeviceImpl::m_instance;

        DeviceImpl::DeviceImpl(std::shared_ptr<platform::Window> window)
            : m_physicalDevice{ VK_NULL_HANDLE }
            , m_device{ vulkanPtr<VkDevice>( vkDestroyDevice) }
            , m_surface{ vulkanPtr<VkSurfaceKHR>(m_instance.instance(), vkDestroySurfaceKHR ) }
            , m_window{ window }
            , m_requiredExtensions{ VK_KHR_SWAPCHAIN_EXTENSION_NAME } //, VK_KHR_DISPLAY_EXTENSION_NAME, VK_EXT_DEBUG_MARKER_EXTENSION_NAME }
            , m_allocator{ nullptr }
        {
            createSurface();

            uint32_t deviceCount{ 0 };
            vkEnumeratePhysicalDevices(m_instance.instance(), &deviceCount, nullptr);
            ASSERT(deviceCount > 0);

            vector<VkPhysicalDevice> devices(deviceCount);
            vkEnumeratePhysicalDevices(m_instance.instance(), &deviceCount, devices.data());
            for (auto& device : devices)
            {
                if (isSuitableDevice(device))
                {
                    m_physicalDevice = device;
                    break;
                }
            }

            ASSERT(m_physicalDevice != VK_NULL_HANDLE);

            createLogicalDevice();

            m_allocator = std::make_shared<CommandAllocatorImpl>(*this);
        }

        std::shared_ptr<CommandAllocatorImpl> DeviceImpl::allocator() const
        {
            return m_allocator;
        }

        void DeviceImpl::uploadBuffer(CommandList& commandList, BufferSRV& buffer, const ByteRange& data, uint32_t startElement)
        {
            uploadBuffer(CommandListImplGet::impl(commandList), buffer, data, startElement);
        }

        void DeviceImpl::uploadBuffer(CommandListImpl& commandList, BufferSRV& buffer, const ByteRange& data, uint32_t startElement)
        {
            auto allocator = &m_allocator;
            /*std::shared_ptr<void> uploadData = std::shared_ptr<void>(
                m_allocator.allocate(data.size(), D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT),
                [allocator](void* ptr) { allocator->free(ptr); });
            m_currentUploadBufferList.emplace_back(uploadData);
            memcpy(uploadData.get(), data.start, data.size());

            commandList.native()->CopyBufferRegion(
                BufferImplGet::impl(BufferSRVImplGet::impl(buffer)->buffer())->native(),
                startElement * BufferSRVImplGet::impl(buffer)->description().elementSize,
                BufferImplGet::impl(*m_uploadBuffer)->native(),
                m_allocator.offset(uploadData.get()), data.size());*/
        }

        void DeviceImpl::uploadBuffer(CommandList& commandList, BufferCBV& buffer, const ByteRange& data, uint32_t startElement)
        {
            uploadBuffer(CommandListImplGet::impl(commandList), buffer, data, startElement);
        }

        void DeviceImpl::uploadBuffer(CommandListImpl& commandList, BufferCBV& buffer, const ByteRange& data, uint32_t startElement)
        {
            auto allocator = &m_allocator;
            /*std::shared_ptr<void> uploadData = std::shared_ptr<void>(
                m_allocator.allocate(data.size(), D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT),
                [allocator](void* ptr) { allocator->free(ptr); });
            m_currentUploadBufferList.emplace_back(uploadData);
            memcpy(uploadData.get(), data.start, data.size());

            commandList.native()->CopyBufferRegion(
                BufferImplGet::impl(BufferCBVImplGet::impl(buffer)->buffer())->native(),
                startElement * BufferCBVImplGet::impl(buffer)->description().elementSize,
                BufferImplGet::impl(*m_uploadBuffer)->native(),
                m_allocator.offset(uploadData.get()), data.size());*/
            LOG_INFO("TODO: vulkan DeviceImpl upload buffer");
        }

        void DeviceImpl::uploadBuffer(CommandList& commandList, BufferIBV& buffer, const ByteRange& data, uint32_t startElement)
        {
            uploadBuffer(CommandListImplGet::impl(commandList), buffer, data, startElement);
        }

        void DeviceImpl::uploadBuffer(CommandListImpl& commandList, BufferIBV& buffer, const ByteRange& data, uint32_t startElement)
        {
            auto allocator = &m_allocator;
            /*std::shared_ptr<void> uploadData = std::shared_ptr<void>(
                m_allocator.allocate(data.size(), D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT),
                [allocator](void* ptr) { allocator->free(ptr); });
            m_currentUploadBufferList.emplace_back(uploadData);
            memcpy(uploadData.get(), data.start, data.size());

            commandList.native()->CopyBufferRegion(
                BufferImplGet::impl(BufferIBVImplGet::impl(buffer)->buffer())->native(),
                startElement * BufferIBVImplGet::impl(buffer)->description().elementSize,
                BufferImplGet::impl(*m_uploadBuffer)->native(),
                m_allocator.offset(uploadData.get()), data.size());*/
            LOG_INFO("TODO: vulkan DeviceImpl upload buffer");
        }

        void DeviceImpl::uploadBuffer(CommandList& commandList, BufferVBV& buffer, const ByteRange& data, uint32_t startElement)
        {
            uploadBuffer(CommandListImplGet::impl(commandList), buffer, data, startElement);
        }

        void DeviceImpl::uploadBuffer(CommandListImpl& commandList, BufferVBV& buffer, const ByteRange& data, uint32_t startElement)
        {
            auto allocator = &m_allocator;
            /*std::shared_ptr<void> uploadData = std::shared_ptr<void>(
                m_allocator.allocate(data.size(), D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT),
                [allocator](void* ptr) { allocator->free(ptr); });
            m_currentUploadBufferList.emplace_back(uploadData);
            memcpy(uploadData.get(), data.start, data.size());

            commandList.native()->CopyBufferRegion(
                BufferImplGet::impl(BufferVBVImplGet::impl(buffer)->buffer())->native(),
                startElement * BufferVBVImplGet::impl(buffer)->description().elementSize,
                BufferImplGet::impl(*m_uploadBuffer)->native(),
                m_allocator.offset(uploadData.get()), data.size());*/
            LOG_INFO("TODO: vulkan DeviceImpl upload buffer");
        }

        Texture DeviceImpl::createTexture(const Device& device, Queue& queue, const TextureDescription& desc)
        {
            if (desc.descriptor.usage == ResourceUsage::CpuToGpu)
            {
                auto gpuDesc = desc;
                auto gpuTexture = createTexture(device, queue, gpuDesc
                    .usage(ResourceUsage::GpuRead)
                    .setInitialData(TextureDescription::InitialData()));

                if (desc.initialData.data.size() > 0)
                {
                    unsigned int subResourceIndex = 0;
                    uint32_t dataIndex = 0;
                    for (int slice = 0; slice < static_cast<int>(desc.descriptor.arraySlices); ++slice)
                    {
                        int width = desc.descriptor.width;
                        int height = desc.descriptor.height;

                        for (int mip = 0; mip < static_cast<int>(desc.descriptor.mipLevels); ++mip)
                        {
                            auto info = surfaceInformation(desc.descriptor.format, width, height);

                            LOG_INFO("TODO: Implement Vulkan DeviceImpl createTexture");
                            // copy to CPU memory
                            /*D3D12_SUBRESOURCE_FOOTPRINT  srcFootprint;
                            srcFootprint.Depth = 1;
                            srcFootprint.Format = dxFormat(desc.descriptor.format);
                            srcFootprint.Width = width;
                            srcFootprint.Height = height;
                            srcFootprint.RowPitch = static_cast<UINT>(Align(
                                info.rowBytes,
                                static_cast<size_t>(D3D12_TEXTURE_DATA_PITCH_ALIGNMENT)));

                            auto allocator = &m_allocator;
                            std::shared_ptr<void> uploadData = std::shared_ptr<void>(
                                m_allocator.allocate(info.numBytes, D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT),
                                [allocator](void* ptr) { allocator->free(ptr); });

                            m_currentUploadBufferList.emplace_back(uploadData);

                            D3D12_PLACED_SUBRESOURCE_FOOTPRINT placedTexture2D = { 0 };
                            placedTexture2D.Offset = m_allocator.offset(uploadData.get());
                            placedTexture2D.Footprint = srcFootprint;

                            const uint8_t* srcData = &desc.initialData.data[dataIndex];
                            for (int i = 0; i < static_cast<int>(info.numRows); ++i)
                            {
                                UINT8 *pScan = static_cast<uint8_t*>(uploadData.get()) + (i * srcFootprint.RowPitch);
                                memcpy(pScan, srcData, info.rowBytes);
                                srcData += info.rowBytes;
                            }
                            dataIndex += info.numBytes;

                            // copy to GPU memory
                            D3D12_TEXTURE_COPY_LOCATION dst;
                            dst.pResource = TextureImplGet::impl(gpuTexture)->native();
                            dst.SubresourceIndex = subResourceIndex;
                            dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;

                            D3D12_TEXTURE_COPY_LOCATION srcLocation;
                            srcLocation.pResource = BufferImplGet::impl(*m_uploadBuffer)->native();
                            srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
                            srcLocation.PlacedFootprint = placedTexture2D;

                            D3D12_BOX srcBox;
                            srcBox.left = 0;
                            srcBox.top = 0;
                            srcBox.right = width;
                            srcBox.bottom = height;
                            srcBox.front = 0;
                            srcBox.back = 1;

                            CommandList texcmdb = CommandList(device);
                            CommandListImplGet::impl(texcmdb).native()->CopyTextureRegion(
                                &dst,
                                0, 0, 0,
                                &srcLocation,
                                &srcBox);
                            Fence texfence = Fence(device);
                            queue.submit(texcmdb, texfence);
                            texfence.blockUntilSignaled();*/

                            width /= 2;
                            height /= 2;
                            if (isBlockCompressedFormat(desc.descriptor.format))
                            {
                                if (width < 4)
                                    width = 4;
                                if (height < 4)
                                    height = 4;
                            }
                            else
                            {
                                if (width < 1)
                                    width = 1;
                                if (height < 1)
                                    height = 1;
                            }
                            ++subResourceIndex;
                        }
                    }
                }
                return gpuTexture;
            }
            else
                return Texture{ std::make_shared<TextureImpl>(*this, desc) };
        }

        std::shared_ptr<CommandAllocatorImpl> DeviceImpl::createCommandAllocator()
        {
            if (m_commandAllocators.size() > 0)
            {
                auto res = m_commandAllocators.front();
                m_commandAllocators.pop();
                m_inUseCommandAllocators.emplace_back(res);
                return res;
            }

            m_inUseCommandAllocators.emplace_back(std::make_shared<CommandAllocatorImpl>(*this));
            return m_inUseCommandAllocators.back();
        }

        void DeviceImpl::nullResources(std::shared_ptr<NullResources> nullResources)
        {
            m_nullResources = nullResources;
        }

        NullResources& DeviceImpl::nullResources()
        {
            return *m_nullResources;
        }

        void DeviceImpl::freeCommandAllocator(std::shared_ptr<CommandAllocatorImpl> allocator)
        {
            m_inUseCommandAllocators.erase(std::find(m_inUseCommandAllocators.begin(), m_inUseCommandAllocators.end(), allocator));
            m_returnedCommandAllocators.emplace_back(allocator);
        }

        void DeviceImpl::createSurface()
        {
            VkWin32SurfaceCreateInfoKHR createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
            createInfo.hwnd = platform::implementation::WindowImplGet::impl(*m_window.get()).handle();
            createInfo.hinstance = GetModuleHandle(nullptr);
            createWindowSurface(m_instance.instance(), createInfo, m_surface.get());
        }

        bool DeviceImpl::isSuitableDevice(const VkPhysicalDevice& device) const
        {
            if (!checkExtensionSupport(device))
                return false;

            SwapChainImpl::SwapChainDetails swapChainDetails = SwapChainImpl::getDetails(device, *m_surface);
            if (swapChainDetails.formats.empty() || swapChainDetails.presentModes.empty())
                return false;

            auto deviceQueues = enumerateQueues(device);
            for (auto queue : deviceQueues)
            {
                if ((queue.flags & VK_QUEUE_GRAPHICS_BIT) && queue.presentSupport)
                    return true;
            }
            return false;
        }

        bool DeviceImpl::checkExtensionSupport(const VkPhysicalDevice& device) const
        {
            uint32_t extensionCount;
            vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

            vector<VkExtensionProperties> availableExtensions(extensionCount);
            vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

            std::set<std::string> requiredExtensionSet(m_requiredExtensions.begin(), m_requiredExtensions.end());

            for (const auto& extension : availableExtensions) {
                LOG("Device extension: %s", extension.extensionName);
                requiredExtensionSet.erase(extension.extensionName);
            }

            return requiredExtensionSet.empty();
        }

        const std::vector<VulkanQueue>& DeviceImpl::deviceQueues() const
        {
            return queues[this];
        }

        void DeviceImpl::createLogicalDevice()
        {
            queues[this] = enumerateQueues(m_physicalDevice);

            vector<VkDeviceQueueCreateInfo> createInfos;
            for (const auto& queue : queues[this])
            {
                VkDeviceQueueCreateInfo queueCreateInfo = {};
                queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                queueCreateInfo.queueFamilyIndex = static_cast<uint32_t>(queue.familyIndex);
                queueCreateInfo.queueCount = static_cast<uint32_t>(queue.available);
                queueCreateInfo.pQueuePriorities = queue.priority.data();
                createInfos.emplace_back(queueCreateInfo);
            }
            
            VkPhysicalDeviceFeatures deviceFeatures = {};

            VkDeviceCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            createInfo.pQueueCreateInfos = createInfos.data();
            createInfo.queueCreateInfoCount = static_cast<uint32_t>(createInfos.size());
            createInfo.pEnabledFeatures = &deviceFeatures;
            createInfo.enabledExtensionCount = static_cast<uint32_t>(m_requiredExtensions.size());
            createInfo.ppEnabledExtensionNames = m_requiredExtensions.data();
            createInfo.enabledLayerCount = 0;
#ifndef NDEBUG
            createInfo.enabledLayerCount = static_cast<uint32_t>(m_instance.validationLayers().size());
            createInfo.ppEnabledLayerNames = m_instance.validationLayers().data();
#endif

            auto result = vkCreateDevice(m_physicalDevice, &createInfo, nullptr, m_device.get());
            ASSERT(result == VK_SUCCESS);
        }

        vector<VulkanQueue> DeviceImpl::enumerateQueues(const VkPhysicalDevice& device) const
        {
            uint32_t queueFamilyCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

            std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

            vector<VulkanQueue> result;
            for (int i = 0; i < static_cast<int>(queueFamilies.size()); ++i)
            {
                std::vector<float> priorities(queueFamilies[static_cast<size_t>(i)].queueCount);
                float decreasePriority = 1.0f / static_cast<float>(priorities.size());
                float currentPriority = 1.0f;
                for (auto& priority : priorities)
                {
                    priority = currentPriority;
                    currentPriority -= decreasePriority;
                }


                VkBool32 presentSupport = false;
                vkGetPhysicalDeviceSurfaceSupportKHR(device, static_cast<uint32_t>(i), *m_surface, &presentSupport);

                result.emplace_back(VulkanQueue{
                    i,
                    static_cast<int>(queueFamilies[static_cast<size_t>(i)].queueCount),
                    queueFamilies[static_cast<size_t>(i)].queueFlags,
                    presentSupport == VK_TRUE,
                    priorities,
                    vector<bool>(priorities.size(), false)
                    });
            }
            return result;
        }

        DeviceImpl::~DeviceImpl()
        {
            for (auto que = queues.begin(); que != queues.end(); ++que)
            {
                if ((*que).first == this)
                {
                    queues.erase(this);
                    break;
                }
            }
        }

        QueueInfo DeviceImpl::createQueue(CommandListType type) const
        {
            for (auto& queue : queues[this])
            {
                if((queue.flags & vulkanCommandListType(type)) &&
                    queue.available > 0)
                {
                    --queue.available;
                    for (int i = 0; i < static_cast<int>(queue.taken.size()); ++i)
                    {
                        if (!queue.taken[static_cast<size_t>(i)])
                        {
                            queue.taken[static_cast<size_t>(i)] = true;
                            return QueueInfo{ queue.familyIndex, i };
                        }
                    }
                    ASSERT(false);
                }
            }
            ASSERT(false);
            return QueueInfo{ InvalidFamilyIndex };
        }

        void DeviceImpl::destroyQueue(QueueInfo commandQueue) const
        {
            for (auto& queue : queues[this])
            {
                if (queue.familyIndex == commandQueue.queueFamilyIndex)
                {
                    ++queue.available;
                    queue.taken[static_cast<size_t>(commandQueue.queueIndex)] = false;
                    return;
                }
            }
            ASSERT(false);
        }

        const VkPhysicalDevice DeviceImpl::physicalDevice() const
        {
            return m_physicalDevice;
        }

        const VkDevice& DeviceImpl::device() const
        {
            return *m_device;
        }

        const VkSurfaceKHR& DeviceImpl::surface() const
        {
            return *m_surface;
        }

        const platform::Window& DeviceImpl::window() const
        {
            return *m_window;
        }

        int DeviceImpl::width() const
        {
            return WindowImplGet::impl(*m_window).width();
        }

        int DeviceImpl::height() const
        {
            return WindowImplGet::impl(*m_window).height();
        }

        void DeviceImpl::createRTV(Buffer& /*buffer*/, DescriptorHandle& /*rtvHandle*/)
        {
            // TODO
        }

        void DeviceImpl::waitForIdle()
        {
            vkDeviceWaitIdle(*m_device);
        }
        
        /*ByteAddressBuffer DeviceImpl::createByteAddressBuffer(int numElements, int sizeElement, const void* initialData, const char* name)
         {
         
         }
         
         StructuredBuffer DeviceImpl::createStructuredBuffer(int numElements, int sizeElement, const void* initialData, const char* name)
         {
         
         }
         
         TypedBuffer DeviceImpl::createTypedBuffer(int numElements, int sizeElement, const void* initialData, const char* name)
         {
         
         }*/
    }
}
