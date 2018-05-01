#include "tools/Clusterize.h"
#include "tools/Debug.h"
#include <stack>

using namespace std;

namespace engine
{
    std::vector<uint32_t> Clusterize::clusterize(const vector<Vector3f>& vertex, const vector<uint32_t>& index)
    {
        ASSERT(index.size() % 3 == 0, "Invalid index count. Needs to be divisible with 3");

        std::vector<uint32_t> outputIndexes;

        generateTriangleList(vertex, index);
        buildAdjacency(vertex);

        auto Neighbors = [&](unordered_map<uint32_t, vector<uint32_t>>& A, uint32_t f)->vector<Triangle>
        {
            vector<Triangle> res;
            for (auto&& triIndex : A[f])
            {
                res.emplace_back(m_triangles[triIndex]);
            }
            return res;
        };

        auto getNextVertex = [](
            const vector<uint32_t>& I,
            int i,
            int k,
            vector<uint32_t>& N,
            vector<uint32_t>& C,
            int s,
            vector<uint32_t>& L,
            stack<uint32_t>& D
            )->int
        {

            auto skipDeadEnd = [](
                vector<uint32_t>& L,
                stack<uint32_t>& D,
                const vector<uint32_t>& I,
                int i
                )->int
            {
                while (D.size() > 0)
                {
                    uint32_t d = D.top(); D.pop();
                    if (L[d] > 0)
                    {
                        return d;
                    }
                }

                while (i < I.size())
                {
                    i = i + 1;
                    if (i < L.size() && L[i] > 0)
                        return i;
                }
                return -1;
            };

            int n = -1;
            int p = -1;
            int m = 0; // m position in cache?
            for (auto&& vertex : N)
            {
                if (L[vertex] > 0)
                {
                    p = 0;
                    if (s - static_cast<int>(C[vertex]) + 2 * static_cast<int>(L[vertex]) <= k)
                    {
                        p = s - C[vertex];
                    }
                    if (p > m)
                    {
                        m = p;
                        n = vertex;
                    }
                }
            }
            if (n == -1)
            {
                n = skipDeadEnd(L, D, I, i);
            }
            return n;
        };

        const vector<uint32_t>& I = index;
        unordered_map<uint32_t, vector<uint32_t>>& A = m_adjacency;
        vector<uint32_t> L(vertex.size());
        for (int i = 0; i < vertex.size(); ++i)
        {
            L[i] = static_cast<uint32_t>(m_adjacency[i].size());
        }
        vector<uint32_t> C(vertex.size(), 0);
        stack<uint32_t> D;
        vector<uint32_t>& O = outputIndexes;
        int f = 0;
        int k = 192;
        int s = k + 1;
        int i = 1;
        
        while (f >= 0)
        {
            std::vector<uint32_t> N;
            
            //for (auto&& triangle : Neighbors(A, f))
            auto& adj = A[f];
            for(int a = 0; a < adj.size(); ++a)
            {
                Triangle& triangle = m_triangles[adj[a]];
                if(!triangle.emitted)
                {
                    for (auto&& vert : { triangle.index[0], triangle.index[1], triangle.index[2] })
                    {
                        O.emplace_back(vert);
                        D.push(vert);
                        N.emplace_back(vert);
                        --L[vert];
                        if (s - static_cast<int>(C[vert]) > k)
                        {
                            C[vert] = s;
                            s = s + 1;
                        }
                    }
                    triangle.emitted = true;
                }
            }

            f = getNextVertex(I, i, k, N, C, s, L, D);
        }
        return outputIndexes;
    }

