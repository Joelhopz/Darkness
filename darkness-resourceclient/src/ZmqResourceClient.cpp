#include "ZmqResourceClient.h"
#include "zmsg.hpp"
#include "engine/network/MqMessage.h"
#include "engine/network/Beacon.h"
#include "tools/Debug.h"
#include "tools/PathTools.h"
#include "platform/network/Socket.h"
#include "platform/Uuid.h"
#include "tools/ProcessorInfo.h"
#include "tools/Process.h"
#include "TaskSplitter.h"

using namespace std;
using namespace zmq;
using namespace engine;

namespace resource_client
{
    std::string addressFromIp(const std::string& ip, int port)
    {
        string address = "tcp://";
        address += ip;
        address += ":";
        address += to_string(port);
        return address;
    }

    ResourceClient::ResourceClient(const std::string& frontIdentity)
        : m_headless{ frontIdentity != "" }
        , m_frontIdentity{ frontIdentity == "" ? platform::uuid() : frontIdentity }
        , m_backIdentity{ platform::uuid() }
        , m_beacon{ std::make_shared<Beacon>(
            BeaconType::Server, m_frontIdentity,
            [this](const BeaconHello& hello) { this->onDiscovery(hello); },
            [this](const BeaconHello& hello) { this->onLoss(hello); }) }
        , m_context{ 1 }
        , m_front{ m_context, socket_type::router }
        , m_back{ m_context, socket_type::router }
        , m_polls{
            { m_front, 0, ZMQ_POLLIN,  0 },
            { m_back, 0, ZMQ_POLLIN,  0 } }
        , m_localIp{ platform::Socket::localIp() }
        , m_shouldShutdown{ false }
    {
        m_front.setsockopt(ZMQ_IDENTITY, m_frontIdentity.data(), m_frontIdentity.size());
        m_front.setsockopt<int>(ZMQ_ROUTER_MANDATORY, 1);
        m_front.bind("tcp://*:12134");

        m_back.setsockopt(ZMQ_IDENTITY, m_backIdentity.data(), m_backIdentity.size());
        m_back.setsockopt<int>(ZMQ_ROUTER_MANDATORY, 1);
        m_back.bind("tcp://*:12135");

        LOG("Host started. Front ID: %s, Back ID: %s",
            m_frontIdentity.c_str(),
            m_backIdentity.c_str());
    }

    void ResourceClient::onDiscovery(const BeaconHello& hello)
    {
        LOG_INFO("Found resource client through discovery: %s", hello.ip().c_str());
    }

    void ResourceClient::onLoss(const BeaconHello& hello)
    {
        LOG_INFO("Lost resource client: %s, Id: %s", hello.ip().c_str(), hello.id().c_str());
        if(m_headless && hello.type() == BeaconHello::Client)
            m_shouldShutdown = true;
    }

    template<typename StructType, typename MsgType>
    vector<char> packageType(MsgType type)
    {
        StructType reqtype;
        reqtype.set_type(type);
        vector<char> typeMessage(reqtype.ByteSizeLong());
        if (typeMessage.size() > 0)
            reqtype.SerializeToArray(&typeMessage[0], typeMessage.size());
        return typeMessage;
    }

    template<typename StructType, typename MsgType>
    MqMessage msgTo(const string& address, MsgType type)
    {
        MqMessage msg;
        msg.emplace_back(address);
        msg.emplace_back("");
        msg.emplace_back(packageType<StructType, MsgType>(type));
        return msg;
    }

    template<typename StructType, typename MsgType, typename PayloadType>
    void respond(
        const string& id, 
        MsgType type,
        PayloadType& answer,
        socket_t& socket)
    {
        auto msg = msgTo<StructType, MsgType>(id, type);
        vector<char> msgPayload(answer.ByteSizeLong());
        if (msgPayload.size() > 0)
            answer.SerializeToArray(&msgPayload[0], msgPayload.size());
        msg.emplace_back(std::move(msgPayload));
        msg.send(socket);
    }

    bool ResourceClient::hasTasks()
    {
        return m_tasks.size() > 0;
    }

    Task ResourceClient::getTask()
    {
        ASSERT(m_tasks.size() > 0, "Tried to get task from an empty task list");
        Task task = std::move(m_tasks.back());
        m_tasks.erase(m_tasks.end() - 1);
        return task;
    }

    bool ResourceClient::hasWorkers()
    {
        bool res = false;
        for (auto&& worker : m_workers)
        {
            if (worker.clientReady)
            {
                res = true;
                break;
            }
        }
        return res;
    }

