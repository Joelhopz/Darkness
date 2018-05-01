#pragma once

#include <string>
#include <unordered_map>

namespace engine
{
    class ArgParser
    {
    public:
        ArgParser(int argc, char **argv);
        std::string value(const std::string& key);
        bool flag(const std::string& flag) const;
        const std::vector<std::string>& flags() const;
    private:
        std::string m_pathOfExecutable;
        void parseArg(const std::string& arg);
        std::unordered_map<std::string, std::string> m_values;
        std::vector<std::string> m_flags;
    };
}
