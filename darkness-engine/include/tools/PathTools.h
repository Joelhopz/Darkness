#pragma once

#include "tools/StringTools.h"
#include "tools/Debug.h"

namespace engine
{
    bool pathExists(const std::string& path);
    bool pathIsFolder(const std::string& path);
    bool pathIsFile(const std::string& path);
    bool pathStartsWithDelimiter(const std::string& path);
    bool pathEndsWithDelimiter(const std::string& path);

    std::vector<char> pathDelimiters();
    char pathPreferredDelim();
    char pathPreferredDelim(const std::string& path);
    std::string pathExtractFolder(const std::string& filepath, bool withLastDelimiter = false);
    std::string pathExtractFilename(const std::string& filepath);
    std::string pathExtractFilenameWithoutExtension(const std::string& filepath);
    std::string pathExtractExtension(const std::string& filepath);
    std::string pathJoin(const std::string& pathA, const std::string& pathB, char delimiter, bool withLastDelimiter = false);
    std::string pathJoin(const std::string& pathA, const std::string& pathB, bool withLastDelimiter = false);
    std::string pathJoin(std::vector<std::string> parts, char delimiter = pathDelimiters()[0], bool withLastDelimiter = false);
    std::vector<std::string> pathSplit(const std::string& path);
    std::string pathReplaceExtension(const std::string& path, const std::string& newExtension);
}

