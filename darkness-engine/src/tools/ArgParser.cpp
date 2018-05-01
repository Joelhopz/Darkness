#include "tools/ArgParser.h"
#include "tools/StringTools.h"

namespace engine
{
    ArgParser::ArgParser(int argc, char **argv)
    {
        m_pathOfExecutable = std::string(argv[0]);
        for (int i = 1; i < argc; ++i)
        {
            parseArg(argv[i]);
        }
    }

    void ArgParser::parseArg(const std::string& arg)
    {
        auto tokens = tokenize(arg, { '=' });
        if (tokens.size() == 1)
        {
            auto token = tokens[0];
            while (token.find('-') == 0)
                token = token.substr(1, token.length() - 1);
            m_flags.emplace_back(token);
        }
        else if (tokens.size() == 2)
        {
            auto token = tokens[0];
            while (token.find('-') == 0)
                token = token.substr(1, token.length() - 1);

            m_values[token] = tokens[1];
        }
        else
        {
            LOG_ERROR("weird argument: %s", arg.c_str());
        }
    }

    std::string ArgParser::value(const std::string& key)
    {
        if(m_values.find(key) != m_values.end())
            return m_values[key];
        return "";
    }

    bool ArgParser::flag(const std::string& flag) const
    {
        for (auto&& f : m_flags)
        {
            if (f == flag)
                return true;
        }
        return false;
    }

    const std::vector<std::string>& ArgParser::flags() const
    {
        return m_flags;
    }
}
