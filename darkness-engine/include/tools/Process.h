#pragma once

#include <string>
#include <functional>
#include <thread>

namespace engine
{
    using OnProcessMessage = std::function<void(const std::string&)>;
    class Process
    {
    public:
        Process(
            const std::string& executable, 
            const std::string& arguments,
            const std::string& workingDirectory,
            OnProcessMessage onMessage
            );
    private:
        std::string m_executable;
        std::string m_arguments;
        std::string m_workingDirectory;
        OnProcessMessage m_onMessage;
        std::unique_ptr<std::thread, std::function<void(std::thread*)>> m_work;
        void run();
    };
}
