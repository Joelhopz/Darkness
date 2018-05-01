#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <mutex>
#include <thread>
#include <chrono>

#include "protocols/network/BeaconProtocols.pb.h"

namespace platform
{
    class Socket;
    class SocketClient;
    class SocketServer;
    struct NetworkMessage;
}

namespace engine
{
    using OnNetworkResourceDiscovery = std::function<void(const BeaconHello&)>;
    using OnNetworkResourceLoss = std::function<void(const BeaconHello&)>;

    enum class BeaconType
    {
        Server,
        Client
    };

    class BeaconHelloContainer : public BeaconHello
    {
    public:
        BeaconHelloContainer(const BeaconHello& hello)
            : BeaconHello{ hello }
            , lastSeen{ std::chrono::high_resolution_clock::now() }
        {}
        std::chrono::time_point<std::chrono::high_resolution_clock> lastSeen;
    };

    class Beacon
    {
    public:
        Beacon(
            BeaconType type, 
            const std::string& identity,
            OnNetworkResourceDiscovery onDiscovery,
            OnNetworkResourceLoss onLoss,
            int keepAliveNotifyMilliSeconds = 2000,
            int keepAliveMilliSeconds = 10000);
        ~Beacon();
    private:
        BeaconType m_type;
        std::string m_identity;
        OnNetworkResourceDiscovery m_onDiscovery;
        OnNetworkResourceLoss m_onLoss;
        int m_keepAliveNotifyMilliSeconds;
        int m_keepAliveMilliSeconds;
        std::string m_localIp;
        std::shared_ptr<platform::SocketClient> m_client;
        std::shared_ptr<platform::SocketServer> m_server;
        void onClientMessage(std::shared_ptr<platform::Socket> socket, const platform::NetworkMessage& message);
        void onServerMessage(std::shared_ptr<platform::Socket> socket, const platform::NetworkMessage& message);
        std::vector<std::string> m_discoveredClients;

        std::vector<BeaconHelloContainer> m_knownServers;
        std::vector<BeaconHelloContainer> m_knownClients;
        std::vector<BeaconHelloContainer> m_onHoldLeaving;

        bool isMe(const BeaconHello& hello);
        bool knownClient(const BeaconHello& hello);

        std::mutex m_mutex;
        std::unique_ptr<std::thread, std::function<void(std::thread*)>> m_thread;
        volatile bool m_alive;
        void onKeepalive();
    };
}
