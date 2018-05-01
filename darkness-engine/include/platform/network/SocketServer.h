#pragma once

#include "SocketCallbacks.h"
#include "PrefixLengthMessageParser.h"

#include <thread>
#include <mutex>
#include <vector>

namespace platform
{
    class SocketServer
    {
    public:
        SocketServer(
            int port, 
            bool blocking,
            bool broadcast,
            OnConnect onConnect, 
            OnDisconnect onDisconnect,
            OnNetworkMessage onData);

        void sendMessage(std::shared_ptr<platform::Socket> socket, const NetworkMessage& message);
        std::string ip() const;
    private:
        int m_port;
        bool m_broadcasting;
        OnConnect           m_onConnect;
        OnDisconnect        m_onDisconnect;
        OnNetworkMessage    m_onData;

        PrefixLengthMessageParser m_serverSocketParser;
        std::shared_ptr<Socket> m_serverSocket;
        std::unique_ptr<std::thread, std::function<void(std::thread*)>> m_work;
        std::mutex m_mutex;
        bool m_alive;
        void run(std::mutex& mutex);

        struct ClientContainer
        {
            std::shared_ptr<PrefixLengthMessageParser> parser;
            std::shared_ptr<Socket> socket;
        };

        std::vector<ClientContainer> m_clients;

        std::unique_ptr<char, std::function<void(char*)>> m_closingDownMessage;
    };
}
