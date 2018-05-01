#pragma once

#include "zmq.hpp"
#include "ImageTask.h"
#include "ModelTask.h"

#include <vector>
#include <string>
#include <memory>
#include <mutex>

class ProcessorTaskImageRequest;
class ProcessorTaskImageResponse;

namespace resource_task
{
    class ResourceTask
    {
    public:
        ResourceTask(
            const std::string& ip, 
            int port, 
            const std::string& id,
            const std::string& hostId);
        int join();

        zmq::socket_t& socket()
        {
            return m_socket;
        }
        const std::string& hostId() const
        {
            return m_hostid;
        }
    private:
        std::string m_id;
        std::string m_hostid;
        zmq::context_t m_context;
        zmq::socket_t m_socket;
        std::vector<zmq::pollitem_t> m_polls;

        ImageTask m_imageTask;
        ModelTask m_modelTask;

        enum class TaskState
        {
            WaitingForHelo,
            PerformingTask
        };
        TaskState m_state;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_lastCheck;
    };
}
