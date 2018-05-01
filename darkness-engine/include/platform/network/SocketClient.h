#pragma once

#include "SocketCallbacks.h"
#include "PrefixLengthMessageParser.h"
#include <mutex>
#include <vector>

namespace platform
{
    class SocketClient
    {
    public:
        SocketClient(
            const std::string& ip,
            int port,
            bool blocking,
            bool broadcast,
            OnConnect onConnect,
            OnDisconnect onDisconnect,
            OnNetworkMessage onData);
        void sendMessage(std::shared_ptr<platform::Socket> socket, const NetworkMessage& message);
        //void write(const char* buffer, size_t bytes);

        std::shared_ptr<Socket>& socket();
    private:
        int m_port;
        OnConnect           m_onConnect;
        OnDisconnect        m_onDisconnect;
        OnNetworkMessage    m_onData;

        std::shared_ptr<PrefixLengthMessageParser> m_parser;
        std::shared_ptr<Socket> m_clientSocket;

        std::unique_ptr<std::thread, std::function<void(std::thread*)>> m_work;
        std::mutex m_mutex;
        bool m_alive;
        void run(std::mutex& mutex);
        std::vector<std::shared_ptr<Socket>> m_clients;
        std::unique_ptr<char, std::function<void(char*)>> m_closingDownMessage;
    };
}
