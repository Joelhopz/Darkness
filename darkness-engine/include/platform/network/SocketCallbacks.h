#pragma once

#include <functional>
#include <memory>

namespace platform
{
    class Socket;
    using OnConnect = std::function<void(std::shared_ptr<Socket>)>;
    using OnDisconnect = std::function<void(std::shared_ptr<Socket>)>;
    using OnData = std::function<void(std::shared_ptr<Socket>, char*, size_t)>;
}
