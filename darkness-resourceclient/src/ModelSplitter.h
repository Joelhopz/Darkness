#pragma once

#include <vector>
#include "ResClientCommon.h"

namespace resource_client
{
    class ModelSplitter
    {
    public:
        std::vector<SplitTask> splitModelTask(Task& container);
        TaskResult joinModelTask(std::vector<SplitTaskResult>& results);
    };
}
