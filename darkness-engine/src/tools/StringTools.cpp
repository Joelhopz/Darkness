#include "tools/StringTools.h"

namespace engine
{
    std::vector<std::string> tokenize(std::string str, const std::vector<char>& delims)
    {
        std::vector<std::string> result;
        for_each_token(
            std::cbegin(str), std::cend(str),
            std::cbegin(delims), std::cend(delims),
            [&result](auto first, auto second)
        {
            if (first != second)
                result.emplace_back(std::string(first, second));
        }
        );
        return result;
    }

    std::string join(const std::vector<std::string>& strings)
    {
        std::string result;
        for (auto&& str : strings)
            result += str;
        return result;
    }

    std::string join(const std::vector<std::string>& strings, const char delim)
    {
        std::string result;
        auto size = strings.size();
        for (int i = 0; i < size - 1; ++i)
        {
            result += strings[i];
            result += delim;
        }
        result += strings[size - 1];
        return result;
    }
}
