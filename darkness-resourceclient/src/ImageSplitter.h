#pragma once

#include <vector>
#include "ResClientCommon.h"

namespace resource_client
{
    class ImageSplitter
    {
    public:
        ImageSplitter();
        ~ImageSplitter();
        std::vector<SplitTask> splitImageTask(Task& container);
        TaskResult joinImageTask(std::vector<SplitTaskResult>& results);
    };
}
