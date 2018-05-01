#pragma once

#include "engine/graphics/Format.h"
#include <vector>

namespace engine
{
    enum class ResourceClientMessage
    {
        GetCores,
        CoresAnswer,
        TaskConfirm,
        ShutdownResourceClient,
        TaskHelo,
        ImageEncodingTask,
        ImageEncodingStart,
        ImageEncodingProgress,
        ImageEncodingDone,
        TestHelo,
        TestEhlo,
        TaskResultsReceived
    };

    enum class DiscoveryMessage
    {
        Helo,
        Ehlo
    };

    struct EncodingTask
    {
        uint64_t    taskId;
        uint32_t    sourceWidth;
        uint32_t    sourceHeight;
        size_t      sourceSizeBytes;
        int         sourceFormat;
        uint32_t    sourceStride;
        size_t      destinationSizeBytes;
        int         destinationFormat;
        Format      encodingFormat;

        std::vector<char> sourceBuffer;
    };

    std::vector<char> encodeTask(const EncodingTask& task);
    EncodingTask decodeTask(const std::vector<char>& data);

    struct EncodingTaskResult
    {
        std::vector<char> buffer;
        uint64_t taskId;
    };

    std::vector<char> encodeTaskResult(const EncodingTaskResult& task);
    EncodingTaskResult decodeTaskResult(const std::vector<char>& data);
}