    std::vector<uint32_t> Clusterize::clusterize2(const vector<Vector3f>& vertex, const vector<uint32_t>& index)
    {
        ASSERT(index.size() % 3 == 0, "Invalid index count. Needs to be divisible with 3");

        generateTriangleList(vertex, index);
        buildAdjacency(vertex);

        auto faceDistance = [&](uint32_t faceA, uint32_t faceB)->float
        {
            Triangle& triA = m_triangles[faceA];
            Vector3f aa = vertex[triA.index[0]];
            Vector3f ab = vertex[triA.index[1]];
            Vector3f ac = vertex[triA.index[2]];
            Vector3f faceACenter = (aa + ab + ac) / 3.0f;

            Triangle& triB = m_triangles[faceB];
            Vector3f ba = vertex[triB.index[0]];
            Vector3f bb = vertex[triB.index[1]];
            Vector3f bc = vertex[triB.index[2]];
            Vector3f faceBCenter = (ba + bb + bc) / 3.0f;

            return (faceACenter - faceBCenter).magnitude();
        };
        auto getAnyNotEmittedFace = [&]()->int
        {
            for (int i = 0; i < m_triangles.size(); ++i)
                if (!m_triangles[i].emitted)
                    return i;
            return -1;
        };
        auto getClosestNotEmittedFace = [&](uint32_t face)->int
        {
            float bestDistance = std::numeric_limits<float>::max();
            int bestIndex = -1;
            for(int i = 0; i < m_triangles.size(); ++i)
            {
                if (i != static_cast<int>(face))
                {
                    auto distance = faceDistance(face, i);
                    if (distance < bestDistance && !m_triangles[i].emitted)
                    {
                        bestDistance = distance;
                        bestIndex = i;
                    }
                }
            }
            return bestIndex;
        };
        auto emitFace = [&](vector<uint32_t>& output, uint32_t face)
        {
            for (auto&& faceIndex : { m_triangles[face].index[0], m_triangles[face].index[1], m_triangles[face].index[2] })
            {
                output.emplace_back(faceIndex);
            }
        };

        vector<uint32_t> output;
        while (output.size() < index.size())
        {
            vector<uint32_t> cluster;
            auto face = getAnyNotEmittedFace();
            auto initialFace = face;
            if (face == -1)
                break;
            while (cluster.size() < 192)
            {
                emitFace(cluster, face);
                m_triangles[face].emitted = true;

                if (face != initialFace)
                {
                    m_emittedTriangles.emplace_back(m_triangles[face]);
                    m_triangles.erase(m_triangles.begin() + face);
                }

                face = getClosestNotEmittedFace(initialFace);
                if (face == -1)
                    break;
            }
            m_emittedTriangles.emplace_back(m_triangles[initialFace]);
            m_triangles.erase(m_triangles.begin() + initialFace);

            output.insert(output.end(), cluster.begin(), cluster.end());
        }
        return output;
    }

    void Clusterize::cleanupUnused(
        const std::vector<Vector3f>& srcVertex,
        const std::vector<uint32_t>& srcIndex,
        std::vector<Vector3f>& dstVertex,
        std::vector<uint32_t>& dstIndex)
    {
        vector<uint32_t> used(srcVertex.size(), 0);
        for (auto&& srcind : srcIndex)
        {
            ++used[srcind];
        }

        int delta = 0;
        int srcindex = 0;
        for (auto&& u : used)
        {
            if (u == 0)
                ++delta;
            else
            {
                u = delta;
                dstVertex.emplace_back(srcVertex[srcIndex[srcindex]]);
            }
            ++srcindex;
        }

        delta = 0;
        int dstindex = 0;
        for(auto&& srcInd : srcIndex)
        {
            dstIndex[dstindex] = srcInd - used[dstindex];
            ++dstindex;
        }
    }

    std::vector<uint32_t> Clusterize::clusterize3(const vector<Vector3f>& vertex, const vector<uint32_t>& index)
    {
        ASSERT(index.size() % 3 == 0, "Invalid index count. Needs to be divisible with 3");

        /*vector<Vector3f> cleanVertex;
        vector<uint32_t> cleanIndex;
        cleanupUnused(vertex, index, cleanVertex, cleanIndex);*/

        generateTriangleList(vertex, index);
        buildAdjacency(vertex);

        auto faceCenter = [&](uint32_t face)->Vector3f
        {
            Triangle& tri = m_triangles[face];
            Vector3f aa = vertex[tri.index[0]];
            Vector3f ab = vertex[tri.index[1]];
            Vector3f ac = vertex[tri.index[2]];
            return (aa + ab + ac) / 3.0f;
        };

        for (uint32_t i = 0; i < m_triangles.size(); ++i)
        {
            auto face = faceCenter(i);
            if (face.x < m_meshBoundingBox.min.x) m_meshBoundingBox.min.x = face.x;
            if (face.y < m_meshBoundingBox.min.y) m_meshBoundingBox.min.y = face.y;
            if (face.z < m_meshBoundingBox.min.z) m_meshBoundingBox.min.z = face.z;
            if (face.x > m_meshBoundingBox.max.x) m_meshBoundingBox.max.x = face.x;
            if (face.y > m_meshBoundingBox.max.y) m_meshBoundingBox.max.y = face.y;
            if (face.z > m_meshBoundingBox.max.z) m_meshBoundingBox.max.z = face.z;
        }

        m_octree = make_unique<Octree<int>>(m_meshBoundingBox);
        Octree<int>& oc = *m_octree;
        /*for (int i = 0; i < vertex.size(); ++i)
        {
            oc.insert(vertex[i]);
        }*/

        

        for(uint32_t i = 0; i < m_triangles.size(); ++i)
        {
            oc.insert(faceCenter(i), i);
        }

        auto faceDistance = [&](uint32_t faceA, uint32_t faceB)->float
        {
            Vector3f faceACenter = faceCenter(faceA);
            Vector3f faceBCenter = faceCenter(faceB);

            return (faceACenter - faceBCenter).magnitude();
        };
        auto getAnyNotEmittedFace = [&]()->int
        {
            for (int i = 0; i < m_triangles.size(); ++i)
                if (!m_triangles[i].emitted)
                    return i;
            return -1;
        };
        auto getClosestNotEmittedFace = [&](uint32_t face)->int
        {
            return m_octree->getClosestPayload(faceCenter(face));
        };
        auto emitFace = [&](vector<uint32_t>& output, uint32_t face)
        {
            for (auto&& faceIndex : { m_triangles[face].index[0], m_triangles[face].index[1], m_triangles[face].index[2] })
            {
                output.emplace_back(faceIndex);
            }
        };

        vector<uint32_t> output;
        while (output.size() < index.size())
        {
            vector<uint32_t> cluster;
            auto face = getAnyNotEmittedFace();
            auto initialFace = face;
            if (face == -1)
                break;
            while (cluster.size() < 192)
            {
                emitFace(cluster, face);
                m_triangles[face].emitted = true;

                if (face != initialFace)
                {
                    m_octree->erase(faceCenter(face), face);
                }

                face = getClosestNotEmittedFace(initialFace);
                if (face == -1)
                    break;
            }
            m_octree->erase(faceCenter(initialFace), initialFace);
            //m_emittedTriangles.emplace_back(m_triangles[initialFace]);
            //m_triangles.erase(m_triangles.begin() + initialFace);

            output.insert(output.end(), cluster.begin(), cluster.end());
        }
        return output;
    }

