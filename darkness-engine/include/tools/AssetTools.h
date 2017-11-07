#pragma once

#include "engine/graphics/Format.h"
#include <string>
#include <functional>
#include <map>

namespace engine
{
    bool isModelFormat(const std::string& filepath);
    bool isImageFormat(const std::string& filepath);
    bool isPrefabFormat(const std::string& filepath);

    enum class AssetTask
    {
        ImageRecompress
    };

    struct AssetNotification
    {
        AssetTask task;
        std::string sourcePath;
        std::string destinationPath;
        engine::Format encodeType;
    };

    class AssetNotificationService
    {
    public:
        void registerListener(void* client, std::function<void(const AssetNotification&)> notification);
        void unregisterListener(void* client);

        void sendNotification(const AssetNotification& notification);

    private:
        std::map<void*, std::function<void(const AssetNotification&)>> m_clients;
    };

    extern AssetNotificationService GlobalAssetNotificationService;
}