    void ResourceClient::startWorker()
    {
        auto exeLocation = engine::getExecutableDirectory();
        auto taskPath = pathClean(pathJoin(pathExtractFolder(exeLocation), ResourceTaskPath));
        auto taskWorkingDirPath = pathClean(pathJoin(pathExtractFolder(exeLocation), ResourceTaskWorkingPath));
        if (!pathExists(taskPath))
        {
            taskPath = pathClean(pathJoin(pathExtractFolder(exeLocation), ResourceTaskFallbackPath));
            taskWorkingDirPath = pathClean(pathJoin(pathExtractFolder(exeLocation), ResourceTaskFallbackWorkingPath));
        }
        ASSERT(
            pathExists(taskPath), 
            "Task executable has not been built. Looking from %s", 
            taskPath.c_str());

        auto newId = platform::uuid();

        Worker worker;
        worker.clientReady = false;
        worker.id = newId;

        auto arguments = ResourceTaskArgumentsIp + m_localIp +
            ResourceTaskArgumentsPort + "12135" +
            ResourceTaskArgumentsId + worker.id +
            ResourceTaskArgumentsHostId + m_backIdentity;
        //LOG("Arguments: %s", arguments.c_str());

        worker.process = std::make_shared<Process>(
            taskPath,
            arguments,
            taskWorkingDirPath,
            [](const std::string& message)
        {
            LOG_PURE_ERROR("%s", message.c_str());
        });

        /*LOG("Waiting for user to start TASK");
        this_thread::sleep_for(chrono::milliseconds(180000));*/

        m_workers.emplace_back(std::move(worker));

        LOG("Started Task Process ID: %s", newId.c_str());
    }

    Worker ResourceClient::getWorker()
    {
        //for (auto&& worker : m_workers)
        for(auto worker = m_workers.begin(); worker != m_workers.end(); ++worker)
        {
            if ((*worker).clientReady)
            {
                Worker res = std::move(*worker);
                m_workers.erase(worker);
                return res;
            }
        }
        ASSERT(false, "Tried to get worker when there was none available");
        return {};
    }

    void ResourceClient::disposeWorker(Worker&& worker)
    {
        auto msg = msgTo<
            ProcessorTaskMessageType, 
            ProcessorTaskMessageType_MessageType>(
                worker.id, 
                ProcessorTaskMessageType::DoneRequest);

        msg.send(m_back);
    }

    void ResourceClient::processTask(SplitTask& task)
    {
        MqMessage msg;
        msg.emplace_back(task.worker.id);
        msg.emplace_back("");
        msg.emplace_back(packageType<ProcessorTaskMessageType, ProcessorTaskMessageType_MessageType>(
            ProcessorTaskMessageType::TaskRequest));
        
        if (task.type == TaskType::Image)
        {
            msg.emplace_back(packageType<ProcessorTaskRequest, ProcessorTaskRequest_TaskType>(
                ProcessorTaskRequest::Image));

            ProcessorTaskImageRequest req;
            req.set_taskid(task.subTaskId);
            req.set_sourcecmformat(task.image.sourceFormat);
            req.set_targetcmbcformat(task.image.targetFormat);
            req.set_enginepackedformat(task.image.originalFormat);
            req.set_width(task.image.width);
            req.set_height(task.image.height);
            req.set_stride(task.image.stride);
            req.set_data(task.data.data(), task.data.size());
            vector<char> imageReqMsg(req.ByteSizeLong());
            if (imageReqMsg.size() > 0)
                req.SerializeToArray(&imageReqMsg[0], imageReqMsg.size());
            msg.emplace_back(std::move(imageReqMsg));
        }
        else if (task.type == TaskType::Model)
        {
            msg.emplace_back(packageType<ProcessorTaskRequest, ProcessorTaskRequest_TaskType>(
                ProcessorTaskRequest::Model));

            ProcessorTaskModelRequest req;
            req.set_taskid(task.subTaskId);
            req.set_modeltargetpath(task.model.modelTargetPath);
            req.set_assetname(task.model.assetName);
            req.set_scalex(task.model.scale.x);
            req.set_scaley(task.model.scale.y);
            req.set_scalez(task.model.scale.z);
            req.set_rotationx(task.model.rotation.x);
            req.set_rotationy(task.model.rotation.y);
            req.set_rotationz(task.model.rotation.z);
            req.set_rotationw(task.model.rotation.w);
            req.set_modeldata(task.data.data(), task.data.size());
            vector<char> modelReqMsg(req.ByteSizeLong());
            if (modelReqMsg.size() > 0)
                req.SerializeToArray(&modelReqMsg[0], modelReqMsg.size());
            msg.emplace_back(std::move(modelReqMsg));
        }
        else
            ASSERT(false, "Unhandled SplitTaskType");

        msg.send(m_back);

        m_splitTasks.emplace_back(&task);
    }

