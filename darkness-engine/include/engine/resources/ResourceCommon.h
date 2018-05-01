#pragma once

#include <string>
#include <vector>
#include <functional>

namespace engine
{
    enum class ResourceType
    {
        Image,
        Model
    };

    struct ProcessResourceItem
    {
        ResourceType type;
        unsigned int encodingFormat;
        bool generateMips;
        std::string absoluteSourceFilepath;
        std::string absoluteContentFilepath;
        std::string absoluteProcessedFilepath;
    };

    using ResourceProcessingStarted = std::function<void(ProcessResourceItem)>;
    using ResourceProcessingFinished = std::function<void(ProcessResourceItem)>;
    using ResourceProcessingProgress = std::function<void(ProcessResourceItem, float)>;
    using ResourceProcessingProgressMessage = std::function<void(ProcessResourceItem, float, const std::string&)>;

    struct ProcessResourcePackage
    {
        std::vector<ProcessResourceItem> images;
        std::vector<ProcessResourceItem> models;

        ResourceProcessingStarted onStarted;
        ResourceProcessingFinished onFinished;
        ResourceProcessingProgress onProgress;
        ResourceProcessingProgressMessage onProgressMessage;
    };
}
