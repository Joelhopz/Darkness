#pragma once

#include "engine/primitives/Vector3.h"
#include "engine/primitives/BoundingBox.h"
#include "tools/Octree.h"
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>

namespace engine
{
    struct Triangle
    {
        uint32_t index[3];
        bool emitted;
    };

    class Clusterize
    {
    public:
        std::vector<uint32_t> clusterize(const std::vector<Vector3f>& vertex, const std::vector<uint32_t>& index);
        std::vector<uint32_t> clusterize2(const std::vector<Vector3f>& vertex, const std::vector<uint32_t>& index);
        std::vector<uint32_t> clusterize3(const std::vector<Vector3f>& vertex, const std::vector<uint32_t>& index);
    private:
        void generateTriangleList(const std::vector<Vector3f>& vertex, const std::vector<uint32_t>& index);
        void buildAdjacency(const std::vector<Vector3f>& vertex);

        std::vector<Triangle> m_triangles;
        std::vector<Triangle> m_emittedTriangles;
        std::unordered_map<uint32_t, std::vector<uint32_t>> m_adjacency;

        BoundingBox m_meshBoundingBox;
        std::unique_ptr<Octree<int>> m_octree;

        void cleanupUnused(
            const std::vector<Vector3f>& srcVertex,
            const std::vector<uint32_t>& srcIndex,
            std::vector<Vector3f>& dstVertex,
            std::vector<uint32_t>& dstIndex);
    };
}
