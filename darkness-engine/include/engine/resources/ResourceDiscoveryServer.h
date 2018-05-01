#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace platform
{
    class Socket;
    class SocketClient;
    class SocketServer;
    struct NetworkMessage;
}

namespace engine
{
    enum class DiscoveryMessage;

    using OnNetworkResourceDiscovery = std::function<void(const std::string&)>;

    class ResourceDiscoveryServer
    {
    public:
        ResourceDiscoveryServer(OnNetworkResourceDiscovery onDiscovery);
    private:
        OnNetworkResourceDiscovery m_onDiscovery;
        std::shared_ptr<platform::SocketClient> m_client;
        std::shared_ptr<platform::SocketServer> m_server;
        void onClientMessage(std::shared_ptr<platform::Socket> socket, const platform::NetworkMessage& message);
        void onServerMessage(std::shared_ptr<platform::Socket> socket, const platform::NetworkMessage& message);
        std::vector<std::string> m_discoveredClients;
    };
}
