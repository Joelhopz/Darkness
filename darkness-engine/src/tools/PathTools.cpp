#include "tools/PathTools.h"
#include "platform/Platform.h"

#include <experimental/filesystem>
using namespace std::experimental;

namespace engine
{
    std::vector<char> pathDelimiters()
    {
        return { '\\','/' };
    }

    char pathPreferredDelim()
    {
        return pathDelimiters()[0];
    }

    char pathPreferredDelim(const std::string& path)
    {
        auto delims = pathDelimiters();
        char pathDelim = delims[0];
        size_t lowestDelimIndex = path.size();
        for (auto&& delim : delims)
        {
            auto index = path.find(delim);
            if (index != std::string::npos)
            {
                if (index < lowestDelimIndex)
                {
                    pathDelim = delim;
                    lowestDelimIndex = index;
                }
            }
        }
        return pathDelim;
    }

    std::wstring toWideString(const std::string& str)
    {
#ifdef _WIN32
        auto lastError = GetLastError();
        // find out the size for target buffer
        SetLastError(0);
        auto wideCharacters = MultiByteToWideChar(
            CP_UTF8,
            MB_ERR_INVALID_CHARS,
            str.data(), -1, nullptr, 0);
        if (wideCharacters == 0)
        {
            auto err = GetLastError();
            switch (err)
            {
            case ERROR_INSUFFICIENT_BUFFER: ASSERT(false, "Could not convert filepath to widechar. Error: ERROR_INSUFFICIENT_BUFFER");
            case ERROR_INVALID_FLAGS: ASSERT(false, "Could not convert filepath to widechar. Error: ERROR_INVALID_FLAGS");
            case ERROR_INVALID_PARAMETER: ASSERT(false, "Could not convert filepath to widechar. Error: ERROR_INVALID_PARAMETER");
            case ERROR_NO_UNICODE_TRANSLATION: ASSERT(false, "Could not convert filepath to widechar. Error: ERROR_NO_UNICODE_TRANSLATION");
            }
        }

        std::vector<wchar_t> wstring(wideCharacters);

        // convert
        auto charactersWritten = MultiByteToWideChar(
            CP_UTF8,
            MB_ERR_INVALID_CHARS,
            str.data(),
            -1,  // assuming null terminated string
            wstring.data(),
            static_cast<int>(wstring.size()));
        if (charactersWritten == 0)
        {
            auto err = GetLastError();
            switch (err)
            {
            case ERROR_INSUFFICIENT_BUFFER: ASSERT(false, "Could not convert filepath to widechar. Error: ERROR_INSUFFICIENT_BUFFER");
            case ERROR_INVALID_FLAGS: ASSERT(false, "Could not convert filepath to widechar. Error: ERROR_INVALID_FLAGS");
            case ERROR_INVALID_PARAMETER: ASSERT(false, "Could not convert filepath to widechar. Error: ERROR_INVALID_PARAMETER");
            case ERROR_NO_UNICODE_TRANSLATION: ASSERT(false, "Could not convert filepath to widechar. Error: ERROR_NO_UNICODE_TRANSLATION");
            }
        }
        SetLastError(lastError);

        return wstring.data();
#else
        ASSERT(false, "toWideString is NOT IMPLEMENTED ON THIS PLATFORM");
#endif
    }

    std::string toUtf8String(const std::wstring& str)
    {
#ifdef _WIN32
        auto lastError = GetLastError();
        // find out the size for target buffer
        SetLastError(0);
        auto utf8Characters = WideCharToMultiByte(
            CP_UTF8,
            WC_ERR_INVALID_CHARS,
            str.data(), -1,
            nullptr, 0,
            nullptr, nullptr);
        if (utf8Characters == 0)
        {
            auto err = GetLastError();
            switch (err)
            {
            /*case ERROR_INSUFFICIENT_BUFFER: ASSERT(false, "Could not convert filepath to widechar. Error: ERROR_INSUFFICIENT_BUFFER");
            case ERROR_INVALID_FLAGS: ASSERT(false, "Could not convert filepath to widechar. Error: ERROR_INVALID_FLAGS");
            case ERROR_INVALID_PARAMETER: ASSERT(false, "Could not convert filepath to widechar. Error: ERROR_INVALID_PARAMETER");
            case ERROR_NO_UNICODE_TRANSLATION: ASSERT(false, "Could not convert filepath to widechar. Error: ERROR_NO_UNICODE_TRANSLATION");
            */
            case ERROR_INSUFFICIENT_BUFFER: return "";
            case ERROR_INVALID_FLAGS: return "";
            case ERROR_INVALID_PARAMETER: return "";
            case ERROR_NO_UNICODE_TRANSLATION: return "";
            }
        }

        std::vector<char> utf8str(utf8Characters);

        // convert
        auto charactersWritten = WideCharToMultiByte(
            CP_UTF8,
            WC_ERR_INVALID_CHARS,
            str.data(), -1,
            utf8str.data(), static_cast<int>(utf8str.size()),
            nullptr, nullptr);
        if (charactersWritten == 0)
        {
            auto err = GetLastError();
            switch (err)
            {
            case ERROR_INSUFFICIENT_BUFFER: ASSERT(false, "Could not convert filepath to widechar. Error: ERROR_INSUFFICIENT_BUFFER");
            case ERROR_INVALID_FLAGS: ASSERT(false, "Could not convert filepath to widechar. Error: ERROR_INVALID_FLAGS");
            case ERROR_INVALID_PARAMETER: ASSERT(false, "Could not convert filepath to widechar. Error: ERROR_INVALID_PARAMETER");
            case ERROR_NO_UNICODE_TRANSLATION: ASSERT(false, "Could not convert filepath to widechar. Error: ERROR_NO_UNICODE_TRANSLATION");
            }
        }
        SetLastError(lastError);

        return utf8str.data();
#else
        ASSERT(false, "toUtf8String is NOT IMPLEMENTED ON THIS PLATFORM");
#endif
    }

