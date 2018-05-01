#pragma once

#include <string>
#include <vector>
#include <mutex>
#include <memory>

struct sockaddr_in;

namespace platform
{
#ifdef _WIN32
    typedef uintptr_t SocketHandle;
#else
    typedef int SocketHandle;
#endif

    std::string getSendErrorString(int errorCode);
    std::string getConnectErrorString(int errorCode);

    enum class SocketType
    {
        TCP,
        UDP
    };

    class SocketServer;
    class Socket
    {
    public:
        std::string ip() const;

        static std::string localHostname();
        static std::string localIp();

    private:
        friend class SocketServer;
        friend class SocketClient;
        friend class PrefixLengthMessageParser;

        uint32_t availableBytes() const;
        size_t read(char* buffer, size_t lengthBytes);
        size_t write(const char* buffer, size_t lengthBytes);

        static std::shared_ptr<Socket> createSocket(int port, const std::string& ip = "", SocketType type = SocketType::TCP);
        Socket(SocketHandle socket);
        SocketHandle m_socket;
        
        void enableSocketReuse();
        void enableNoDelay();
        void enableNonBlockingSocket();
        void enableBroadcasting();
        uint32_t dataAvailableInSocket();

        std::shared_ptr<sockaddr_in> m_sockAddr;
        int connect();
        int bind();

        /*void private_read();*/
        size_t private_write();

        std::mutex m_mutex;
        bool m_nonBlocking;
        struct DataBlock
        {
            std::vector<char> data;
            uint32_t currentPointer;
        };
        std::vector<DataBlock> m_data;
    };
}
