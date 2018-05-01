#include "engine/graphics/vulkan/VulkanCommon.h"
#include "engine/graphics/vulkan/VulkanCommandList.h"
#include "engine/graphics/CommandList.h"
#include "engine/graphics/Common.h"

namespace engine
{
    GpuMarker::GpuMarker(CommandList& cmd, char const* formatString, ...)
        : ptr{ implementation::CommandListImplGet::impl(cmd).native() }
    {
        std::vector<char> message(32768);
        memset(message.data(), 0, message.size());
        va_list arguments;
        va_start(arguments, formatString);
        vsprintf_s(message.data(), 32768, formatString, arguments);
        va_end(arguments);

        //PIXBeginEvent(reinterpret_cast<ID3D12GraphicsCommandList*>(ptr), 0, message.data());
    }

    GpuMarker::~GpuMarker()
    {
        //PIXEndEvent(reinterpret_cast<ID3D12GraphicsCommandList*>(ptr));
    }

    CpuMarker::CpuMarker(char const* formatString, ...)
    {
        std::vector<char> message(32768);
        memset(message.data(), 0, message.size());
        va_list arguments;
        va_start(arguments, formatString);
        vsprintf_s(message.data(), 32768, formatString, arguments);
        va_end(arguments);

        //PIXBeginEvent(0, message.data());
    }

    CpuMarker::~CpuMarker()
    {
        //PIXEndEvent();
    }
}
