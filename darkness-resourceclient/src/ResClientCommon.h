#pragma once

#include <string>
#include <vector>
#include <memory>
#include "engine/primitives/Vector3.h"
#include "engine/primitives/Quaternion.h"
#include "platform/Environment.h"

namespace engine
{
    class Process;
}

namespace resource_client
{
#ifdef _DEBUG
    static const char* ResourceTaskPath = "..\\..\\..\\..\\darkness-resourcetask\\bin\\x64\\Debug DX12\\DarknessResourceTask.exe";
    static const char* ResourceTaskWorkingPath = "..\\..\\..\\..\\darkness-resourcetask\\bin\\x64\\Debug DX12";

    static const char* ResourceTaskFallbackPath = "..\\..\\..\\..\\darkness-resourcetask\\bin\\x64\\Release DX12\\DarknessResourceTask.exe";
    static const char* ResourceTaskFallbackWorkingPath = "..\\..\\..\\..\\darkness-resourcetask\\bin\\x64\\Release DX12";
#else
    static const char* ResourceTaskPath = "..\\..\\..\\..\\darkness-resourcetask\\bin\\x64\\Release DX12\\DarknessResourceTask.exe";
    static const char* ResourceTaskWorkingPath = "..\\..\\..\\..\\darkness-resourcetask\\bin\\x64\\Release DX12";

    static const char* ResourceTaskFallbackPath = "..\\..\\..\\..\\darkness-resourcetask\\bin\\x64\\Debug DX12\\DarknessResourceTask.exe";
    static const char* ResourceTaskFallbackWorkingPath = "..\\..\\..\\..\\darkness-resourcetask\\bin\\x64\\Debug DX12";
#endif

    static const char* ResourceTaskArgumentsIp = "--ip=";
    static const char* ResourceTaskArgumentsPort = " --port=";
    static const char* ResourceTaskArgumentsId = " --id=";
    static const char* ResourceTaskArgumentsHostId = " --hostid=";
    

    enum class TaskType
    {
        Image,
        Model
    };

    struct TaskImage
    {
        std::string extension;
        int32_t format;
        bool generateMips;
    };

    struct TaskModel
    {
        std::string modelTargetPath;
        std::string assetName;
        engine::Vector3f scale;
        engine::Quaternionf rotation;
    };

    struct Task
    {
        std::string hostId;
        std::string hostTaskId;
        std::string taskId;

        TaskType type;
        TaskImage image;
        TaskModel model;

        std::vector<char> data;

        float lastProgressUpdateValue = -1.0f;
        std::string lastProgressMessageUpdateValue = "";
    };

    struct TaskImageResult
    {
        std::vector<char> data;
        uint32_t width;
        uint32_t height;
        uint32_t mips;
        int32_t format;
    };

    struct TaskModelResult
    {
        std::vector<char> modelData;
        std::vector<char> prefabData;
    };

    struct TaskResult
    {
        TaskImageResult image;
        TaskModelResult model;
        TaskType type;
        std::string id;
        std::string hostid;
    };

    struct Worker
    {
        bool clientReady;
        std::string id;
        std::shared_ptr<engine::Process> process;
    };

    struct SplitTaskImageResult
    {
        int32_t width;
        int32_t height;
        int32_t format;
        std::vector<char> data;
    };

    struct SplitTaskModelResult
    {
        std::vector<char> modelData;
        std::vector<char> prefabData;
    };

    struct SplitTaskResult
    {
        TaskType type;
        SplitTaskImageResult image;
        SplitTaskModelResult model;
    };

    struct SplitImageTask
    {
        int32_t width;
        int32_t height;
        int32_t stride;
        int32_t sourceFormat;
        int32_t targetFormat;
        int32_t originalFormat;
    };

    struct SplitModelTask
    {
        std::string modelTargetPath;
        std::string assetName;
        engine::Vector3f scale;
        engine::Quaternionf rotation;
    };

    struct SplitTask
    {
        bool requestedWorker = false;
        bool returnedWorker = false;
        bool processing = false;
        bool done = false;
        float progress = 0.0f;
        std::string progressMessage = "";
        uint64_t bytes;
        Worker worker;
        SplitTaskResult result;
        TaskType type;

        std::string taskId;
        std::string subTaskId;
        SplitImageTask image;
        SplitModelTask model;
        std::vector<char> data;
    };

}
