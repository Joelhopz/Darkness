#pragma once

#include <string>

namespace zmq
{
    class socket_t;
}

class ProcessorTaskModelRequest;
class ProcessorTaskModelResponse;

namespace resource_task
{
    class ModelTask
    {
    public:
        ProcessorTaskModelResponse process(
            ProcessorTaskModelRequest& request,
            const std::string& hostId,
            zmq::socket_t* socket);
    };
}
