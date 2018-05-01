#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "platform/FileWatcher.h"
#include "engine/graphics/ShaderSupport.h"

struct D3D12_SHADER_BYTECODE;

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
                int permutationId,
                const std::vector<std::string>& defines,
                platform::FileWatcher& watcher);
            ~ShaderBinaryImpl();

            ShaderBinaryImpl(const ShaderBinaryImpl&);
            ShaderBinaryImpl(ShaderBinaryImpl&&);
            ShaderBinaryImpl& operator=(const ShaderBinaryImpl&);
            ShaderBinaryImpl& operator=(ShaderBinaryImpl&&);

            void registerForChange(void* client, std::function<void(void)> change) const;
            void unregisterForChange(void* client) const;

            D3D12_SHADER_BYTECODE& native() const;

            // for debug
            const std::string& supportPath() const { return m_shaderSupport.file; }
        private:
            std::unique_ptr<char[]> m_buffer;
            D3D12_SHADER_BYTECODE* m_shaderBinary;
            int m_permutationId;
            std::vector<std::string> m_defines;
            
            ShaderSupport m_shaderSupport;
            platform::WatchHandle m_watchHandle;
            mutable std::unordered_map<void*, std::function<void(void)>> m_change;

            std::string onFileChange(const std::string& path);
            void readFile(const std::string& path);
            void clear();
        };
    }
}

