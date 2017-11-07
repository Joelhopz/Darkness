#include "platform/FileWatcherWin.h"
#include "platform/FileWatcher.h"
#include "tools/Debug.h"
#include "tools/PathTools.h"
#include <chrono>
#include <string>

using namespace engine;

namespace platform
{
    
    FileWatcherImpl::FileWatcherImpl()
        : m_overlap{ 0, 0, 0, 0, CreateEvent(NULL, false, false, NULL) }
        , m_completionPort(RAIIHandle([]()->HANDLE {
            return CreateIoCompletionPort(
                (HANDLE)INVALID_HANDLE_VALUE,
                NULL,
                NULL,
                0); }))
        , m_alive(true)
        , m_idCounter{ std::make_shared<IdCounter>() }
        , m_task(std::async(
            std::launch::async,
            [&]() { heartbeat(); }))
    {
    }

    FileWatcherImpl::~FileWatcherImpl()
    {
        m_alive = false;

        Action exit{ ActionType::EXIT, nullptr };
        PostQueuedCompletionStatus(
            m_completionPort.get(),
            sizeof(Action),
            reinterpret_cast<ULONG_PTR>(nullptr),
            &m_overlap);

        m_task.get();
    }

    FileWatcherImpl::WatchContainer* FileWatcherImpl::locateExistingContainer(const std::string& path)
    {
        for (auto&& watch : m_watches)
        {
            if (watch.folder == path)
                return &watch;
        }
        return nullptr;
    }

    WatchHandle FileWatcherImpl::addWatch(
        const std::string& filePath,
        std::function<std::string(const std::string&)> onFileChange)
    {
        std::lock_guard<std::mutex> lock(m_watchesMutex);
        try
        {
            auto path = pathExtractFolder(filePath);
            auto existing = locateExistingContainer(path);
            if (existing)
            {
                existing->clients->emplace_back(ClientContainer{ filePath, onFileChange, m_idCounter->nextId() });
                std::vector<ClientContainer>* containerPtr = existing->clients.get();
                IdCounter* idCounter = m_idCounter.get();
                return WatchHandle(
                    [containerPtr](int64_t id)
                    {
                        for (int i = 0; i < containerPtr->size(); ++i)
                        {
                            if ((*containerPtr)[i].id == id)
                            {
                                containerPtr->erase(containerPtr->begin() + i);
                            }
                        }
                    },
                    [containerPtr, idCounter](int64_t id)->int64_t
                    {
                        int64_t newId = idCounter->nextId();
                        for (int i = 0; i < containerPtr->size(); ++i)
                        {
                            if ((*containerPtr)[i].id == id)
                            {
                                containerPtr->emplace_back(ClientContainer{ (*containerPtr)[i].filename, (*containerPtr)[i].callback, newId });
                            }
                        }
                        return newId;
                    },
                    idCounter->nextId()
                );
            }
            else
            {
                auto nextId = m_idCounter->nextId();
                IdCounter* idCounter = m_idCounter.get();

                ClientContainer clientContainer{
                    filePath,
                    onFileChange,
                    nextId };

                m_watches.emplace_back(
                    WatchContainer{ 
                        path, 
                        std::make_shared<WatchItem>(
                            m_completionPort.get(), 
                            path, 
                            [this](const std::string& pathChanged)
                            {
                                if(pathExtractExtension(pathChanged) == "hlsl")
                                    this->addNewChange(pathChanged);
                            }
                        ),
                        std::make_shared<std::vector<ClientContainer>>(
                            std::vector<ClientContainer>(
                                { clientContainer }
                            )
                        )
                    }
                );

                std::vector<ClientContainer>* containerPtr = m_watches.back().clients.get();
                return WatchHandle(
                    [containerPtr](int64_t id)
                    {
                        for (int i = 0; i < containerPtr->size(); ++i)
                        {
                            if ((*containerPtr)[i].id == id)
                            {
                                containerPtr->erase(containerPtr->begin() + i);
                            }
                        }
                    },
                    [containerPtr, idCounter](int64_t id)->int64_t
                    {
                        int64_t newId = idCounter->nextId();
                        for (int i = 0; i < containerPtr->size(); ++i)
                        {
                            if ((*containerPtr)[i].id == id)
                            {
                                containerPtr->emplace_back(ClientContainer{ (*containerPtr)[i].filename, (*containerPtr)[i].callback, newId });
                            }
                        }
                        return newId;
                    },
                    nextId
                );
            }
        }
        catch (WatcherBaseException ex)
        {
            printf("%s", ex.message());
        }
        return {};
    }

