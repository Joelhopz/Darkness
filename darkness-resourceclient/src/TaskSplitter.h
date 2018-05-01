#pragma once

#include <vector>
#include "ImageSplitter.h"
#include "ModelSplitter.h"

namespace resource_client
{
    struct Task;
    struct SplitTask;
    struct SplitTaskResult;
    struct TaskResult;

    constexpr int MaximumTaskSize = 256;

    class TaskSplitter
    {
    public:
        std::vector<SplitTask> taskSplitter(Task& container);
        TaskResult taskJoiner(std::vector<SplitTaskResult>& results);
    private:
        ImageSplitter m_imageSplitter;
        ModelSplitter m_modelSplitter;
    };
    
}
