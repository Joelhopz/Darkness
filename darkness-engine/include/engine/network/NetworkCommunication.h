#pragma once

#include "external/libzmq/include/zmq.hpp"
#include "engine/network/MqMessage.h"
#include "engine/network/Beacon.h"
#include <memory>
#include <vector>
#include <mutex>
#include <thread>
#include <string>
#include <queue>

class BeaconHello;
class HostTaskImageResponse;
class HostTaskModelResponse;

namespace engine
{
    class Beacon;

    using NetTaskNotifyStarted = std::function<void(const std::string&)>;
    using NetTaskNotifyFinished = std::function<void(const std::string&)>;
    using NetTaskNotifyProgress = std::function<void(const std::string&, float)>;
    using NetTaskNotifyProgressMessage = std::function<void(const std::string&, float, const std::string&)>;
    using NetTaskImageResult = std::function<void(HostTaskImageResponse)>;
    using NetTaskModelResult = std::function<void(HostTaskModelResponse)>;

    class NetworkCommunication
    {
    public:
        NetworkCommunication(
            OnNetworkResourceDiscovery found,
            OnNetworkResourceLoss lost,
            NetTaskNotifyStarted started,
            NetTaskNotifyFinished finished,
            NetTaskNotifyProgress progress,
            NetTaskNotifyProgressMessage progressMessage,
            NetTaskImageResult imageResult,
            NetTaskModelResult modelResult);
        ~NetworkCommunication();

        uint32_t cores(const BeaconHello& beacon);
        void send(MqMessage&& msg, const BeaconHello& beacon);

    private:
        std::mutex m_commMutex;
        enum class CommTaskType
        {
            ClientEntry,
            ClientLeave,
            Send
        };
        struct CommTask
        {
            CommTaskType type;
            BeaconHello beacon;
            MqMessage msg;
        };
        std::queue<CommTask> m_commTasks;

        std::string m_uuid;
        OnNetworkResourceDiscovery m_found;
        OnNetworkResourceLoss m_lost;
        NetTaskNotifyStarted m_started;
        NetTaskNotifyFinished m_finished;
        NetTaskNotifyProgress m_progress;
        NetTaskNotifyProgressMessage m_progressMessage;
        NetTaskImageResult m_imageResult;
        NetTaskModelResult m_modelResult;
        std::shared_ptr<Beacon> m_beacon;
        void onDiscovery(const BeaconHello& beacon);
        void onLoss(const BeaconHello& beacon);

        volatile bool m_alive;
        std::unique_ptr<zmq::context_t> m_context;
        std::unique_ptr<zmq::socket_t> m_router;
        std::vector<zmq::pollitem_t> m_polls;

        std::thread m_commThread;
        void commHeartbeat();

        struct ClientContainer
        {
            BeaconHello beacon;
            unsigned int cores;
            std::chrono::time_point<std::chrono::high_resolution_clock> lastSent;
        };
        std::vector<ClientContainer> m_clients;

        std::string addressFromIp(const std::string& ip);
    };
}
