#pragma once

#include <functional>
#include <memory>
#include <vector>

namespace platform
{
    class Socket;

    struct NetworkMessage
    {
        NetworkMessage()
            : buffer{ nullptr }
            , bytes{ 0 }
        {}

        template<typename T>
        NetworkMessage(const std::vector<T>& data)
            : buffer{ &data[0] }
            , bytes{ data.size() }
        {}

        const char* buffer;
        size_t bytes;
    };

    using OnNetworkMessage = std::function<void(std::shared_ptr<platform::Socket> socket, const NetworkMessage&)>;

    class PrefixLengthMessageParser
    {
    public:
        PrefixLengthMessageParser(OnNetworkMessage onMessage);
        void sendMessage(std::shared_ptr<platform::Socket> socket, const NetworkMessage& message);

        void setSocket(std::shared_ptr<platform::Socket> socket);
        void onData(std::shared_ptr<platform::Socket> socket, const char* buffer, size_t bytes);
    private:
        enum class ParserState
        {
            GetPrefix,
            GetMessage
        };
        std::shared_ptr<platform::Socket> m_socket;
        OnNetworkMessage m_onMessage;
        ParserState m_parserState;
        uint32_t m_messagePrefix;
        uint32_t m_messagePrefixBytes;
        std::vector<char> m_messageData;
        uint32_t m_messageDataBytes;
    };
}