    bool pathExists(const std::string& path)
    {
        return filesystem::exists(path);
    }

    bool pathIsFolder(const std::string& path)
    {
#ifdef _WIN32
        if (pathExists(path))
        {
#ifdef UNICODE
            auto wstring = toWideString(path);
            return (GetFileAttributes(wstring.data()) &
                FILE_ATTRIBUTE_DIRECTORY) ==
                FILE_ATTRIBUTE_DIRECTORY;
#else
            return (GetFileAttributes(path.data()) &
                FILE_ATTRIBUTE_DIRECTORY) ==
                FILE_ATTRIBUTE_DIRECTORY;
#endif
        }
        else
        {
            if (pathEndsWithDelimiter(path))
                return true;
            return false;
        }
#else
        ASSERT(false, "pathIsFolder is NOT IMPLEMENTED ON THIS PLATFORM");
#endif
    }

    bool pathIsFile(const std::string& path)
    {
        return !pathIsFolder(path);
    }

    bool pathStartsWithDelimiter(const std::string& path)
    {
        for (auto&& delim : pathDelimiters())
        {
            if (path.find(delim) == 0)
                return true;
        }
        return false;
    }

    bool pathEndsWithDelimiter(const std::string& path)
    {
        for (auto&& delim : pathDelimiters())
        {
            if (path[path.size() - 1] == delim)
                return true;
        }
        return false;
    }

    std::string pathRemoveTrailingDelimiter(const std::string& path)
    {
        auto delim = pathPreferredDelim(path);
        auto tokens = tokenize(path, pathDelimiters());
        if (tokens.size() > 0)
            return pathJoin(tokens, delim);
        else
            return tokens[0];
    }

    std::string pathExtractFolder(const std::string& filepath, bool withLastDelimiter)
    {
        auto delims = pathDelimiters();
        auto tokens = tokenize(filepath, delims);
        std::string res = "";

        auto prefDelim = pathPreferredDelim(filepath);

        std::string joined = "";
        if (pathIsFile(filepath))
            joined = pathJoin(std::vector<std::string>(tokens.begin(), tokens.end() - 1), prefDelim);
        else
            joined = pathJoin(std::vector<std::string>(tokens.begin(), tokens.end()), prefDelim);

        if (pathStartsWithDelimiter(filepath))
            res += prefDelim + joined;
        else
            res += joined;
        if (withLastDelimiter)
            res += prefDelim;
        return res;
    }

    std::string pathExtractFilename(const std::string& filepath)
    {
        auto delims = pathDelimiters();
        auto tokens = tokenize(filepath, delims);
        return tokens.back();
    }

    std::string pathExtractFilenameWithoutExtension(const std::string& filepath)
    {
        auto filename = pathExtractFilename(filepath);
        auto ext = pathExtractExtension(filename);
        return filename.substr(0, filename.length() - ext.length() - 1);
    }

    std::string pathExtractExtension(const std::string& filepath)
    {
        auto filename = pathExtractFilename(filepath);
        auto tokens = tokenize(filename, { '.' });
        if (tokens.size() == 1)
            return "";
        else
            return tokens.back();
    }

    std::string pathJoin(const std::string& pathA, const std::string& pathB, bool withLastDelimiter)
    {
        auto delim = pathPreferredDelim(pathA);
        return pathJoin(pathA, pathB, delim, withLastDelimiter);
    }

    std::string pathJoin(const std::string& pathA, const std::string& pathB, char delimiter, bool withLastDelimiter)
    {
        auto pathASplit = pathSplit(pathA);
        auto pathBSplit = pathSplit(pathB);

        auto pAjoin = pathJoin(pathASplit, delimiter, withLastDelimiter);
        auto pBjoin = pathJoin(pathBSplit, delimiter, withLastDelimiter);

        if(pathStartsWithDelimiter(pathA))
            return delimiter + pathJoin({ pAjoin, pBjoin }, delimiter, withLastDelimiter);
        else
            return pathJoin({ pAjoin, pBjoin }, delimiter, withLastDelimiter);
    }

    std::string pathJoin(std::vector<std::string> parts, char delimiter, bool withLastDelimiter)
    {
        if (parts.size() == 0)
            return "";

        if (parts.size() == 1)
        {
            if (withLastDelimiter)
                return parts[0] + delimiter;
            else
                return parts[0];
        }

        std::string result = "";
        if (pathStartsWithDelimiter(parts[0]))
            result += delimiter;

        auto part = parts.begin();
        do
        {
            if (*part != "")
            {
                result += pathRemoveTrailingDelimiter(*part);
                part++;
                if (part != parts.end())
                    result += delimiter;
            }
            else
                part++;
        } while (part != parts.end());
        if (withLastDelimiter)
            result += delimiter;
        return result;
    }

    std::vector<std::string> pathSplit(const std::string& path)
    {
        return tokenize(path, pathDelimiters());
    }

    std::string pathReplaceExtension(const std::string& path, const std::string& newExtension)
    {
        auto folder = pathExtractFolder(path);
        auto filename = pathExtractFilename(path);
        auto extension = pathExtractExtension(filename);
        auto filenameWithoutExt = filename.substr(0, filename.size() - extension.size());
        return pathJoin(folder, filenameWithoutExt + newExtension);
    }

}
