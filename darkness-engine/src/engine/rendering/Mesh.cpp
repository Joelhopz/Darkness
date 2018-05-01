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

    Mesh::Mesh(ModelResources& modelResources, const string& filename)
        : m_filename{ filename }
    {
        if (getMeshVersion(filename) == SupportedMeshVersion)
        {
            load(modelResources);
        }
    }

    void Mesh::load(ModelResources& modelResources)
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
                if (!m_subMeshes.back().load(modelResources, file))
                {
                    m_subMeshes.pop_back();
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

    void Mesh::saveToMemory(std::vector<char>& mem)
    {
        CompressedFile file;
        file.open(mem, ios::out | ios::binary);
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
