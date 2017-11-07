#include "tools/MeshTools.h"
#include "tools/Debug.h"
#include <unordered_map>
#include <cmath>
#include <memory>

using namespace std;

namespace engine
{
    namespace adjacency_impl
    {
        struct VertexHashEntry
        {
            Vector3f v;
            uint32_t index;
            VertexHashEntry* next;
        };

        struct EdgeHashEntry
        {
            uint32_t v1;
            uint32_t v2;
            uint32_t vOther;
            uint32_t face;
            EdgeHashEntry* next;
        };

        constexpr uint32_t UNUSED32 = uint32_t(-1);

        vector<uint32_t> makeHeap(const vector<Vector3f>& positions)
        {
            auto index = vector<uint32_t>(positions.size());
            for(uint32_t i = 0; i < static_cast<uint32_t>(index.size()); ++i)
            {
                index[i] = i;
            }

            if (positions.size() > 0)
            {
                uint32_t iulLim = static_cast<uint32_t>(positions.size());

                for (uint32_t vert = static_cast<uint32_t>(positions.size() >> 1); --vert != -1;)
                {
                    uint32_t iulI = vert;
                    uint32_t iulJ = vert + vert + 1;
                    uint32_t ulT = index[iulI];

                    while (iulJ < iulLim)
                    {
                        uint32_t ulJ = index[iulJ];

                        if (iulJ + 1 < iulLim)
                        {
                            uint32_t ulJ1 = index[iulJ + 1];
                            if (positions[ulJ1].x <= positions[ulJ].x)
                            {
                                ++iulJ;
                                ulJ = ulJ1;
                            }
                        }

                        if (positions[ulJ].x > positions[ulT].x)
                            break;

                        index[iulI] = index[iulJ];
                        iulI = iulJ;
                        iulJ += iulJ + 1;
                    }

                    index[iulI] = ulT;
                }

                // sort
                while (--iulLim != -1)
                {
                    uint32_t ulT = index[iulLim];
                    index[iulLim] = index[0];

                    // percolate down
                    uint32_t iulI = 0;
                    uint32_t iulJ = 1;

                    while (iulJ < iulLim)
                    {
                        uint32_t ulJ = index[iulJ];

                        if (iulJ + 1 < iulLim)
                        {
                            uint32_t ulJ1 = index[iulJ + 1];
                            if (positions[ulJ1].x <= positions[ulJ].x)
                            {
                                ++iulJ;
                                ulJ = ulJ1;
                            }
                        }

                        if (positions[ulJ].x > positions[ulT].x)
                            break;

                        index[iulI] = index[iulJ];
                        iulI = iulJ;
                        iulJ += iulJ + 1;
                    }

                    ASSERT(iulI < positions.size(), "adjacency heap create failed");
                    index[iulI] = ulT;
                }
            }

            return index;
        }
        
