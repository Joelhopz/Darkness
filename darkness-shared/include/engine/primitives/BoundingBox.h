#pragma once

#include "engine/graphics/HLSLTypeConversions.h"
#include "shaders/core/shared_types/BoundingBox.hlsli"
#include "engine/primitives/Vector3.h"

namespace engine
{
    class BoundingBox : public ::BoundingBox
    {
    public:
        BoundingBox() = default;
        BoundingBox(const Vector3f& _min, const Vector3f& _max)
            : ::BoundingBox{ _min, _max }
        {}

        BoundingBox(const Vector3f& center, float ray)
            : ::BoundingBox{ 
                Vector3f{ center.x - ray, center.y - ray, center.z - ray },
                Vector3f{ center.x + ray, center.y + ray, center.z + ray } }
        {}

        BoundingBox bb_union(const BoundingBox& b)
        {
            return BoundingBox{
                Vector3f{
                    min.x < b.min.x ? min.x : b.min.x,
                    min.y < b.min.y ? min.y : b.min.y,
                    min.z < b.min.z ? min.z : b.min.z },
                Vector3f{
                    max.x > b.max.x ? max.x : b.max.x,
                    max.y > b.max.y ? max.y : b.max.y,
                    max.z > b.max.z ? max.z : b.max.z }
            };
        }

        BoundingBox bb_intersection(const BoundingBox& b)
        {
            return BoundingBox{
                Vector3f{
                    min.x < b.min.x ? b.min.x : min.x,
                    min.y < b.min.y ? b.min.y : min.y,
                    min.z < b.min.z ? b.min.z : min.z },
                Vector3f{
                    max.x > b.max.x ? b.max.x : max.x,
                    max.y > b.max.y ? b.max.y : max.y,
                    max.z > b.max.z ? b.max.z : max.z }
            };
        }

        bool valid() const
        {
            return
                (min.x <= max.x) &&
                (min.y <= max.y) &&
                (min.z <= max.z);
        }

        bool contains(const Vector3f& point) const
        {
            return
                (point.x >= min.x) &&
                (point.x <= max.x) &&
                (point.y >= min.y) &&
                (point.y <= max.y) &&
                (point.z >= min.z) &&
                (point.z <= max.z);
        }

        bool contains(const BoundingBox& bb) const
        {
            return
                (bb.min.x >= min.x) &&
                (bb.min.y >= min.y) &&
                (bb.min.z >= min.z) &&
                (bb.max.x <= max.x) &&
                (bb.max.y <= max.y) &&
                (bb.max.z <= max.z);
        }

        BoundingBox operator-(const Vector3f& val)
        {
            BoundingBox res = *this;
            res.min.x -= val.x;
            res.min.y -= val.y;
            res.min.z -= val.z;
            res.max.x -= val.x;
            res.max.y -= val.y;
            res.max.z -= val.z;
            return res;
        }

        BoundingBox operator+(const Vector3f& val)
        {
            BoundingBox res = *this;
            res.min.x += val.x;
            res.min.y += val.y;
            res.min.z += val.z;
            res.max.x += val.x;
            res.max.y += val.y;
            res.max.z += val.z;
            return res;
        }

        BoundingBox& operator-=(const Vector3f& val)
        {
            min.x -= val.x;
            min.y -= val.y;
            min.z -= val.z;
            max.x -= val.x;
            max.y -= val.y;
            max.z -= val.z;
            return *this;
        }

        BoundingBox& operator+=(const Vector3f& val)
        {
            min.x += val.x;
            min.y += val.y;
            min.z += val.z;
            max.x += val.x;
            max.y += val.y;
            max.z += val.z;
            return *this;
        }

        float width() const
        {
            return max.x - min.x;
        }

        float height() const
        {
            return max.y - min.y;
        }

        float depth() const
        {
            return max.z - min.z;
        }

        Vector3f center() const
        {
            return Vector3f{
                min.x + (width() / 2.0f),
                min.y + (height() / 2.0f),
                min.z + (depth() / 2.0f)
            };
        }
    };
}
