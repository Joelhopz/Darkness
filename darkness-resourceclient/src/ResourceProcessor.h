#pragma once

#include <thread>
#include <mutex>
#include <memory>
#include <queue>

#include "TaskSplitter.h"
#include "ZmqResourceClient.h"

namespace resource_client
{
    class ResourceProcessor
    {
    public:
        ResourceProcessor(const std::string& frontIdentity = "");
        ~ResourceProcessor();
        void run(bool async = false);
    private:
        std::string m_frontIdentity;
        TaskSplitter m_splitter;
        volatile bool m_alive;
        std::mutex m_workerMutex;
        std::thread m_worker;
        std::unique_ptr<ResourceClient> m_client;
        int32_t m_currentTaskCount;
        int32_t m_coreCount;

        int maxActiveTasks() const;

        void processTasks();
        void runClient();
        
        std::mutex m_taskMutex;
        volatile bool m_taskAlive;
        std::unique_ptr<std::thread, std::function<void(std::thread*)>> m_taskThread;

        struct TaskContainer
        {
            Task task;
            std::vector<SplitTask> splits;
            std::vector<SplitTaskResult> splitResults;
            TaskResult result;
            float progress;
            uint64_t sourceSizeBytes;
            TaskContainer(Task _task)
                : task{ _task }
                , splits{}
                , splitResults{}
                , result{}
                , progress{ 0.0f }
                , sourceSizeBytes{ 0u }
            {}
        };
        std::queue <std::shared_ptr<TaskContainer>> m_tasksForSplitting;
        std::vector<std::shared_ptr<TaskContainer>> m_tasksForProcessing;
        std::queue <std::shared_ptr<TaskContainer>> m_tasksForJoining;
        std::vector<std::shared_ptr<TaskContainer>> m_tasksDone;
    };
}