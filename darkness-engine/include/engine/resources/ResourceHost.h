#pragma once

#include "engine/graphics/Format.h"
#include "engine/resources/ResourceCommon.h"
#include "engine/network/NetworkCommunication.h"

#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>
#include <mutex>
#include <deque>

namespace platform
{
    class Socket;
    class SocketClient;
    struct NetworkMessage;
}

namespace engine
{
    class Process;
    
#ifdef _DEBUG
    static const char* ResourceClientPath = "..\\..\\..\\..\\darkness-resourceclient\\bin\\x64\\Debug DX12\\DarknessResourceClient.exe";
    static const char* ResourceClientWorkingPath = "..\\..\\..\\..\\darkness-resourceclient\\bin\\x64\\Debug DX12";
    
    static const char* ResourceClientFallbackPath = "..\\..\\..\\..\\darkness-resourceclient\\bin\\x64\\Release DX12\\DarknessResourceClient.exe";
    static const char* ResourceClientFallbackWorkingPath = "..\\..\\..\\..\\darkness-resourceclient\\bin\\x64\\Release DX12";
#else
    static const char* ResourceClientPath = "..\\..\\..\\..\\darkness-resourceclient\\bin\\x64\\Release DX12\\DarknessResourceClient.exe";
    static const char* ResourceClientWorkingPath = "..\\..\\..\\..\\darkness-resourceclient\\bin\\x64\\Release DX12";
    
    static const char* ResourceClientFallbackPath = "..\\..\\..\\..\\darkness-resourceclient\\bin\\x64\\Debug DX12\\DarknessResourceClient.exe";
    static const char* ResourceClientFallbackWorkingPath = "..\\..\\..\\..\\darkness-resourceclient\\bin\\x64\\Debug DX12";
#endif

    enum class TaskMessageType
    {
        TaskStarted,
        TaskDone
    };

    constexpr int FakeStartMaxSize = 5;

    class ResourceHost
    {
    public:
        ResourceHost();
        ~ResourceHost();

        void processResources(const ProcessResourcePackage& package);

    private:
        std::mutex m_taskIdMutex;

        // delayed start
        std::unique_ptr<std::thread, std::function<void(std::thread*)>> m_delayedLocalResourceClientStart;
        void delayedStart();
        void startLocalResourceClient();
        void stopLocalResourceClient();
        std::shared_ptr<Process> m_resourceClientProcess;
        volatile bool m_localResourceProcess;
        std::string m_localResourceProcessId;

        // TASK STUFF
        std::unique_ptr<std::thread, std::function<void(std::thread*)>> m_taskThread;
        std::mutex m_taskMutex;
        struct ProcessingItem
        {
            std::vector<ProcessResourceItem> items;
            ResourceProcessingStarted onStarted;
            ResourceProcessingFinished onFinished;
            ResourceProcessingProgress onProgress;
            ResourceProcessingProgressMessage onProgressMessage;
        };
        std::deque<ProcessingItem> m_taskQueue;
        ProcessingItem m_workItem;
        volatile bool m_taskAlive;
        volatile bool m_taskWorkedOn;
        void taskHandler();

        struct ProcessingTask
        {
            engine::ProcessResourceItem item;
            std::vector<char> data;
            std::string id;
        };
        std::mutex m_tasksListMutex;
        std::unordered_map<std::string, std::unique_ptr<ProcessingTask>> m_tasks;

        void sendTask(BeaconHello& beacon, ProcessingTask* task);

        std::vector<char> readFile(const std::string& path);

        std::unique_ptr<std::thread, std::function<void(std::thread*)>> m_taskWorkerThread;
        void taskWorkerHandler();

        NetworkCommunication m_netComm;
        std::mutex m_clientMutex;
        struct ClientContainer
        {
            BeaconHello client;
            uint32_t cores;
            std::vector<std::string> tasksActive;
        };
        std::vector<ClientContainer> m_knownClients;
        BeaconHello getAvailableClient(const std::string& taskId);

        void forceItemVisibility();
        std::vector<ProcessResourceItem> m_visibleItems;
    };
}
