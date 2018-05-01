#pragma once

#include <string>

class ProcessorTaskImageResponse;
class ProcessorTaskImageRequest;

namespace zmq
{
    class socket_t;
}

namespace resource_task
{
    class ImageTask
    {
    public:
        ProcessorTaskImageResponse process(
            ProcessorTaskImageRequest& request,
            const std::string& hostId,
            zmq::socket_t* socket);
    };
}
