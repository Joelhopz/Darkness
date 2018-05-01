#include "ImageTask.h"
#include "platform/Platform.h"
#include "protocols/network/ResourceProtocols.pb.h"
#include "Compressonator/Header/Compressonator.h"
#include "engine/network/MqMessage.h"
#include "engine/graphics/Format.h"
#include "tools/Debug.h"

using namespace engine;
using namespace zmq;
using namespace std;

namespace resource_task
{
    struct ImporterSurfaceWork
    {
        ProcessorTaskImageRequest* request;
        zmq::socket_t* socket;
        string hostId;
        float lastProgress;
    };

    bool feedback(float fProgress, DWORD_PTR pUser1, DWORD_PTR pUser2)
    {
        auto surfaceWork = reinterpret_cast<ImporterSurfaceWork*>(pUser1);

        float progress = fProgress / 100.0f;

        if (progress != surfaceWork->lastProgress)
        {
            surfaceWork->lastProgress = progress;
            ProcessorTaskMessageType type;
            type.set_type(ProcessorTaskMessageType::TaskProgress);
            vector<char> type_message(type.ByteSizeLong());
            if (type_message.size() > 0)
                type.SerializeToArray(&type_message[0], type_message.size());

            ProcessorTaskProgress prog;
            prog.set_taskid(surfaceWork->request->taskid());
            prog.set_progress(progress);
            vector<char> progData(prog.ByteSizeLong());
            if (progData.size() > 0)
                prog.SerializeToArray(&progData[0], progData.size());

            MqMessage msg;
            msg.emplace_back(surfaceWork->hostId);
            msg.emplace_back("");
            msg.emplace_back(std::move(type_message));
            msg.emplace_back(std::move(progData));
            msg.send(*surfaceWork->socket);
        }
        return false;
    }

    ProcessorTaskImageResponse ImageTask::process(
        ProcessorTaskImageRequest& request,
        const std::string& hostId,
        zmq::socket_t* socket)
    {
        ProcessorTaskImageResponse response;
        response.set_taskid(request.taskid());

        auto error = CMP_InitializeBCLibrary();
        if (error != BC_ERROR_NONE)
        {
            switch (error)
            {
            case BC_ERROR_LIBRARY_NOT_INITIALIZED: LOG("BC_ERROR_LIBRARY_NOT_INITIALIZED"); break;
            case BC_ERROR_LIBRARY_ALREADY_INITIALIZED: LOG("BC_ERROR_LIBRARY_ALREADY_INITIALIZED"); break;
            case BC_ERROR_INVALID_PARAMETERS: LOG("BC_ERROR_INVALID_PARAMETERS"); break;
            case BC_ERROR_OUT_OF_MEMORY: LOG("BC_ERROR_OUT_OF_MEMORY"); break;
            }
        }
        //ASSERT(error == BC_ERROR_NONE, "Block compression library failed to initialize");
        auto destinationSizeBytes = static_cast<uint32_t>(engine::formatBytes(
            static_cast<Format>(request.enginepackedformat()),
            request.width(),
            request.height()));

        std::vector<char> dstBuffer(destinationSizeBytes);

        CMP_Texture dstTexture;
        dstTexture.dwSize = sizeof(CMP_Texture);
        dstTexture.dwWidth = request.width();
        dstTexture.dwHeight = request.height();
        dstTexture.dwPitch = 0;
        dstTexture.format = static_cast<CMP_FORMAT>(request.targetcmbcformat());
        dstTexture.nBlockHeight = 4;
        dstTexture.nBlockWidth = 4;
        dstTexture.nBlockDepth = 1;
        dstTexture.dwDataSize = static_cast<CMP_DWORD>(destinationSizeBytes);
        dstTexture.pData = reinterpret_cast<uint8_t*>(dstBuffer.data());// +currentDstBytes;

        CMP_Texture srcTexture;
        srcTexture.dwSize = sizeof(CMP_Texture);
        srcTexture.dwWidth = request.width();
        srcTexture.dwHeight = request.height();
        srcTexture.dwPitch = request.stride();
        srcTexture.format = static_cast<CMP_FORMAT>(request.sourcecmformat());
        srcTexture.dwDataSize = static_cast<CMP_DWORD>(request.data().size());// dibSize;
        srcTexture.pData = const_cast<CMP_BYTE*>(reinterpret_cast<const CMP_BYTE*>(request.data().data()));

        CMP_CompressOptions options = {};
        options.dwSize = sizeof(CMP_CompressOptions);
        options.bDisableMultiThreading = true;
        //options.fquality = 1.0;

        ImporterSurfaceWork work;
        work.request = &request;
        work.lastProgress = 0.0f;
        work.socket = socket;
        work.hostId = hostId;

        CMP_ConvertTexture(
            &srcTexture,
            &dstTexture,
            &options,
            feedback,
            reinterpret_cast<DWORD_PTR>(&work),
            reinterpret_cast<DWORD_PTR>(nullptr));

        response.set_width(request.width());
        response.set_height(request.height());
        response.set_format(request.enginepackedformat());
        response.set_data(dstBuffer.data(), dstBuffer.size());
        LOG("Task finished encoding: %s", request.taskid().c_str());
        return response;
    }
}
