#include "engine/graphics/vulkan/VulkanShaderBinary.h"
#include "engine/graphics/vulkan/VulkanHeaders.h"
#include "engine/graphics/vulkan/VulkanDevice.h"
#include "engine/graphics/Device.h"
#include "tools/Recompile.h"
#include "tools/Debug.h"

#include <iostream>
#include <fstream>

using namespace std;

namespace engine
{
    namespace implementation
    {
        ShaderBinaryImpl::ShaderBinaryImpl(
            const Device& device,
            const std::string& binaryPath,
            const std::string& supportPath,
            platform::FileWatcher& watcher)
            : m_device{ &device }
            , m_shaderSupport{ supportPath }
            , m_watchHandle{ watcher.addWatch(m_shaderSupport.file, [this](const std::string& changedPath)->std::string { return this->onFileChange(changedPath); }) }
            , m_change{}
        {
            readFile(binaryPath);
            createShaderModule(device, m_data);
        }

        std::string ShaderBinaryImpl::onFileChange(const std::string& /*path*/)
        {
            auto res = recompile(m_shaderSupport);
            readFile(m_shaderSupport.binaryFile);
            createShaderModule(*m_device, m_data);
            for (auto&& change : m_change)
            {
                change.second();
            }
            return res;
        }

        void ShaderBinaryImpl::registerForChange(void* client, std::function<void(void)> change) const
        {
            m_change[client] = change;
        }

        void ShaderBinaryImpl::unregisterForChange(void* client) const
        {
            m_change.erase(client);
        }

        void ShaderBinaryImpl::readFile(const std::string& path)
        {
            const char* temp = path.data();
            ASSERT(temp);
            ifstream shaderFile;
            shaderFile.open(path, ios::in | ios::binary);
            if (shaderFile.is_open())
            {
                auto begin = shaderFile.tellg();
                shaderFile.seekg(0, ios::end);
                auto end = shaderFile.tellg();
                size_t size = static_cast<size_t>(end - begin);

                m_data.resize(size / 4);
                shaderFile.seekg(0, ios::beg);
                shaderFile.read(reinterpret_cast<char*>(m_data.data()), end - begin);
                shaderFile.close();
            }
        }

        const std::vector<uint32_t> ShaderBinaryImpl::data() const
        {
            return m_data;
        }

        void ShaderBinaryImpl::createShaderModule(const Device& device, const std::vector<uint32_t>& data)
        {
            VkShaderModuleCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
            createInfo.codeSize = data.size() * 4;
            createInfo.pCode = reinterpret_cast<const uint32_t*>(data.data());

            m_shaderBinary.reset(new VkShaderModule(), [&](VkShaderModule* module) 
            {
                vkDestroyShaderModule(DeviceImplGet::impl(device).device(), *module, nullptr);
                delete module;
            });
            auto result = vkCreateShaderModule(DeviceImplGet::impl(device).device(), &createInfo, nullptr, m_shaderBinary.get());
            ASSERT(result == VK_SUCCESS);
        }

        const VkShaderModule& ShaderBinaryImpl::native() const
        {
            return *m_shaderBinary;
        }
    }
}
