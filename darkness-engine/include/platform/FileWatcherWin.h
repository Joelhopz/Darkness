#pragma once

#include "tools/PathTools.h"
#include <unordered_map>
#include <future>
#include <mutex>
#include <atomic>
#include <chrono>
#include <Windows.h>

namespace platform
{
    const int NotifyBufferSize = 1024;
    class WatchHandle;

    class FileWatcherImpl
    {
    private:

        class WatchItem;

        enum ActionType
        {
            ADDITEM = 0,
            EXIT = 1
        };

        struct Action
        {
            ActionType type;
            WatchItem* item;
        };

        class WatcherBaseException : public std::runtime_error
        {
        protected:
            char m_buffer[512];
        public:
            WatcherBaseException()
                : std::runtime_error("FileNotFoundException")
            {
                DWORD error = GetLastError();
                memset(&m_buffer[0], 0, 512);
                strerror_s(m_buffer, 512, error);
            }
            const char* message() const
            {
                return &m_buffer[0];
            }
        };
        class FailedToCreateHandle : public WatcherBaseException { };
        class FailedToReadDirectoryChanges : public WatcherBaseException { };

        typedef std::function<HANDLE()> CreateHandle;

        class RAIIHandle
        {
        private:
            HANDLE m_handle;
        public:
            RAIIHandle(CreateHandle create)
                : m_handle(create())
            {
                if (m_handle == INVALID_HANDLE_VALUE)
                    throw FailedToCreateHandle();
            }
            RAIIHandle operator=(const RAIIHandle&) = delete;
            RAIIHandle(const RAIIHandle&) = delete;
            RAIIHandle operator=(RAIIHandle&& handle) = delete;
            RAIIHandle(RAIIHandle&& handle)
                : m_handle(handle.m_handle)
            {
                handle.m_handle = INVALID_HANDLE_VALUE;
            }
            ~RAIIHandle()
            {
                if(m_handle != INVALID_HANDLE_VALUE)
                    CloseHandle(m_handle);
            }
            HANDLE get()
            {
                return m_handle;
            }
        };
        
        static const int MaxNotifications = 100;
        static const DWORD NotifyFilter = FILE_NOTIFY_CHANGE_FILE_NAME |
                                          FILE_NOTIFY_CHANGE_ATTRIBUTES |
                                          FILE_NOTIFY_CHANGE_SIZE |
                                          FILE_NOTIFY_CHANGE_LAST_WRITE |
                                          FILE_NOTIFY_CHANGE_CREATION;
        static const DWORD ShareMode = FILE_SHARE_READ |
                                       FILE_SHARE_WRITE |
                                       FILE_SHARE_DELETE;
        static const DWORD FlagsAndAttributes = FILE_FLAG_BACKUP_SEMANTICS |
                                                FILE_FLAG_OVERLAPPED;
        class WatchItem
        {
        private:
            
            bool m_directory;
            
            std::function<void(const std::string&)> m_onFileChange;
            
            
            std::shared_ptr<Action> m_lastAction;

            HANDLE createFile(const std::string& filePath)
            {
#ifdef UNICODE
                auto wstring = engine::toWideString(filePath);
                return CreateFile(
                    wstring.data(),               // lpFileName
                    FILE_LIST_DIRECTORY,    // dwDesiredAccess
                    ShareMode,              // dwShareMode
                    NULL,                   // lpSecurityAttributes
                    OPEN_EXISTING,          // dwCreationDisposition
                    FlagsAndAttributes,     // dwFlagsAndAttributes
                    NULL                    // hTemplateFile
                    );
#else
                return CreateFile(
                    filePath.data(),               // lpFileName
                    FILE_LIST_DIRECTORY,    // dwDesiredAccess
                    ShareMode,              // dwShareMode
                    NULL,                   // lpSecurityAttributes
                    OPEN_EXISTING,          // dwCreationDisposition
                    FlagsAndAttributes,     // dwFlagsAndAttributes
                    NULL                    // hTemplateFile
                );
#endif
            }
        public:
            FILE_NOTIFY_INFORMATION* m_notificationBuffer;
            RAIIHandle m_handle;

            void sendChangeNotification(const std::string& filePath)
            {
                m_onFileChange(filePath);
            }
            std::string m_path;
            WatchItem(
                HANDLE completionPort,
                const std::string& filePath,
                std::function<void(const std::string&)> onFileChange)
                : m_path(filePath)
                , m_onFileChange(onFileChange)
                , m_directory(engine::pathIsFolder(filePath))
                , m_handle(RAIIHandle([this, filePath]()->HANDLE {
                    return this->createFile(filePath); }))
                , m_lastAction(std::make_shared<Action>(Action{ ActionType::ADDITEM, this }))
            {
                m_notificationBuffer = new FILE_NOTIFY_INFORMATION[MaxNotifications];
                memset(&m_notificationBuffer[0], 0, sizeof(FILE_NOTIFY_INFORMATION) * MaxNotifications);
                // bind our handle to parents existing completion port
                CreateIoCompletionPort(
                    m_handle.get(),
                    completionPort,
                    (ULONG_PTR)this,
                    0);

                startOperation();
            }

            void startOperation()
            {
                DWORD bytesReturned = 0;
                memset(&m_notificationBuffer[0], 0, sizeof(FILE_NOTIFY_INFORMATION) * MaxNotifications);
                OVERLAPPED overlap{ 0, 0, 0, 0, 0 }; // CreateEvent(NULL, false, false, NULL)
                if (ReadDirectoryChangesW(
                    m_handle.get(),
                    m_notificationBuffer,
                    sizeof(FILE_NOTIFY_INFORMATION) * MaxNotifications,
                    FALSE,
                    NotifyFilter,
                    &bytesReturned,
                    &overlap,
                    NULL
                    ) == 0)
                {
                    throw FailedToReadDirectoryChanges();
                }
            }
        };

        class IdCounter
        {
        public:
            IdCounter()
                : m_id{ 0 }
            {}

            int64_t nextId()
            {
                int64_t res = m_id;
                ++m_id;
                return res;
            }
        private:
            int64_t m_id;
        };
        
        std::shared_ptr<IdCounter> m_idCounter;

        struct ClientContainer
        {
            std::string filename;
            std::function<std::string(const std::string&)> callback;
            int64_t id;
        };

        struct WatchContainer
        {
            std::string folder;
            std::shared_ptr<WatchItem> watch;
            std::shared_ptr<std::vector<ClientContainer>> clients;
        };

        WatchContainer* locateExistingContainer(const std::string& path);

        //std::unordered_map<std::string, std::shared_ptr<WatchItem>> m_watches;
        std::vector<WatchContainer> m_watches;
        
        RAIIHandle m_completionPort;
        OVERLAPPED m_overlap;

        std::atomic<bool> m_alive;
        std::future<void> m_task;
        std::mutex m_watchesMutex;

    public:
        void addNewChange(const std::string& filename);
    private:
        std::mutex m_changeMutex;
        std::unordered_map<std::string, std::chrono::high_resolution_clock::time_point> m_newChanges;
        std::vector<std::string> m_lastMessages;
        void heartbeat();
    public:
        FileWatcherImpl();
        ~FileWatcherImpl();
        WatchHandle addWatch(
            const std::string& filePath,
            std::function<std::string(const std::string&)> onFileChange);

        bool hasChanges();
        bool processChanges();
        std::vector<std::string>& lastMessages();
    };
}