        template <class index_t>
        vector<uint32_t> generatePointReps(
            const vector<uint32_t>& indices,
            const vector<Vector3f>& positions,
            float epsilon)
        {
            auto faces = indices.size() / 3;
            auto verts = positions.size();

            vector<uint32_t> result(verts);
            vector<uint32_t> temp(verts + faces * 3);

            uint32_t* vertexToCorner = &temp[0];
            uint32_t* vertexCornerList = &temp[verts];

            memset(vertexToCorner, 0xff, sizeof(uint32_t) * verts);
            memset(vertexCornerList, 0xff, sizeof(uint32_t) * faces * 3);

            // build initial lists and validate indices
            for (size_t j = 0; j < (faces * 3); ++j)
            {
                index_t k = indices[j];
                if (k == index_t(-1))
                    continue;

                ASSERT(k < verts, "Problem generating point reps");

                vertexCornerList[j] = vertexToCorner[k];
                vertexToCorner[k] = uint32_t(j);
            }

            if (epsilon == 0.f)
            {
                size_t hashSize = verts / 3;

                vector<VertexHashEntry*> hashTable(hashSize);
                memset(&hashTable[0], 0, sizeof(VertexHashEntry*) * hashSize);

                vector<VertexHashEntry> hashEntries(verts);

                uint32_t freeEntry = 0;

                for (size_t vert = 0; vert < verts; ++vert)
                {
                    uint32_t hashKey = (
                        *reinterpret_cast<const uint32_t*>(&positions[vert].x) +
                        *reinterpret_cast<const uint32_t*>(&positions[vert].y) +
                        *reinterpret_cast<const uint32_t*>(&positions[vert].z)
                        ) % hashSize;
                    
                    uint32_t found = UNUSED32;

                    for (auto current = hashTable[hashKey]; current != 0; current = current->next)
                    {
                        if (current->v.x == positions[vert].x &&
                            current->v.x == positions[vert].y &&
                            current->v.x == positions[vert].z)
                        {
                            uint32_t head = vertexToCorner[vert];
                            bool ispresent = false;

                            while (head != UNUSED32)
                            {
                                uint32_t face = head / 3;
                                ASSERT(face < faces, "oops in the adjacency code");
                                ASSERT((indices[face * 3] == vert) || (indices[face * 3 + 1] == vert) || (indices[face * 3 + 2] == vert), "another oops in adjacency code");

                                if (
                                    (indices[face * 3] == current->index) ||
                                    (indices[face * 3 + 1] == current->index) ||
                                    (indices[face * 3 + 2] == current->index))
                                {
                                    ispresent = true;
                                    break;
                                }

                                head = vertexCornerList[head];
                            }

                            if (!ispresent)
                            {
                                found = current->index;
                                break;
                            }
                        }
                    }

                    if (found != UNUSED32)
                    {
                        result[vert] = found;
                    }
                    else
                    {
                        ASSERT(freeEntry < verts, "Adjacency oops");

                        auto newEntry = &hashEntries[freeEntry];
                        ++freeEntry;

                        newEntry->v = positions[vert];
                        newEntry->index = static_cast<uint32_t>(vert);
                        newEntry->next = hashTable[hashKey];
                        hashTable[hashKey] = newEntry;

                        result[vert] = static_cast<uint32_t>(vert);
                    }
                }

                ASSERT(freeEntry <= verts);

                return result;
            }
            else
            {
                vector<uint32_t> xorder = makeHeap(positions);
                memset(&result[0], 0xff, sizeof(uint32_t) * verts);

                Vector3f vepsilon(
                    epsilon * epsilon, 
                    epsilon * epsilon, 
                    epsilon * epsilon);
                uint32_t head = 0;
                uint32_t tail = 0;

                while (tail < verts)
                {
                    while ((head < verts) && ((positions[tail].x - positions[head].x) <= epsilon))
                    {
                        ++head;
                    }

                    uint32_t tailIndex = xorder[tail];
                    ASSERT(tailIndex < verts, "another damn ups in adjacency");

                    if (result[tailIndex] == UNUSED32)
                    {
                        result[tailIndex] = tailIndex;
                        Vector3f outer = positions[tailIndex];
                        for (size_t current = tail + 1; current < head; ++current)
                        {
                            uint32_t curIndex = xorder[current];
                            ASSERT(curIndex < verts, "yawn");

                            if (result[curIndex] == UNUSED32)
                            {
                                Vector3f inner = positions[curIndex];
                                float sq = (inner - outer).magnitude() * (inner - outer).magnitude();
                                Vector3f diff(sq, sq, sq);

                                if (diff < vepsilon)
                                {
                                    uint32_t headvc = vertexToCorner[tailIndex];
                                    bool ispresent = false;
                                    while (headvc != UNUSED32)
                                    {
                                        uint32_t face = headvc / 3;
                                        ASSERT(face < faces, "taas");
                                        ASSERT(
                                            (indices[face * 3] == tailIndex) || 
                                            (indices[face * 3 + 1] == tailIndex) || 
                                            (indices[face * 3 + 2] == tailIndex), "another oops in adjacency code");

                                        if (
                                            (indices[face * 3] == curIndex) ||
                                            (indices[face * 3 + 1] == curIndex) ||
                                            (indices[face * 3 + 2] == curIndex))
                                        {
                                            ispresent = true;
                                            break;
                                        }

                                        headvc = vertexCornerList[headvc];
                                    }

                                    if (!ispresent)
                                    {
                                        result[curIndex] = tailIndex;
                                    }
                                }
                            }
                        }
                    }
                    ++tail;
                }
                return result;
            }
        }