    void ResourceClient::updateTaskProgress(
        const std::string& hostId,
        const std::string& hostTaskId,
        float progress)
    {
        HostProcessorMessageType type;
        type.set_type(HostProcessorMessageType::TaskProgress);
        vector<char> typeData(type.ByteSizeLong());
        if (typeData.size() > 0)
            type.SerializeToArray(&typeData[0], typeData.size());

        HostTaskProgress prog;
        prog.set_taskid(hostTaskId);
        prog.set_progress(progress);
        vector<char> progData(prog.ByteSizeLong());
        if (progData.size() > 0)
            prog.SerializeToArray(&progData[0], progData.size());

        MqMessage msg;
        msg.emplace_back(hostId);
        msg.emplace_back("");
        msg.emplace_back(move(typeData));
        msg.emplace_back(move(progData));
        msg.send(m_front);
    }

    void ResourceClient::updateTaskProgressMessage(
        const std::string& hostId,
        const std::string& hostTaskId,
        float progress,
        const std::string& message)
    {
        HostProcessorMessageType type;
        type.set_type(HostProcessorMessageType::TaskProgressMessage);
        vector<char> typeData(type.ByteSizeLong());
        if (typeData.size() > 0)
            type.SerializeToArray(&typeData[0], typeData.size());

        HostTaskProgressMessage prog;
        prog.set_taskid(hostTaskId);
        prog.set_progress(progress);
        prog.set_message(message);
        vector<char> progData(prog.ByteSizeLong());
        if (progData.size() > 0)
            prog.SerializeToArray(&progData[0], progData.size());

        MqMessage msg;
        msg.emplace_back(hostId);
        msg.emplace_back("");
        msg.emplace_back(move(typeData));
        msg.emplace_back(move(progData));
        msg.send(m_front);
    }

    bool ResourceClient::shouldShutdown() const
    {
        return m_shouldShutdown;
    }

    void ResourceClient::sendTaskStart(
        const std::string& hostId,
        const std::string& hostTaskId)
    {
        HostProcessorMessageType type;
        type.set_type(HostProcessorMessageType::TaskStarted);
        vector<char> typeData(type.ByteSizeLong());
        if (typeData.size() > 0)
            type.SerializeToArray(&typeData[0], typeData.size());

        HostTaskStarted prog;
        prog.set_taskid(hostTaskId);
        vector<char> progData(prog.ByteSizeLong());
        if (progData.size() > 0)
            prog.SerializeToArray(&progData[0], progData.size());

        MqMessage msg;
        msg.emplace_back(hostId);
        msg.emplace_back("");
        msg.emplace_back(move(typeData));
        msg.emplace_back(move(progData));
        msg.send(m_front);
    }

    void ResourceClient::sendTaskFinished(
        const std::string& hostId,
        const std::string& hostTaskId)
    {
        HostProcessorMessageType type;
        type.set_type(HostProcessorMessageType::TaskFinished);
        vector<char> typeData(type.ByteSizeLong());
        if (typeData.size() > 0)
            type.SerializeToArray(&typeData[0], typeData.size());

        HostTaskFinished prog;
        prog.set_taskid(hostTaskId);
        vector<char> progData(prog.ByteSizeLong());
        if (progData.size() > 0)
            prog.SerializeToArray(&progData[0], progData.size());

        MqMessage msg;
        msg.emplace_back(hostId);
        msg.emplace_back("");
        msg.emplace_back(move(typeData));
        msg.emplace_back(move(progData));
        msg.send(m_front);
    }

