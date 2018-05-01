#pragma once

#include <mutex>
#include <vector>

namespace engine
{
    class Process;
    class ResourceDiscoveryServer;
}

namespace platform
{
    class Socket;
    class SocketServer;
    struct NetworkMessage;
}

namespace resource_client_old
{
    static const char* ResourceTaskPath1 = "C:\\work\\darkness\\darkness-resourcetask\\bin\\x64\\Debug DX12\\DarknessResourceTask.exe";
    static const char* ResourceTaskPath2 = "DarknessResourceTask.exe";
    static const char* ResourceTaskArgumentsStart = "--ip=";
    static const char* ResourceTaskArgumentsStop = " --port=2134 --magic=";
    static const char* ResourceTaskWorkingDir = "C:\\work\\darkness\\darkness-resourcetask\\bin\\x64\\Debug DX12";

    class ResourceClient
    {
    public:
        ResourceClient();
        void broadcastClientStartup();
        int runService();
    private:
        std::shared_ptr<platform::SocketServer> m_server;
        std::shared_ptr<platform::SocketServer> m_serverToHost;
        std::shared_ptr<engine::ResourceDiscoveryServer> m_discoveryServer;
        void onMessage(std::shared_ptr<platform::Socket> socket, const platform::NetworkMessage& message);
        void onMessageFromHost(std::shared_ptr<platform::Socket> socket, const platform::NetworkMessage& message);
        std::mutex m_mutex;
        bool m_alive;

        std::shared_ptr<platform::Socket> m_masterSocket;

        struct ResourceTaskContainer
        {
            std::shared_ptr<engine::Process> taskProcess;
            std::shared_ptr<platform::Socket> socket;
            uint32_t magicNumber;
            std::vector<char> encodingTaskData;
        };
        uint32_t m_magicNumber;
        std::string m_localIp;
        std::vector<ResourceTaskContainer> m_tasks;
    };
}