        template <class index_t>
        vector<uint32_t> convertPointRepsToAdjacency(
            const vector<index_t> indices,
            const vector<Vector3f>& positions,
            const vector<uint32_t>& pointRep)
        {
            auto nFaces = indices.size() / 3;
            auto nVerts = positions.size();

            vector<uint32_t> adjacency(indices.size());
            size_t hashSize = nVerts / 3;
            vector<EdgeHashEntry*> hashTable(hashSize);

            memset(&hashTable[0], 0, sizeof(EdgeHashEntry*) * hashSize);

            vector<EdgeHashEntry> hashEntries(nFaces * 3);



            uint32_t freeEntry = 0;

            // add face edges to hash table and validate indices
            for( size_t face = 0; face < nFaces; ++face )
            {
                index_t i0 = indices[ face*3 ];
                index_t i1 = indices[ face*3 + 1 ];
                index_t i2 = indices[ face*3 + 2 ];

                if ( i0 == index_t(-1)
                     || i1 == index_t(-1)
                     || i2 == index_t(-1) )
                    continue;

                /*if ( i0 >= nVerts
                     || i1 >= nVerts
                     || i2 >= nVerts )
                    return E_UNEXPECTED;*/

                uint32_t v1 = pointRep[ i0 ];
                uint32_t v2 = pointRep[ i1 ];
                uint32_t v3 = pointRep[ i2 ];

                // filter out degenerate triangles
                if ( v1 == v2 || v1 == v3 || v2 == v3 )
                    continue;

                for( uint32_t point = 0; point < 3; ++point )
                {
                    uint32_t va = pointRep[ indices[ face * 3 + point ] ];
                    uint32_t vb = pointRep[ indices[ face * 3 + ( ( point + 1 ) % 3 ) ] ];
                    uint32_t vOther = pointRep[ indices[ face * 3 + ( ( point + 2) % 3 ) ] ];

                    uint32_t hashKey = va % hashSize;

                    auto newEntry = &hashEntries[ freeEntry ];
                    ++freeEntry;

                    newEntry->v1 = va;
                    newEntry->v2 = vb;
                    newEntry->vOther = vOther;
                    newEntry->face = uint32_t( face );
                    newEntry->next = hashTable[ hashKey ];
                    hashTable[ hashKey ] = newEntry;
                }
            }

            memset( adjacency.data(), 0xff, sizeof(uint32_t) * nFaces * 3 );

            for( size_t face = 0; face < nFaces; ++face )
            {
                index_t i0 = indices[ face*3 ];
                index_t i1 = indices[ face*3 + 1 ];
                index_t i2 = indices[ face*3 + 2 ];

                // filter out unused triangles
                if ( i0 == index_t(-1)
                     || i1 == index_t(-1)
                     || i2 == index_t(-1) )
                    continue;

                uint32_t v1 = pointRep[ i0 ];
                uint32_t v2 = pointRep[ i1 ];
                uint32_t v3 = pointRep[ i2 ];

                // filter out degenerate triangles
                if ( v1 == v2 || v1 == v3 || v2 == v3 )
                    continue;

                for( uint32_t point = 0; point < 3; ++point )
                {
                    if ( adjacency[ face * 3 + point ] != UNUSED32 )
                        continue;

                    // see if edge already entered, if not then enter it
                    uint32_t va = pointRep[ indices[ face * 3 + ( ( point + 1 ) % 3 ) ] ];
                    uint32_t vb = pointRep[ indices[ face * 3 + point ] ];
                    uint32_t vOther = pointRep[ indices[ face * 3 + ( ( point + 2) % 3 ) ] ];

                    uint32_t hashKey = va % hashSize;

                    EdgeHashEntry* current = hashTable[ hashKey ];
                    EdgeHashEntry* prev = nullptr;

                    uint32_t foundFace = UNUSED32;

                    while( current != 0 )
                    {
                        if ( ( current->v2 == vb ) && ( current->v1 == va ) )
                        {
                            foundFace = current->face;
                            break;
                        }

                        prev = current;
                        current = current->next;
                    }

                    EdgeHashEntry* found = current;
                    EdgeHashEntry* foundPrev = prev;

                    float bestDiff = -2.f;

                    // Scan for additional matches
                    if ( current != 0 )
                    {
                        prev = current;
                        current = current->next;

                        // find 'better' match
                        while ( current != 0 )
                        {
                            if ( ( current->v2 == vb ) && ( current->v1 == va ) )
                            {
                                Vector3f pB1 = positions[ vb ];
                                Vector3f pB2 = positions[ va ];
                                Vector3f pB3 = positions[ vOther ];

                                Vector3f v12 = pB1 - pB2;
                                Vector3f v13 = pB1 - pB3;

                                Vector3f bnormal = v12.cross(v13).normalize();

                                if ( bestDiff == -2.f )
                                {
                                    Vector3f pA1 = positions[ found->v1 ];
                                    Vector3f pA2 = positions[ found->v2 ];
                                    Vector3f pA3 = positions[ found->vOther ];

                                    v12 = pA1 - pA2;
                                    v13 = pA1 - pA3;

                                    Vector3f anormal = v12.cross(v13).normalize();

                                    bestDiff = anormal.dot(bnormal);
                                }

                                Vector3f pA1 = positions[ current->v1 ];
                                Vector3f pA2 = positions[ current->v2 ];
                                Vector3f pA3 = positions[ current->vOther ];

                                v12 = pA1 - pA2;
                                v13 = pA1 - pA3;

                                Vector3f anormal = v12.cross(v13).normalize();

                                float diff = anormal.dot(bnormal);

                                // if face normals are closer, use new match
                                if ( diff > bestDiff )
                                {
                                    found = current;
                                    foundPrev = prev;
                                    foundFace = current->face;
                                    bestDiff = diff;
                                }
                            }

                            prev = current;
                            current = current->next;
                        }
                    }

                    if ( foundFace != UNUSED32 )
                    {
                        // remove found face from hash table
                        if ( foundPrev != 0 )
                        {
                            foundPrev->next = found->next;
                        }
                        else
                        {
                            hashTable[ hashKey ] = found->next;
                        }

                        adjacency[ face * 3 + point ] = foundFace;

                        // Check for other edge
                        uint32_t hashKey2 = vb % hashSize;

                        current = hashTable[ hashKey2 ];
                        prev = nullptr;

                        while( current != 0 )
                        {
                            if ( ( current->face == uint32_t( face ) ) && ( current->v2 == va ) && ( current->v1 == vb ) )
                            {
                                // trim edge from hash table
                                if ( prev != 0 )
                                {
                                    prev->next = current->next;
                                }
                                else
                                {
                                    hashTable[ hashKey2 ] = current->next;
                                }
                                break;
                            }

                            prev = current;
                            current = current->next;
                        }

                        // mark neighbor to point back
                        bool linked = false;

                        for( uint32_t point2 = 0; point2 < point; ++point2 )
                        {
                            if ( foundFace == adjacency[ face * 3 + point2 ] )
                            {
                                linked = true;
                                adjacency[ face * 3 + point ] = UNUSED32;
                                break;
                            }
                        }

                        if ( !linked )
                        {
                            uint32_t point2 = 0;
                            for( ; point2 < 3; ++point2 )
                            {
                                index_t k = indices[ foundFace * 3 + point2 ];
                                if ( k == index_t(-1) )
                                    continue;

                                if ( pointRep[ k ] == va )
                                    break;
                            }

                            if ( point2 < 3 )
                            {
        #ifndef NDEBUG
                                uint32_t testPoint = indices[ foundFace * 3 + ( ( point2 + 1 ) % 3 ) ];
                                testPoint = pointRep[ testPoint ];
        #endif

                                // update neighbor to point back to this face match edge
                                adjacency[ foundFace * 3 + point2 ] = uint32_t( face );
                            }
                        }
                    }
                }
            }
            return adjacency;

#if 0
            uint32_t freeEntry = 0;

            for (size_t face = 0; face < nFaces; ++face)
            {
                index_t i0 = indices[face * 3];
                index_t i1 = indices[face * 3 + 1];
                index_t i2 = indices[face * 3 + 2];

                if (i0 == index_t(-1) ||
                    i1 == index_t(-1) ||
                    i2 == index_t(-1))
                    continue;

                ASSERT(
                    i0 < nVerts &&
                    i1 < nVerts &&
                    i2 < nVerts, "oh no");

                uint32_t v1 = pointRep[i0];
                uint32_t v2 = pointRep[i1];
                uint32_t v3 = pointRep[i2];

                // filter out defenerate triangles
                if (v1 == v2 || v1 == v3 || v2 == v3)
                    continue;

                for (uint32_t point = 0; point < 3; ++point)
                {
                    uint32_t va = pointRep[indices[face * 3 + point]];
                    uint32_t vb = pointRep[indices[face * 3 + ((point + 1) % 3)]];
                    uint32_t vOther = pointRep[indices[face * 3 + ((point + 2) % 3)]];

                    uint32_t hashKey = va % hashSize;

                    ASSERT(freeEntry < (3 * nFaces), "again and again");

                    auto newEntry = &hashEntries[freeEntry];
                    ++freeEntry;

                    newEntry->v1 = va;
                    newEntry->v2 = vb;
                    newEntry->vOther = vOther;
                    newEntry->face = static_cast<uint32_t>(face);
                    newEntry->next = hashTable[hashKey];
                    hashTable[hashKey] = newEntry;
                }
            }

            ASSERT(freeEntry <= (nFaces * 3));
            memset(&result[0], 0xff, sizeof(uint32_t) * nFaces * 3);

            for (size_t face = 0; face < nFaces; ++face)
            {
                index_t i0 = indices[face * 3];
                index_t i1 = indices[face * 3 + 1];
                index_t i2 = indices[face * 3 + 2];

                // filter out unused triangles
                if (i0 == index_t(-1) ||
                    i1 == index_t(-1) ||
                    i2 == index_t(-1))
                    continue;

                ASSERT(
                    i0 < nVerts &&
                    i1 < nVerts &&
                    i2 < nVerts, "oh no");

                uint32_t v1 = pointRep[i0];
                uint32_t v2 = pointRep[i1];
                uint32_t v3 = pointRep[i2];

                // filter out defenerate triangles
                if (v1 == v2 || v1 == v3 || v2 == v3)
                    continue;

                for (uint32_t point = 0; point < 3; ++point)
                {
                    if (result[face * 3 + point] != UNUSED32)
                        continue;

                    // see if edge already entered, if not then enter it
                    uint32_t va = pointRep[indices[face * 3 + ((point + 1) % 3)]];
                    uint32_t vb = pointRep[indices[face * 3 + point]];
                    uint32_t vOther = pointRep[indices[face * 3 + ((point + 2) % 3)]];

                    uint32_t hashKey = va % hashSize;

                    EdgeHashEntry* current = hashTable[hashKey];
                    EdgeHashEntry* prev = nullptr;

                    uint32_t foundFace = UNUSED32;

                    while (current != 0)
                    {
                        if ((current->v2 == vb) && (current->v1 == va))
                        {
                            foundFace = current->face;
                            break;
                        }

                        prev = current;
                        current = current->next;
                    }

                    EdgeHashEntry* found = current;
                    EdgeHashEntry* foundPrev = prev;

                    float bestDiff = -2.f;

                    // scan for additional matches
                    if (current != 0)
                    {
                        prev = current;
                        current = current->next;

                        // find 'better' match
                        while (current != 0)
                        {
                            if ((current->v2 == vb) && (current->v1 == va))
                            {
                                Vector3f pB1 = positions[vb];
                                Vector3f pB2 = positions[va];
                                Vector3f pB3 = positions[vOther];

                                Vector3f v12 = pB1 - pB2;
                                Vector3f v13 = pB1 - pB3;

                                Vector3f bnormal = v12.cross(v13).normalize();

                                if (bestDiff == -2.f)
                                {
                                    Vector3f pA1 = positions[found->v1];
                                    Vector3f pA2 = positions[found->v2];
                                    Vector3f pA3 = positions[found->vOther];

                                    v12 = pA1 - pA2;
                                    v13 = pA1 - pA3;

                                    Vector3f anormal = v12.cross(v13).normalize();

                                    bestDiff = anormal.dot(bnormal);
                                }

                                Vector3f pA1 = positions[current->v1];
                                Vector3f pA2 = positions[current->v2];
                                Vector3f pA3 = positions[current->vOther];

                                v12 = pA1 - pA2;
                                v13 = pA1 - pA3;

                                Vector3f anormal = v12.cross(v13).normalize();

                                float diff = anormal.dot(bnormal);

                                if (diff > bestDiff)
                                {
                                    found = current;
                                    foundPrev = prev;
                                    foundFace = current->face;
                                    bestDiff = diff;
                                }
                            }

                            prev = current;
                            current = current->next;
                        }
                    }

                    if (foundFace != UNUSED32)
                    {
                        ASSERT(found != 0, "ja taas");

                        if (foundPrev != 0)
                        {
                            foundPrev->next = found->next;
                        }
                        else
                        {
                            hashTable[hashKey] = found->next;
                        }

                        ASSERT(result[face * 3 + point] == UNUSED32, "poks");
                        result[face * 3 + point] = foundFace;

                        // check for other edge
                        uint32_t hashKey2 = vb % hashSize;

                        current = hashTable[hashKey2];
                        prev = nullptr;

                        while (current != 0)
                        {
                            if ((current->face == static_cast<uint32_t>(face)) && (current->v2 == va) && (current->v1 == vb))
                            {
                                // trim edge from hash table
                                if (prev != 0)
                                {
                                    prev->next = current->next;
                                }
                                else
                                {
                                    hashTable[hashKey2] = current->next;
                                }
                                break;
                            }
                            prev = current;
                            current = current->next;
                        }

                        // mark neighbor to point back
                        bool linked = false;

                        for (uint32_t point2 = 0; point2 < point; ++point2)
                        {
                            if (foundFace == result[face * 3 + point2])
                            {
                                linked = true;
                                result[face * 3 + point] = UNUSED32;
                                break;
                            }
                        }

                        if (!linked)
                        {
                            uint32_t point2 = 0;
                            for (; point2 < 3; ++point2)
                            {
                                index_t k = indices[foundFace * 3 + point2];
                                if (k == index_t(-1))
                                    continue;

                                ASSERT(k < nVerts, "jep");

                                if (pointRep[k] == va)
                                    break;
                            }

                            if (point2 < 3)
                            {
                                ASSERT(result[foundFace * 3 + point2] == UNUSED32, "noh");

                                // update neighbor to point back to this face match edge
                                result[foundFace * 3 + point2] = static_cast<uint32_t>(face);
                            }
                        }
                    }
                }
            }

            return result;
#endif
        }