    void ResourceClient::finishTask(
        TaskResult&& task,
        const std::string& hostTaskId)
    {
        if (task.type == TaskType::Image)
        {
            HostProcessorMessageType type;
            type.set_type(HostProcessorMessageType::TaskImageResponse);
            vector<char> typeData(type.ByteSizeLong());
            if (typeData.size() > 0)
                type.SerializeToArray(&typeData[0], typeData.size());

            HostTaskImageResponse resp;
            resp.set_taskid(hostTaskId);
            resp.set_data(task.image.data.data(), task.image.data.size());
            resp.set_width(task.image.width);
            resp.set_height(task.image.height);
            resp.set_mips(task.image.mips);
            resp.set_format(task.image.format);
            vector<char> respData(resp.ByteSizeLong());
            if (respData.size() > 0)
                resp.SerializeToArray(&respData[0], respData.size());

            MqMessage msg;
            msg.emplace_back(task.hostid);
            msg.emplace_back("");
            msg.emplace_back(move(typeData));
            msg.emplace_back(move(respData));
            msg.send(m_front);
        }
        else if (task.type == TaskType::Model)
        {
            HostProcessorMessageType type;
            type.set_type(HostProcessorMessageType::TaskModelResponse);
            vector<char> typeData(type.ByteSizeLong());
            if (typeData.size() > 0)
                type.SerializeToArray(&typeData[0], typeData.size());

            HostTaskModelResponse resp;
            resp.set_taskid(hostTaskId);
            resp.set_modeldata(task.model.modelData.data(), task.model.modelData.size());
            resp.set_prefabdata(task.model.prefabData.data(), task.model.prefabData.size());
            vector<char> respData(resp.ByteSizeLong());
            if (respData.size() > 0)
                resp.SerializeToArray(&respData[0], respData.size());

            MqMessage msg;
            msg.emplace_back(task.hostid);
            msg.emplace_back("");
            msg.emplace_back(move(typeData));
            msg.emplace_back(move(respData));
            msg.send(m_front);
        }
        else
            ASSERT(false, "Unsupported task type");
    }

