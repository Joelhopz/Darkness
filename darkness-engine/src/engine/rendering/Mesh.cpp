#include "engine/rendering/Mesh.h"
#include "tools/Debug.h"

using namespace std;

namespace engine
{
    MeshVersion getMeshVersion(const std::string& filename)
    {
        CompressedFile file;
        file.open(filename, ios::in | ios::binary);
        if (file.is_open())
        {
            MeshVersion version;
            file.read(reinterpret_cast<char*>(&version), sizeof(MeshVersion));
            file.close();
            return version;
        }
        return MeshVersion{};
    }

    Mesh::Mesh()
    {
    }

    Mesh::Mesh(const string& filename)
        : m_filename{ filename }
    {
        if (getMeshVersion(filename) == SupportedMeshVersion)
        {
            load();
        }
    }

    void Mesh::load()
    {
        CompressedFile file;
        file.open(m_filename, ios::in | ios::binary);
        if (file.is_open())
        {
            MeshVersion version;
            file.read(reinterpret_cast<char*>(&version), sizeof(MeshVersion));

            while (!file.eof())
            {
                m_subMeshes.emplace_back(SubMesh());
                if (!m_subMeshes[m_subMeshes.size() - 1].load(file))
                {
                    m_subMeshes.erase(m_subMeshes.end() - 1);
                }
            }

            file.close();
        }
    }

    void Mesh::save()
    {
        CompressedFile file;
        file.open(m_filename, ios::out | ios::binary);
        if (file.is_open())
        {
            file.write(reinterpret_cast<const char*>(&SupportedMeshVersion), sizeof(MeshVersion));
            for (const auto& mesh : m_subMeshes)
            {
                mesh.save(file);
            }

            file.close();
        }
    }

    void Mesh::setFilename(const std::string& filename)
    {
        m_filename = filename;
    }

    std::vector<SubMesh>& Mesh::subMeshes()
    {
        return m_subMeshes;
    }

    const std::vector<SubMesh>& Mesh::subMeshes() const
    {
        return m_subMeshes;
    }

}
