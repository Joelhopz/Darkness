#include "engine/network/Beacon.h"
#include "engine/resources/ResourceMessages.h"
#include "platform/network/PrefixLengthMessageParser.h"
#include "platform/network/Socket.h"
#include "platform/network/SocketClient.h"
#include "platform/network/SocketServer.h"
#include "tools/Debug.h"
#include <random>

using namespace platform;
using namespace std;

namespace engine
{
    Beacon::Beacon(
        BeaconType type, 
        const std::string& identity,
        OnNetworkResourceDiscovery onDiscovery,
        OnNetworkResourceLoss onLoss,
        int keepAliveNotifyMilliSeconds,
        int keepAliveMilliSeconds)
        : m_type{ type }
        , m_identity{ identity }
        , m_onDiscovery{ onDiscovery }
        , m_onLoss{ onLoss }
        , m_keepAliveNotifyMilliSeconds{ keepAliveNotifyMilliSeconds }
        , m_keepAliveMilliSeconds{ keepAliveMilliSeconds }
        , m_localIp{ Socket::localIp() }
        , m_client{ std::make_shared<SocketClient>("255.255.255.255", 21343, false, true,
            [](std::shared_ptr<Socket> socket) { /*LOG_INFO("Client connected");*/ },
            [](std::shared_ptr<Socket> socket) { /*LOG_INFO("Client disconnected");*/ },
            [this](std::shared_ptr<Socket> socket, const platform::NetworkMessage& message)
            {
                this->onClientMessage(socket, message);
            })}
        , m_server{ make_shared<SocketServer>(21343, false, true,
            [](shared_ptr<Socket> socket) { /*LOG("Connection from: %s", socket->ip().c_str());*/ },
            [](shared_ptr<Socket> socket) { /*LOG("Client %s disconnected", socket->ip().c_str());*/ },
            [this](shared_ptr<Socket> socket, const NetworkMessage& message)
            {
                this->onServerMessage(socket, message);
            })}
        , m_alive{ true }
    {
        BeaconHello hello;
        hello.set_ip(m_localIp);
        hello.set_id(m_identity);
        hello.set_action(BeaconHello_BeaconAction::BeaconHello_BeaconAction_Entering);
        switch (m_type)
        {
        case BeaconType::Server: hello.set_type(BeaconHello_BeaconType_Server); break;
        case BeaconType::Client: hello.set_type(BeaconHello_BeaconType_Client); break;
        }
        std::vector<char> hellomsg(hello.ByteSizeLong());
        hello.SerializeToArray(&hellomsg[0], static_cast<int>(hellomsg.size()));
        m_client->sendMessage(m_client->socket(), hellomsg);

        m_thread = move(unique_ptr<thread, function<void(thread*)>>(new thread(
            // task
            [this]()
            {
                this->onKeepalive();
            }),

            // deleter
            [&](thread* ptr) {
                {
                    lock_guard<mutex> lock(m_mutex);
                    m_alive = false;
                }
                ptr->join();
                delete ptr;
        }));

        LOG_INFO("Discovery server started");
    }