    bool ResourceClient::runService()
    {
        bool workHappened = false;
        poll(m_polls, 20);

        // check for resource host control messages (front)
        if (m_polls[0].revents & ZMQ_POLLIN)
        {
            workHappened = true;
            MqMessage msg(m_front);
            string id = string(msg.parts()[0].begin(), msg.parts()[0].end());
                
            HostProcessorMessageType type;
            type.ParseFromArray(msg.parts()[1].data(), msg.parts()[1].size());

            switch (type.type())
            {
                case HostProcessorMessageType::CoreRequest:
                {
                    HostCoreResponse coreAnswer;
                    auto processorInfo = getProcessorInfo();
                    coreAnswer.set_cores(processorInfo.processorCoreCount);

                    respond<HostProcessorMessageType, HostProcessorMessageType_MessageType, HostCoreResponse>(
                        id, HostProcessorMessageType::CoreResponse, coreAnswer, m_front);

                    LOG("Got Core Query from ID: %s, responding", id.c_str());
                    break;
                }
                case HostProcessorMessageType::TaskImageRequest:
                {
                    HostTaskImageRequest req;
                    req.ParseFromArray(msg.parts()[2].data(), msg.parts()[2].size());

                    Task container;
                    container.type = TaskType::Image;
                    container.hostId = id;
                    container.hostTaskId = req.taskid();
                    container.taskId = platform::uuid();

                    container.image.extension = req.extension();
                    container.image.format = req.format();
                    container.image.generateMips = req.generatemips();
                    vector<char> data(req.data().size());
                    memcpy(&data[0], req.data().data(), data.size());
                    container.data = data;

                    m_tasks.emplace_back(std::move(container));
                    break;
                }
                case HostProcessorMessageType::TaskModelRequest:
                {
                    HostTaskModelRequest req;
                    req.ParseFromArray(msg.parts()[2].data(), msg.parts()[2].size());

                    Task container;
                    container.type = TaskType::Model;
                    container.hostId = id;
                    container.hostTaskId = req.taskid();
                    container.taskId = platform::uuid();

                    container.model.modelTargetPath = req.modeltargetpath();
                    container.model.assetName = req.assetname();
                    container.model.scale.x = req.scalex();
                    container.model.scale.y = req.scaley();
                    container.model.scale.z = req.scalez();
                    container.model.rotation.x = req.rotationx();
                    container.model.rotation.y = req.rotationy();
                    container.model.rotation.z = req.rotationz();
                    container.model.rotation.w = req.rotationw();
                    vector<char> data(req.data().size());
                    memcpy(&data[0], req.data().data(), data.size());
                    container.data = data;

                    m_tasks.emplace_back(std::move(container));
                    break;
                }
                case HostProcessorMessageType::ShutdownProcessor:
                {
                    // exit this program
                    m_shouldShutdown = true;
                    break;
                }
            }
        }

        // check for task client messages (back)
        if (m_polls[1].revents & ZMQ_POLLIN)
        {
            workHappened = true;
            MqMessage msg(m_back);
            string id = string(msg.parts()[0].begin(), msg.parts()[0].end());

            ProcessorTaskMessageType type;
            type.ParseFromArray(msg.parts()[1].data(), msg.parts()[1].size());

            switch (type.type())
            {
                case ProcessorTaskMessageType::Helo:
                {
                    for (auto&& worker : m_workers)
                    {
                        if (worker.id == id)
                        {
                            worker.clientReady = true;
                            break;
                        }
                    }
                    break;
                }
                case ProcessorTaskMessageType::TaskProgress:
                {
                    ProcessorTaskProgress prog;
                    prog.ParseFromArray(msg.parts()[2].data(), msg.parts()[2].size());

                    for (auto&& split : m_splitTasks)
                    {
                        if (split->subTaskId == prog.taskid())
                        {
                            split->progress = prog.progress();
                            LOG("Task ID: %s, Subtask ID: %s, Progress: %f",
                                split->taskId.c_str(),
                                split->subTaskId.c_str(),
                                split->progress);
                            break;
                        }
                    }

                    break;
                }
                case ProcessorTaskMessageType::TaskProgressMessage:
                {
                    ProcessorTaskMessageProgress prog;
                    prog.ParseFromArray(msg.parts()[2].data(), msg.parts()[2].size());

                    for (auto&& split : m_splitTasks)
                    {
                        if (split->subTaskId == prog.taskid())
                        {
                            split->progress = prog.progress();
                            split->progressMessage = prog.message();
                            LOG("Task ID: %s, Subtask ID: %s, Progress: %f, Message: %s",
                                split->taskId.c_str(),
                                split->subTaskId.c_str(),
                                split->progress,
                                split->progressMessage.c_str());
                            break;
                        }
                    }

                    break;
                }
                case ProcessorTaskMessageType::TaskResponse:
                {
                    ProcessorTaskRequest requestType;
                    requestType.ParseFromArray(msg.parts()[2].data(), msg.parts()[2].size());

                    switch (requestType.type())
                    {
                        case ProcessorTaskRequest::Image:
                        {
                            ProcessorTaskImageResponse taskRes;
                            taskRes.ParseFromArray(msg.parts()[3].data(), msg.parts()[3].size());

                            //for (auto&& split : m_splitTasks)
                            for (auto spl = m_splitTasks.begin(); spl != m_splitTasks.end(); ++spl)
                            {
                                auto split = *spl;
                                if (split->subTaskId == taskRes.taskid())
                                {
                                    split->result.type = TaskType::Image;
                                    split->result.image.width = taskRes.width();
                                    split->result.image.height = taskRes.height();
                                    split->result.image.format = taskRes.format();
                                    split->result.image.data.resize(taskRes.data().size());
                                    memcpy(&split->result.image.data[0], taskRes.data().data(), split->result.image.data.size());

                                    LOG("Task ID: %s, Subtask ID: %s, DONE",
                                        split->taskId.c_str(),
                                        split->subTaskId.c_str());

                                    split->done = true;
                                    m_splitTasks.erase(spl);
                                    break;
                                }
                            }
                            break;
                        }
                        case ProcessorTaskRequest::Model:
                        {
                            ProcessorTaskModelResponse taskRes;
                            taskRes.ParseFromArray(msg.parts()[3].data(), msg.parts()[3].size());

                            //for (auto&& split : m_splitTasks)
                            for (auto spl = m_splitTasks.begin(); spl != m_splitTasks.end(); ++spl)
                            {
                                auto split = *spl;
                                if (split->subTaskId == taskRes.taskid())
                                {
                                    split->result.type = TaskType::Model;
                                    split->result.model.modelData.resize(taskRes.modeldata().size());
                                    split->result.model.prefabData.resize(taskRes.prefabdata().size());
                                    memcpy(
                                        &split->result.model.modelData[0], 
                                        taskRes.modeldata().data(), 
                                        split->result.model.modelData.size());
                                    memcpy(
                                        &split->result.model.prefabData[0],
                                        taskRes.prefabdata().data(),
                                        split->result.model.prefabData.size());

                                    LOG("Task ID: %s, Subtask ID: %s, DONE",
                                        split->taskId.c_str(),
                                        split->subTaskId.c_str());

                                    split->done = true;
                                    m_splitTasks.erase(spl);
                                    break;
                                }
                            }
                            break;
                        }
                    }
                    

                    break;
                }
            }
        }
        return workHappened;
    }
}
