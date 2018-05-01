#pragma once

#include <string>
#include "zmq.hpp"

namespace engine
{
    struct EncodingTask;
}

namespace resource_task_back
{
    class ResourceTask
    {
    public:
        ResourceTask(const std::string& ip, int port, uint32_t magicNumber);
        int join();
    private:
        zmq::context_t m_context;
        zmq::socket_t m_socket;
        bool m_taskDone;

        void performTask(const engine::EncodingTask& task);
    };
}
