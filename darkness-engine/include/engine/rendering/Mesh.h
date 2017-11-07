#pragma once

#include "tools/CompressedFile.h"
#include "SubMesh.h"
#include <vector>
#include <string>

namespace engine
{
    typedef uint32_t MeshVersion;
    MeshVersion getMeshVersion(const std::string& filename);

    class Mesh
    {
    public:
        Mesh();
        Mesh(const std::string& filename);
        static const MeshVersion SupportedMeshVersion{ 1 };

        void setFilename(const std::string& filename);
        void save();
        void load();
        std::vector<SubMesh>& subMeshes();
        const std::vector<SubMesh>& subMeshes() const;
    private:
        std::string m_filename;
        std::vector<SubMesh> m_subMeshes;
    };
}
