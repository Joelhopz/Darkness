#include "platform/FileWatcher.h"

#ifdef _WIN32
#include "platform/FileWatcherWin.h"
#endif

namespace platform
{
    FileWatcher::FileWatcher()
        : m_impl{ std::make_shared<FileWatcherImpl>() }
    {
    }

    WatchHandle FileWatcher::addWatch(
        const std::string& filePath,
        std::function<std::string(const std::string&)> onFileChange)
    {
        return m_impl->addWatch(filePath, onFileChange);
    }

    bool FileWatcher::hasChanges()
    {
        return m_impl->hasChanges();
    }

    bool FileWatcher::processChanges()
    {
        return m_impl->processChanges();
    }

    std::vector<std::string>& FileWatcher::lastMessages()
    {
        return m_impl->lastMessages();
    }
}