    void Beacon::onKeepalive()
    {
        bool alive = true;
        while(alive)
        {
            {
                lock_guard<mutex> lock(m_mutex);
                alive = m_alive;
            }
            if (!alive)
                return;

            this_thread::sleep_for(chrono::milliseconds(m_keepAliveNotifyMilliSeconds));

            BeaconHello hello;
            hello.set_ip(m_localIp);
            hello.set_id(m_identity);
            hello.set_action(BeaconHello_BeaconAction::BeaconHello_BeaconAction_Alive);
            switch (m_type)
            {
            case BeaconType::Server: hello.set_type(BeaconHello_BeaconType_Server); break;
            case BeaconType::Client: hello.set_type(BeaconHello_BeaconType_Client); break;
            }
            std::vector<char> hellomsg(hello.ByteSizeLong());
            hello.SerializeToArray(&hellomsg[0], static_cast<int>(hellomsg.size()));
            m_client->sendMessage(m_client->socket(), hellomsg);

            {
                lock_guard<mutex> lock(m_mutex);
                auto now = std::chrono::high_resolution_clock::now();

                bool stillRemoving = true;
                while (stillRemoving)
                {
                    stillRemoving = false;
                    for (auto hold = m_onHoldLeaving.begin(); hold != m_onHoldLeaving.end(); ++hold)
                    {
                        auto duration = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(now - (*hold).lastSeen).count()) / 1000000.0;
                        if (duration > static_cast<double>(2000))
                        {
                            m_onHoldLeaving.erase(hold);
                            stillRemoving = true;
                            break;
                        }
                    }
                }

                bool somethingRemoved = true;
                while (somethingRemoved)
                {
                    somethingRemoved = false;
                    for (auto hl = m_knownClients.begin(); hl != m_knownClients.end(); ++hl)
                    {
                        auto duration = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(now - (*hl).lastSeen).count()) / 1000000.0;
                        if (duration > static_cast<double>(m_keepAliveMilliSeconds))
                        {
                            somethingRemoved = true;
                            LOG("Lost resource to keep alive: %s, Id: %s", (*hl).ip().c_str(), (*hl).id().c_str());
                            if (m_onLoss)
                                m_onLoss(*static_cast<BeaconHello*>(&(*hl)));
                            m_onHoldLeaving.emplace_back(*hl);
                            m_knownClients.erase(hl);
                            break;
                        }
                    }
                    for (auto hl = m_knownServers.begin(); hl != m_knownServers.end(); ++hl)
                    {
                        auto duration = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(now - (*hl).lastSeen).count()) / 1000000.0;
                        if (duration > static_cast<double>(m_keepAliveMilliSeconds))
                        {
                            somethingRemoved = true;
                            LOG("Lost resource to keep alive: %s, Id: %s", (*hl).ip().c_str(), (*hl).id().c_str());
                            if (m_onLoss)
                                m_onLoss(*static_cast<BeaconHello*>(&(*hl)));
                            m_onHoldLeaving.emplace_back(*hl);
                            m_knownServers.erase(hl);
                            break;
                        }
                    }
                }
            }
        }
    }

    Beacon::~Beacon()
    {
        BeaconHello hello;
        hello.set_ip(m_localIp);
        hello.set_id(m_identity);
        hello.set_action(BeaconHello_BeaconAction::BeaconHello_BeaconAction_Leaving);
        switch (m_type)
        {
        case BeaconType::Server: hello.set_type(BeaconHello_BeaconType_Server); break;
        case BeaconType::Client: hello.set_type(BeaconHello_BeaconType_Client); break;
        }
        std::vector<char> hellomsg(hello.ByteSizeLong());
        hello.SerializeToArray(&hellomsg[0], static_cast<int>(hellomsg.size()));
        m_client->sendMessage(m_client->socket(), hellomsg);
    }

    void Beacon::onClientMessage(shared_ptr<platform::Socket> socket, const NetworkMessage& message)
    {
        ASSERT(message.bytes >= sizeof(DiscoveryMessage), "Malformed Resource client message");
    }

    bool Beacon::isMe(const BeaconHello& hello)
    {
        return m_identity == hello.id();
    }

    bool Beacon::knownClient(const BeaconHello& hello)
    {
        for (auto&& hl : m_knownClients)
        {
            if (hl.id() == hello.id())
                return true;
        }
        for (auto&& hl : m_knownServers)
        {
            if (hl.id() == hello.id())
                return true;
        }
        return false;
    }

    void Beacon::onServerMessage(shared_ptr<platform::Socket> socket, const NetworkMessage& message)
    {
        lock_guard<mutex> lock(m_mutex);
        BeaconHello hello;
        hello.ParseFromArray(message.buffer, static_cast<int>(message.bytes));
        if (hello.type() == BeaconHello_BeaconType_Server && m_type == BeaconType::Server)
            return;
        if (hello.type() == BeaconHello_BeaconType_Client && m_type == BeaconType::Client)
            return;

        if(hello.action() == BeaconHello_BeaconAction::BeaconHello_BeaconAction_Entering  && !isMe(hello) && !knownClient(hello))
        {
            switch (hello.type())
            {
            case BeaconHello_BeaconType::BeaconHello_BeaconType_Client: m_knownClients.emplace_back(hello); break;
            case BeaconHello_BeaconType::BeaconHello_BeaconType_Server: m_knownServers.emplace_back(hello); break;
            }

            if (m_onDiscovery)
                m_onDiscovery(hello);

            hello.set_ip(m_localIp);
            hello.set_id(m_identity);
            switch (m_type)
            {
            case BeaconType::Server: hello.set_type(BeaconHello_BeaconType_Server); break;
            case BeaconType::Client: hello.set_type(BeaconHello_BeaconType_Client); break;
            }

            std::vector<char> hellomsg(hello.ByteSizeLong());
            hello.SerializeToArray(&hellomsg[0], static_cast<int>(hellomsg.size()));
            m_client->sendMessage(m_client->socket(), hellomsg);
        }
        else if (hello.action() == BeaconHello_BeaconAction::BeaconHello_BeaconAction_Leaving && !isMe(hello))
        {
            for (auto hl = m_knownClients.begin(); hl != m_knownClients.end(); ++hl)
            {
                if ((*hl).id() == hello.id())
                {
                    if (m_onLoss)
                        m_onLoss(hello);
                    m_onHoldLeaving.emplace_back(*hl);
                    m_knownClients.erase(hl);
                    return;
                }
            }
            for (auto hl = m_knownServers.begin(); hl != m_knownServers.end(); ++hl)
            {
                if ((*hl).id() == hello.id())
                {
                    if (m_onLoss)
                        m_onLoss(hello);
                    m_onHoldLeaving.emplace_back(*hl);
                    m_knownServers.erase(hl);
                    return;
                }
            }
        }
        else if(hello.action() == BeaconHello_BeaconAction::BeaconHello_BeaconAction_Alive && !isMe(hello))
        {
            auto now = std::chrono::high_resolution_clock::now();

            bool found = false;
            for (auto&& hl : m_knownClients)
            {
                if (hl.id() == hello.id())
                {
                    hl.lastSeen = now;
                    found = true;
                    break;
                }
            }
            for (auto&& hl : m_onHoldLeaving)
            {
                if (hl.id() == hello.id())
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                for (auto&& hl : m_knownServers)
                {
                    if (hl.id() == hello.id())
                    {
                        hl.lastSeen = now;
                        found = true;
                        break;
                    }
                }
            }
            if (!found)
            {
                switch (hello.type())
                {
                case BeaconHello_BeaconType::BeaconHello_BeaconType_Client: m_knownClients.emplace_back(hello); break;
                case BeaconHello_BeaconType::BeaconHello_BeaconType_Server: m_knownServers.emplace_back(hello); break;
                }

                if (m_onDiscovery)
                    m_onDiscovery(hello);

                hello.set_ip(m_localIp);
                hello.set_id(m_identity);
                switch (m_type)
                {
                case BeaconType::Server: hello.set_type(BeaconHello_BeaconType_Server); break;
                case BeaconType::Client: hello.set_type(BeaconHello_BeaconType_Client); break;
                }

                std::vector<char> hellomsg(hello.ByteSizeLong());
                hello.SerializeToArray(&hellomsg[0], static_cast<int>(hellomsg.size()));
                m_client->sendMessage(m_client->socket(), hellomsg);
            }
        }
    }
}
