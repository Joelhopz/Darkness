#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include "tools/Debug.h"

namespace engine
{
    static const char DefaultDelimiters[] = { ":" };
    std::vector<std::string> tokenize(
        std::string str, 
        const std::vector<char>& delims = std::vector<char>(
            DefaultDelimiters, 
            DefaultDelimiters + sizeof(DefaultDelimiters) / sizeof(DefaultDelimiters[0])));

    template <class InputIt, class ForwardIt, class BinOp>
    void for_each_token(InputIt first, InputIt last, ForwardIt s_first, ForwardIt s_last, BinOp binaryOp)
    {
        while (first != last)
        {
            const auto pos = std::find_first_of(first, last, s_first, s_last);
            binaryOp(first, pos);
            if (pos == last) break;
            first = std::next(pos);
        }
    }

    std::string join(const std::vector<std::string>& strings);
    std::string join(const std::vector<std::string>& strings, const char delim);

    template<typename T>
    T stringToNumber(const std::string& value)
    {
        T result;
        std::stringstream stream(value);
        stream >> result;
        ASSERT(!stream.fail());
        return result;
    }
    
    std::wstring toWideString(const std::string& str);
    std::string toUtf8String(const std::wstring& str);
}