        struct AdjacencyAndPointReps
        {
            vector<uint32_t> pointReps;
            vector<uint32_t> adjacency;
        };

        AdjacencyAndPointReps generateAdjacencyAndPointReps(
            const vector<uint32_t>& indices,
            const vector<Vector3f>& positions,
            float epsilon)
        {
            AdjacencyAndPointReps result;
            result.pointReps = generatePointReps<uint32_t>(indices, positions, epsilon);
            /*result.pointReps.resize(positions.size());
            for (int i = 0; i < positions.size(); ++i)
            {
                result.pointReps[i] = i;
            }*/
            result.adjacency = convertPointRepsToAdjacency<uint32_t>(indices, positions, result.pointReps);
            return result;
        }

        template<class index_t>
        vector<uint32_t> generateGSAdjacency(const vector<index_t>& indices,
            const vector<uint32_t>& pointRep,
            const vector<uint32_t>& adjacency)
        {
            size_t nFaces = indices.size() / 3;
            size_t nVerts = pointRep.size();
            vector<uint32_t> indicesAdj(nFaces * 6);


            //if (!indices || !nFaces || !pointRep || !adjacency || !nVerts || !indicesAdj)
            //    return E_INVALIDARG;

            /*if (nVerts >= index_t(-1))
                return E_INVALIDARG;*/

            /*if (indices == indicesAdj)
            {
                // Does not support in-place conversion of the index buffer
                return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
            }*/

            /*if ((uint64_t(nFaces) * 3) >= UINT32_MAX)
                return HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW);*/

            size_t inputi = 0;
            size_t outputi = 0;

            for (size_t face = 0; face < nFaces; ++face)
            {
                for (uint32_t point = 0; point < 3; ++point)
                {
                    //assert(outputi < (nFaces * 6));
                    //_Analysis_assume_(outputi < (nFaces * 6));

                    indicesAdj[outputi] = indices[inputi];
                    ++outputi;
                    ++inputi;

                    //assert(outputi < (nFaces * 6));
                    //_Analysis_assume_(outputi < (nFaces * 6));

                    uint32_t a = adjacency[face * 3 + point];
                    if (a == UNUSED32)
                    {
                        indicesAdj[outputi] = indices[face * 3 + ((point + 2) % 3)];
                    }
                    else
                    {
                        uint32_t v1 = indices[face * 3 + point];
                        uint32_t v2 = indices[face * 3 + ((point + 1) % 3)];

                        if (v1 == index_t(-1) || v2 == index_t(-1))
                        {
                            indicesAdj[outputi] = index_t(-1);
                        }
                        else
                        {
                            /*if (v1 >= nVerts
                                || v2 >= nVerts)
                                return E_UNEXPECTED;*/

                            v1 = pointRep[v1];
                            v2 = pointRep[v2];

                            uint32_t vOther = UNUSED32;

                            // find other vertex
                            for (uint32_t k = 0; k < 3; ++k)
                            {
                                //assert(a < nFaces);
                                //_Analysis_assume_(a < nFaces);
                                uint32_t ak = indices[a * 3 + k];
                                if (ak == index_t(-1))
                                    break;

                                //if (ak >= nVerts)
                                //    return E_UNEXPECTED;

                                if (pointRep[ak] == v1)
                                    continue;

                                if (pointRep[ak] == v2)
                                    continue;

                                vOther = ak;
                            }

                            if (vOther == UNUSED32)
                            {
                                indicesAdj[outputi] = indices[face * 3 + ((point + 2) % 3)];

                            }
                            else
                            {
                                indicesAdj[outputi] = index_t(vOther);
                            }
                        }
                    }
                    ++outputi;
                }
            }

            //assert(inputi == (nFaces * 3));
            //assert(outputi == (nFaces * 6));

            return indicesAdj;
        }
    }