    void Clusterize::generateTriangleList(const std::vector<Vector3f>& vertex, const std::vector<uint32_t>& index)
    {
        m_triangles.clear();
        m_triangles.resize(index.size() / 3);
        uint32_t triId = 0;
        m_meshBoundingBox.min = Vector3f(numeric_limits<float>::max(), numeric_limits<float>::max(), numeric_limits<float>::max());
        m_meshBoundingBox.max = Vector3f(numeric_limits<float>::lowest(), numeric_limits<float>::lowest(), numeric_limits<float>::lowest());
        for(uint32_t i = 0; i < index.size(); i += 3, ++triId)
        {
            m_triangles[triId] = Triangle{ index[i], index[i+1], index[i+2], false };

            for (int a = 0; a < 3; ++a)
            {
                const Vector3f& vec = vertex[index[i + a]];
                if (vec.x < m_meshBoundingBox.min.x) m_meshBoundingBox.min.x = vec.x;
                if (vec.y < m_meshBoundingBox.min.y) m_meshBoundingBox.min.y = vec.y;
                if (vec.z < m_meshBoundingBox.min.z) m_meshBoundingBox.min.z = vec.z;
                if (vec.x > m_meshBoundingBox.max.x) m_meshBoundingBox.max.x = vec.x;
                if (vec.y > m_meshBoundingBox.max.y) m_meshBoundingBox.max.y = vec.y;
                if (vec.z > m_meshBoundingBox.max.z) m_meshBoundingBox.max.z = vec.z;
            }
        }
    }

    void Clusterize::buildAdjacency(const std::vector<Vector3f>& vertex)
    {
        m_meshBoundingBox.min = Vector3f(numeric_limits<float>::max(), numeric_limits<float>::max(), numeric_limits<float>::max());
        m_meshBoundingBox.max = Vector3f(numeric_limits<float>::lowest(), numeric_limits<float>::lowest(), numeric_limits<float>::lowest());

        for(uint32_t i = 0; i < m_triangles.size(); ++i)
        {
            Triangle& tri = m_triangles[i];
            m_adjacency[tri.index[0]].emplace_back(i);
            m_adjacency[tri.index[1]].emplace_back(i);
            m_adjacency[tri.index[2]].emplace_back(i);

            for (int a = 0; a < 3; ++a)
            {
                const Vector3f& vec = vertex[tri.index[a]];
                if (vec.x < m_meshBoundingBox.min.x) m_meshBoundingBox.min.x = vec.x;
                if (vec.y < m_meshBoundingBox.min.y) m_meshBoundingBox.min.y = vec.y;
                if (vec.z < m_meshBoundingBox.min.z) m_meshBoundingBox.min.z = vec.z;
                if (vec.x > m_meshBoundingBox.max.x) m_meshBoundingBox.max.x = vec.x;
                if (vec.y > m_meshBoundingBox.max.y) m_meshBoundingBox.max.y = vec.y;
                if (vec.z > m_meshBoundingBox.max.z) m_meshBoundingBox.max.z = vec.z;
            }
        }
    }
}
