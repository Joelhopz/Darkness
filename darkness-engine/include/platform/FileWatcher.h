#pragma once

#include <string>
#include <functional>
#include <memory>
#include <vector>

namespace platform
{
    class FileWatcherImpl;
    
    class WatchHandle
    {
    public:
        WatchHandle() : m_id{ -1 } {}
        WatchHandle(const WatchHandle& cl)
        {
            m_id = cl.m_copyId(cl.m_id);
        }
        WatchHandle(WatchHandle&& cl)
            : m_id{ -1 }
        {
            std::swap(m_id, cl.m_id);
        }
        WatchHandle& operator=(const WatchHandle& cl)
        {
            if (m_id != -1)
                m_remove(m_id);
            m_id = m_copyId(cl.m_id);
        }
        WatchHandle& operator=(WatchHandle&& cl)
        {
            std::swap(m_id, cl.m_id);
        }
        ~WatchHandle()
        {
            if (m_id != -1)
                m_remove(m_id);
        }

    private:
        friend class FileWatcherImpl;

        WatchHandle(
            std::function<void(int64_t)> remove,
            std::function<int64_t(int64_t)> copy,
            int64_t id)
            : m_id{ id }
            , m_remove{ remove }
            , m_copyId{ copy }
        {}

        int64_t m_id;
        std::function<void(int64_t)> m_remove;
        std::function<int64_t(int64_t)> m_copyId;
    };

    class FileWatcher
    {
    public:
        FileWatcher();

        WatchHandle addWatch(
            const std::string& filePath, 
            std::function<std::string(const std::string&)> onFileChange);

        bool hasChanges();
        bool processChanges();
        std::vector<std::string>& lastMessages();

    private:
        std::shared_ptr<FileWatcherImpl> m_impl;
    };
}