    uint64_t meshKey(uint32_t a, uint32_t b)
    {
        return (static_cast<uint64_t>(a) << 32) | static_cast<uint64_t>(b);
    }

    bool near(const Vector4f& a, const Vector4f& b, float limit)
    {
        return (
            (std::abs(a.x - b.x) < limit) &&
            (std::abs(a.y - b.y) < limit) &&
            (std::abs(a.z - b.z) < limit));
    }

    bool twoMatches(
        const Vector4f& a1, const Vector4f& b1, const Vector4f& c1,
        const Vector4f& a2, const Vector4f& b2, const Vector4f& c2,
        float limit
        )
    {
        return 
            near(a1, a2, limit) && near(b1, b2, limit) ||
            near(a1, b2, limit) && near(b1, a2, limit) || 

            near(b1, b2, limit) && near(c1, c2, limit) ||
            near(b1, c2, limit) && near(c1, b2, limit) ||

            near(a1, a2, limit) && near(c1, c2, limit) || 
            near(a1, c2, limit) && near(c1, a2, limit);
    }

    uint32_t findAdjacentTriangle(
        const std::vector<uint32_t>& indices, 
        const std::vector<Vector4f>& vertices, 
        uint32_t edgeIndexA, uint32_t edgeIndexB, uint32_t triangleCornerIndex)
    {
        Vector4f edgePointA = vertices[indices[edgeIndexA]];
        Vector4f edgePointB = vertices[indices[edgeIndexB]];
        Vector4f edgePointC = vertices[indices[triangleCornerIndex]];

        float limit = 0.0001f;

        for (int i = 0; i < indices.size(); i += 3)
        {
            Vector4f edgeA = vertices[indices[i]];
            Vector4f edgeB = vertices[indices[i + 1]];
            Vector4f edgeC = vertices[indices[i + 2]];

            if (!(
                ((i == edgeIndexA) && (i + 1 == edgeIndexB) && (i + 2 == triangleCornerIndex)) ||
                ((i == edgeIndexA) && (i + 1 == triangleCornerIndex) && (i + 2 == edgeIndexB)) ||
                ((i == edgeIndexB) && (i + 1 == edgeIndexA) && (i + 2 == triangleCornerIndex)) ||
                ((i == edgeIndexB) && (i + 1 == triangleCornerIndex) && (i + 2 == edgeIndexA)) ||
                ((i == triangleCornerIndex) && (i + 1 == edgeIndexA) && (i + 2 == edgeIndexB)) ||
                ((i == triangleCornerIndex) && (i + 1 == edgeIndexB) && (i + 2 == edgeIndexA))
                ))
            {
                if (near(edgePointA, edgeA, limit) && near(edgePointB, edgeB, limit))
                {
                    return i + 2;
                }
                if (near(edgePointA, edgeB, limit) && near(edgePointB, edgeA, limit))
                {
                    return i + 2;
                }

                if (near(edgePointB, edgeB, limit) && near(edgePointC, edgeC, limit))
                {
                    return i;
                }
                if (near(edgePointB, edgeC, limit) && near(edgePointC, edgeB, limit))
                {
                    return i;
                }

                if (near(edgePointA, edgeA, limit) && near(edgePointC, edgeC, limit))
                {
                    return i + 1;
                }
                if (near(edgePointA, edgeC, limit) && near(edgePointC, edgeA, limit))
                {
                    return i + 1;
                }
            }
        }
        return edgeIndexB;
    }

