#pragma once

#include "engine/resources/ResourceCommon.h"
#include <string>
#include <vector>

namespace engine
{
    struct CopyJob
    {
        std::string from;
        std::string to;
    };

    struct ExpandedFilePath
    {
        std::string absolutePath;
        std::string relativePath;
    };

    class ResourceDropHandler
    {
    public:
        ResourceDropHandler(const std::string& contentPath, const std::string& processedPath);
        ProcessResourcePackage handleDrop(const std::vector<std::string>& sourceFilepaths, const std::string& destinationPath, unsigned int preferredEncodingFormat, bool generateMips, bool performCopy = true);
    private:
        std::string m_contentPath;
        std::string m_processedPath;
        std::string m_commonAncestor;

        std::vector<ExpandedFilePath> expandFilepaths(
            const std::vector<std::string>& sourceFilepaths,
            const std::string& destinationPath);
        std::vector<std::string> recursiveGather(const std::string& path);
        std::vector<CopyJob> copyJobs(const std::vector<ExpandedFilePath>& sourceFilepaths);

        bool isAssetUnderContent(const std::string& path);
        
        std::string pathUnderContent(const ExpandedFilePath& path);
        std::string pathUnderProcessed(const ExpandedFilePath& path);
        
        void performCopys(const std::vector<CopyJob>& jobs);
    };
}
