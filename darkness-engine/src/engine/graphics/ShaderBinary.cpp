#include "engine/graphics/ShaderBinary.h"

#if defined(_WIN32) && !defined(VULKAN)
#include "engine/graphics/dx12/DX12ShaderBinary.h"
#endif

#if defined(VULKAN)
#include "engine/graphics/vulkan/VulkanShaderBinary.h"
#endif

#ifdef __APPLE__
#include "engine/graphics/metal/MetalShaderBinary.h"
#endif

using namespace tools;
using namespace engine::implementation;

namespace engine
{
    PIMPL_ACCESS_IMPLEMENTATION(ShaderBinary, ShaderBinaryImpl)

    ShaderBinary::ShaderBinary(
        const Device& device, 
        const std::string& binaryPath, 
        const std::string& supportPath,
        int permutationId,
        const std::vector<std::string>& defines,
        platform::FileWatcher& watcher)
        : m_impl{ tools::make_unique_impl<ShaderBinaryImpl>(device, binaryPath, supportPath, permutationId, defines, watcher) }
    {
    }

    void ShaderBinary::registerForChange(void* client, std::function<void(void)> change) const
    {
        m_impl->registerForChange(client, change);
    }

    void ShaderBinary::unregisterForChange(void* client) const
    {
        m_impl->unregisterForChange(client);
    }
}