    /*
    template<class index_t>
    bool _GenerateGSAdjacency(
        const std::vector<index_t>& indices,
        size_t nFaces,
        _In_reads_(nVerts) const uint32_t* pointRep,
        _In_reads_(nFaces * 3) const uint32_t* adjacency, _In_ size_t nVerts,
        _Out_writes_(nFaces * 6) index_t* indicesAdj)
    {
        if (!indices || !nFaces || !pointRep || !adjacency || !nVerts || !indicesAdj)
            return E_INVALIDARG;

        if (nVerts >= index_t(-1))
            return E_INVALIDARG;

        if (indices == indicesAdj)
        {
            // Does not support in-place conversion of the index buffer
            return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
        }

        if ((uint64_t(nFaces) * 3) >= UINT32_MAX)
            return HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW);

        size_t inputi = 0;
        size_t outputi = 0;

        for (size_t face = 0; face < nFaces; ++face)
        {
            for (uint32_t point = 0; point < 3; ++point)
            {
                assert(outputi < (nFaces * 6));
                _Analysis_assume_(outputi < (nFaces * 6));

                indicesAdj[outputi] = indices[inputi];
                ++outputi;
                ++inputi;

                assert(outputi < (nFaces * 6));
                _Analysis_assume_(outputi < (nFaces * 6));

                uint32_t a = adjacency[face * 3 + point];
                if (a == UNUSED32)
                {
                    indicesAdj[outputi] = indices[face * 3 + ((point + 2) % 3)];
                }
                else
                {
                    uint32_t v1 = indices[face * 3 + point];
                    uint32_t v2 = indices[face * 3 + ((point + 1) % 3)];

                    if (v1 == index_t(-1) || v2 == index_t(-1))
                    {
                        indicesAdj[outputi] = index_t(-1);
                    }
                    else
                    {
                        if (v1 >= nVerts
                            || v2 >= nVerts)
                            return E_UNEXPECTED;

                        v1 = pointRep[v1];
                        v2 = pointRep[v2];

                        uint32_t vOther = UNUSED32;

                        // find other vertex
                        for (uint32_t k = 0; k < 3; ++k)
                        {
                            assert(a < nFaces);
                            _Analysis_assume_(a < nFaces);
                            uint32_t ak = indices[a * 3 + k];
                            if (ak == index_t(-1))
                                break;

                            if (ak >= nVerts)
                                return E_UNEXPECTED;

                            if (pointRep[ak] == v1)
                                continue;

                            if (pointRep[ak] == v2)
                                continue;

                            vOther = ak;
                        }

                        if (vOther == UNUSED32)
                        {
                            indicesAdj[outputi] = indices[face * 3 + ((point + 2) % 3)];

                        }
                        else
                        {
                            indicesAdj[outputi] = index_t(vOther);
                        }
                    }
                }
                ++outputi;
            }
        }

        assert(inputi == (nFaces * 3));
        assert(outputi == (nFaces * 6));

        return S_OK;
    }*/

