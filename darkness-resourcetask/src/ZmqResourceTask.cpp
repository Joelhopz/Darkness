#include "ZmqResourceTask.h"
#include "zmsg.hpp"
#include "Compressonator/Header/Compressonator.h"
#include "engine/resources/ResourceMessages.h"

using namespace std;
using namespace zmq;

namespace resource_task_back
{
    ResourceTask::ResourceTask(const std::string& ip, int port, uint32_t magicNumber)
        : m_context{ 1 }
        , m_socket{ m_context, socket_type::req }
    {
        intptr_t id = static_cast<intptr_t>(magicNumber);
        std::string identity = s_set_id(m_socket, id);
        auto connectTarget = string("tcp://") + ip + string(":") + to_string(port);
        m_socket.connect(connectTarget);

        s_send(m_socket, "READY");
    }

    int ResourceTask::join()
    {
        while (!m_taskDone)
        {
            zmsg zm(m_socket);

            engine::EncodingTask task;
            performTask(task);

            zm.send(m_socket);
        }
        return 0;
    }

    void ResourceTask::performTask(const engine::EncodingTask& task)
    {
    }
}