    void FileWatcherImpl::addNewChange(const std::string& filename)
    {
        std::lock_guard<std::mutex> lock(m_changeMutex);
        m_newChanges[filename] = std::chrono::high_resolution_clock::now();
    }

    bool FileWatcherImpl::hasChanges()
    {
        std::lock_guard<std::mutex> lock(m_changeMutex);
        return m_newChanges.size() > 0;
    }

    bool FileWatcherImpl::processChanges()
    {
        std::vector<std::string> changesToPerform;

        {
            std::lock_guard<std::mutex> lock(m_changeMutex);
            auto now = std::chrono::high_resolution_clock::now();

            bool hadChange = true;
            while (hadChange)
            {
                hadChange = false;
                for (auto change = m_newChanges.begin(); change != m_newChanges.end(); ++change)
                {
                    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - (*change).second).count() > 300)
                    {
                        changesToPerform.emplace_back((*change).first);
                        m_newChanges.erase(change);
                        hadChange = true;
                        break;
                    }
                }
            }
        }

        bool processedChanges = false;
        {
            for (auto&& change : changesToPerform)
            {
                for (auto&& container : m_watches)
                {
                    for (auto&& client : *container.clients)
                    {
                        if (client.filename == change)
                        {
                            processedChanges = true;
                            m_lastMessages.emplace_back(client.callback(change));
                        }
                    }
                }
            }
        }
        return processedChanges;
    }

    std::vector<std::string>& FileWatcherImpl::lastMessages()
    {
        return m_lastMessages;
    }

    std::string getNotifyString(DWORD action)
    {
        switch (action)
        {
            case FILE_ACTION_ADDED: { return std::string("FILE_ACTION_ADDED"); }
            case FILE_ACTION_REMOVED: { return std::string("FILE_ACTION_REMOVED"); }
            case FILE_ACTION_MODIFIED: { return std::string("FILE_ACTION_MODIFIED"); }
            case FILE_ACTION_RENAMED_OLD_NAME: { return std::string("FILE_ACTION_RENAMED_OLD_NAME"); }
            case FILE_ACTION_RENAMED_NEW_NAME: { return std::string("FILE_ACTION_RENAMED_NEW_NAME"); }
            default:
                return std::string("Unknown action");
        }
    }

    void FileWatcherImpl::heartbeat()
    {
        while (m_alive)
        {
            WatchItem* item = nullptr;
            LPOVERLAPPED overlapped;

            DWORD bytesReturned = 0;
            GetQueuedCompletionStatus(
                m_completionPort.get(),
                &bytesReturned,
                //&ulKey,
                (PULONG_PTR)&item,
                &overlapped,
                INFINITE);

            if (item)
            {
                char* ptr = (char*)item->m_notificationBuffer;
                while (ptr)
                {
                    FILE_NOTIFY_INFORMATION* notification = (FILE_NOTIFY_INFORMATION*)ptr;

                    std::wstring file(notification->FileName, notification->FileNameLength);

                    auto utf8file = engine::toUtf8String(file);
                    if(utf8file.length() != 0)
                        item->sendChangeNotification(engine::pathJoin(item->m_path, utf8file));

                    if (notification->NextEntryOffset == 0)
                        break;
                    ptr += notification->NextEntryOffset;
                }

                item->startOperation();
            }
            else
                break;

            OutputDebugStringA("\n");
        }
    }
}