    std::vector<uint32_t> meshGenerateAdjacency(const std::vector<uint32_t>& indices, const std::vector<Vector3f>& vertices)
    {
#if 0
        

#endif

#if 0
        float limit = 0.0001f;
        std::vector<uint32_t> reducedIndices = indices;
        std::vector<Vector4f> reducedVertices;
        //for (auto&& vec : vertices)
        for(int a = 0; a < vertices.size(); ++a)
        {
            bool found = false;
            int foundVertexIndex = 0;
            //for (auto&& rVec : reducedVertices)
            for(int i = 0; i < reducedVertices.size(); ++i)
            {
                if (near(vertices[a], reducedVertices[i], limit))
                {
                    found = true;
                    foundVertexIndex = i;
                    break;
                }
            }

            if (!found)
                reducedVertices.emplace_back(vertices[a]);
            else
            {
                for (int i = 0; i < reducedIndices.size(); ++i)
                {
                    if (reducedIndices[i] == a)
                    {
                        reducedIndices[i] = foundVertexIndex;
                    }
                }
            }
        }

        // build map
        std::map<uint64_t, std::vector<uint32_t>> map;
        for (int i = 0; i < reducedIndices.size(); i += 3)
        {
            auto k = meshKey(reducedIndices[i], reducedIndices[i + 1]);
            if (map.find(k) == map.end())
                map[k] = std::vector<uint32_t>();
            map[k].emplace_back(reducedIndices[i + 2]);

            k = meshKey(reducedIndices[i + 1], reducedIndices[i + 2]);
            if (map.find(k) == map.end())
                map[k] = std::vector<uint32_t>();
            map[k].emplace_back(reducedIndices[i]);

            k = meshKey(reducedIndices[i + 2], reducedIndices[i]);
            if (map.find(k) == map.end())
                map[k] = std::vector<uint32_t>();
            map[k].emplace_back(reducedIndices[i + 1]);
        }

        // build adjacency buffer
        std::vector<uint32_t> adjacencyData(reducedIndices.size() * 2);
        int adjacencyIndex = 0;
        for (int i = 0; i < reducedIndices.size(); i += 3, adjacencyIndex += 6)
        {
            adjacencyData[adjacencyIndex + 0] = reducedIndices[i];

            auto k = meshKey(reducedIndices[i + 1], reducedIndices[i]);
            if (map.find(k) != map.end())
            {
                if(map[k][0] != reducedIndices[i + 2])
                    adjacencyData[adjacencyIndex + 1] = map[k][0];
                else
                    adjacencyData[adjacencyIndex + 1] = map[k][1];
            }

            adjacencyData[adjacencyIndex + 2] = reducedIndices[i + 1];

            k = meshKey(reducedIndices[i + 2], reducedIndices[i + 1]);
            if (map.find(k) != map.end())
            {
                if (map[k][0] != reducedIndices[i])
                    adjacencyData[adjacencyIndex + 3] = map[k][0];
                else
                    adjacencyData[adjacencyIndex + 3] = map[k][1];
            }
            /*else
                adjacencyData[adjacencyIndex + 3] = findAdjacentTriangle(reducedIndices, reducedVertices, i + 2, i + 1, i);*/

            adjacencyData[adjacencyIndex + 4] = reducedIndices[i + 2];

            k = meshKey(reducedIndices[i], reducedIndices[i + 2]);
            if (map.find(k) != map.end())
            {
                if (map[k][0] != reducedIndices[i + 1])
                    adjacencyData[adjacencyIndex + 5] = map[k][0];
                else
                    adjacencyData[adjacencyIndex + 5] = map[k][1];
            }
            /*else
                adjacencyData[adjacencyIndex + 5] = findAdjacentTriangle(reducedIndices, reducedVertices, i, i + 2, i + 1);*/
        }
        return adjacencyData;
#endif

        // build map
        /*std::map<uint64_t, uint32_t> map;
        for (int i = 0; i < indices.size(); i += 3)
        {
            map[meshKey(indices[i], indices[i + 1])] = indices[i + 2];
            map[meshKey(indices[i + 1], indices[i + 2])] = indices[i];
            map[meshKey(indices[i + 2], indices[i])] = indices[i + 1];
        }

        // build adjacency buffer
        std::vector<uint32_t> adjacencyData(indices.size() * 2);
        int adjacencyIndex = 0;
        for (int i = 0; i < indices.size(); i += 3, adjacencyIndex += 6)
        {
            adjacencyData[adjacencyIndex + 0] = indices[i];
            adjacencyData[adjacencyIndex + 1] = map[meshKey(indices[i + 1], indices[i])];
            adjacencyData[adjacencyIndex + 2] = indices[i + 1];
            adjacencyData[adjacencyIndex + 3] = map[meshKey(indices[i + 2], indices[i + 1])];
            adjacencyData[adjacencyIndex + 4] = indices[i + 2];
            adjacencyData[adjacencyIndex + 5] = map[meshKey(indices[i], indices[i + 2])];
        }
        return adjacencyData;*/

        if (indices.size() < 10000)
        {
            auto adjacencyPointReps = adjacency_impl::generateAdjacencyAndPointReps(indices, vertices, 0.0001f);
            auto result = adjacency_impl::generateGSAdjacency(indices, adjacencyPointReps.pointReps, adjacencyPointReps.adjacency);
            return result;
        }
        else
        {

            std::unordered_map<uint64_t, uint32_t> map;
            for (int i = 0; i < indices.size(); i += 3)
            {
                map[meshKey(indices[i], indices[i + 1])] = indices[i + 2];
                map[meshKey(indices[i + 1], indices[i + 2])] = indices[i];
                map[meshKey(indices[i + 2], indices[i])] = indices[i + 1];
            }

            // build adjacency buffer
            std::vector<uint32_t> adjacencyData(indices.size() * 2);
            int adjacencyIndex = 0;
            for (int i = 0; i < indices.size(); i += 3, adjacencyIndex += 6)
            {
                adjacencyData[adjacencyIndex + 0] = indices[i];

                auto k = meshKey(indices[i + 1], indices[i]);
                if (map.find(k) != map.end())
                    adjacencyData[adjacencyIndex + 1] = map[k];
                else
                    adjacencyData[adjacencyIndex + 1] = 0xffffffff;

                adjacencyData[adjacencyIndex + 2] = indices[i + 1];

                k = meshKey(indices[i + 2], indices[i + 1]);
                if (map.find(k) != map.end())
                    adjacencyData[adjacencyIndex + 3] = map[k];
                else
                    adjacencyData[adjacencyIndex + 3] = 0xffffffff;

                adjacencyData[adjacencyIndex + 4] = indices[i + 2];

                k = meshKey(indices[i], indices[i + 2]);
                if (map.find(k) != map.end())
                    adjacencyData[adjacencyIndex + 5] = map[k];
                else
                    adjacencyData[adjacencyIndex + 5] = 0xffffffff;
            }
            return adjacencyData;
        }
    }
}
