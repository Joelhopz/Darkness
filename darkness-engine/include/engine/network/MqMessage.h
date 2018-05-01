#pragma once

#include <vector>
#include <string>
#include "zmq.hpp"

namespace engine
{
    class MqMessage
    {
    public:
        MqMessage();
        MqMessage(zmq::socket_t& socket);

        MqMessage(MqMessage&& msg)
            : m_parts{ std::move(msg.m_parts) }
        {}
        MqMessage& operator=(MqMessage&& msg)
        {
            m_parts = std::move(msg.m_parts);
            return *this;
        }

        MqMessage(const MqMessage&) = delete;
        MqMessage& operator=(const MqMessage&) = delete;

        void prepend(const char* msg);
        void prepend(std::vector<char>&& msg);
        void prepend(const std::string& msg);

        void emplace_back(const char* msg);
        void emplace_back(std::vector<char>&& msg);
        void emplace_back(const std::string& msg);
        void send(zmq::socket_t& socket);

        const std::vector<std::vector<char>>& parts() const;
        std::vector<std::vector<char>>& parts();
    private:
        std::vector<std::vector<char>> m_parts;
    };
}
