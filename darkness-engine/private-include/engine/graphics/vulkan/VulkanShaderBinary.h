#pragma once

#include "engine/graphics/vulkan/VulkanHeaders.h"
#include "engine/graphics/ShaderSupport.h"
#include "platform/FileWatcher.h"
#include <memory>
#include <string>
#include <vector>
#include <map>

namespace engine
{
    class Device;

    namespace implementation
    {
        class ShaderBinaryImpl
        {
        public:
            ShaderBinaryImpl(
                const Device& device, 
                const std::string& binaryPath,
                const std::string& supportPath,
                platform::FileWatcher& watcher);

            void registerForChange(void* client, std::function<void(void)> change) const;
            void unregisterForChange(void* client) const;

            const VkShaderModule& native() const;
            const std::vector<uint32_t> data() const;
        private:
            const Device* m_device;
            std::shared_ptr<VkShaderModule> m_shaderBinary = nullptr;
            std::vector<uint32_t> m_data;
            
            ShaderSupport m_shaderSupport;
            platform::WatchHandle m_watchHandle;
            mutable std::map<void*, std::function<void(void)>> m_change;

            std::string onFileChange(const std::string& path);

            void readFile(const std::string& path);
            void createShaderModule(const Device& device, const std::vector<uint32_t>& data);
        };
    }
}

