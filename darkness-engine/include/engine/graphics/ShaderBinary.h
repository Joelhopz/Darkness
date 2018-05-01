#pragma once

#include "tools/SmartPimpl.h"
#include "platform/FileWatcher.h"
#include <string>

namespace engine
{
    PIMPL_FWD(ShaderBinary, ShaderBinaryImpl)
    class Device;

    class ShaderBinary
    {
    public:
        ShaderBinary(
            const Device& device, 
            const std::string& binaryPath, 
            const std::string& supportPath,
            int permutationId,
            const std::vector<std::string>& defines,
            platform::FileWatcher& watcher);

        void registerForChange(void* client, std::function<void(void)> change) const;
        void unregisterForChange(void* client) const;

        PIMPL_FRIEND_ACCESS(ShaderBinaryImpl)
    };
}
