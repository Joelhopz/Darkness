#pragma once

#include "zmq.hpp"
#include "ResClientCommon.h"
#include <queue>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>

#include "protocols/network/ResourceProtocols.pb.h"

class BeaconHello;

namespace engine
{
    class Beacon;
    class Process;
}

namespace resource_client
{
    class ResourceClient
    {
    public:
        ResourceClient(const std::string& frontIdentity = "");
        bool runService();

        bool hasTasks();
        Task getTask();
        bool hasWorkers();
        void startWorker();
        Worker getWorker();
        void disposeWorker(Worker&& worker);
        void processTask(SplitTask& task);
        void finishTask(
            TaskResult&& task,
            const std::string& hostTaskId);

        void sendTaskStart(
            const std::string& hostId,
            const std::string& hostTaskId);

        void sendTaskFinished(
            const std::string& hostId,
            const std::string& hostTaskId);

        void updateTaskProgress(
            const std::string& hostId,
            const std::string& hostTaskId,
            float progress);

        void updateTaskProgressMessage(
            const std::string& hostId,
            const std::string& hostTaskId,
            float progress,
            const std::string& message);

        bool shouldShutdown() const;
    private:
        bool m_headless;
        std::string m_frontIdentity;
        std::string m_backIdentity;
        std::shared_ptr<engine::Beacon> m_beacon;
        void onDiscovery(const BeaconHello& hello);
        void onLoss(const BeaconHello& hello);
        zmq::context_t m_context;
        zmq::socket_t m_front;
        zmq::socket_t m_back;
        std::vector<zmq::pollitem_t> m_polls;
        std::string m_localIp;
        
        std::vector<Worker> m_workers;
        std::vector<Task> m_tasks;

        std::vector<SplitTask*> m_splitTasks;

        bool m_shouldShutdown;
    };
}
