#include "engine/graphics/Common.h"
#include "components/Camera.h"
#include "engine/primitives/Vector3.h"
#include "engine/primitives/Vector4.h"
#include "engine/primitives/Matrix4.h"
#include <algorithm>

namespace engine
{
    int mipCount(int width, int height)
    {
        return static_cast<int>(1 + floor(log10((float)std::max(width, height)) / log10(2.0)));
    }

    std::vector<Vector4f> extractFrustumPlanes(const Matrix4f& modelViewProjection)
    {
        std::vector<Vector4f> result(6);

        // column major matrix
        // Left Plane
        // col4 + col1
        /*result[0].x = modelViewProjection[3] + modelViewProjection[0];
        result[0].y = modelViewProjection[7] + modelViewProjection[4];
        result[0].z = modelViewProjection[11] + modelViewProjection[8];
        result[0].w = modelViewProjection[15] + modelViewProjection[12];

        // Right Plane
        // col4 - col1
        result[1].x = modelViewProjection[3] - modelViewProjection[0];
        result[1].y = modelViewProjection[7] - modelViewProjection[4];
        result[1].z = modelViewProjection[11] - modelViewProjection[8];
        result[1].w = modelViewProjection[15] - modelViewProjection[12];

        // Bottom Plane
        // col4 + col2
        result[2].x = modelViewProjection[3] + modelViewProjection[1];
        result[2].y = modelViewProjection[7] + modelViewProjection[5];
        result[2].z = modelViewProjection[11] + modelViewProjection[9];
        result[2].w = modelViewProjection[15] + modelViewProjection[13];

        // Top Plane
        // col4 - col2
        result[3].x = modelViewProjection[3] - modelViewProjection[1];
        result[3].y = modelViewProjection[7] - modelViewProjection[5];
        result[3].z = modelViewProjection[11] - modelViewProjection[9];
        result[3].w = modelViewProjection[15] - modelViewProjection[13];

        // Near Plane
        // col4 + col3
        result[4].x = modelViewProjection[3] + modelViewProjection[2];
        result[4].y = modelViewProjection[7] + modelViewProjection[6];
        result[4].z = modelViewProjection[11] + modelViewProjection[10];
        result[4].w = modelViewProjection[15] + modelViewProjection[14];

        // Far Plane
        // col4 - col3
        result[5].x = modelViewProjection[3] - modelViewProjection[2];
        result[5].y = modelViewProjection[7] - modelViewProjection[6];
        result[5].z = modelViewProjection[11] - modelViewProjection[10];
        result[5].w = modelViewProjection[15] - modelViewProjection[14];*/

        /*
        0   =   0
        1   =   4
        2   =   8
        3   =   12
        4   =   1
        5   =   5
        6   =   9
        7   =   13
        8   =   2
        9   =   6
        10  =   10
        11  =   14
        12  =   3
        13  =   7
        14  =   11
        15  =   15
        */

        result[0].x = modelViewProjection[12] + modelViewProjection[0];
        result[0].y = modelViewProjection[13] + modelViewProjection[1];
        result[0].z = modelViewProjection[14] + modelViewProjection[2];
        result[0].w = modelViewProjection[15] + modelViewProjection[3];
        result[0].xyz(result[0].xyz().normalize());

        // Right Plane
        // col4 - col1
        result[1].x = modelViewProjection[12] - modelViewProjection[0];
        result[1].y = modelViewProjection[13] - modelViewProjection[1];
        result[1].z = modelViewProjection[14] - modelViewProjection[2];
        result[1].w = modelViewProjection[15] - modelViewProjection[3];
        result[1].xyz(result[1].xyz().normalize());

        // Bottom Plane
        // col4 + col2
        result[2].x = modelViewProjection[12] + modelViewProjection[4];
        result[2].y = modelViewProjection[13] + modelViewProjection[5];
        result[2].z = modelViewProjection[14] + modelViewProjection[6];
        result[2].w = modelViewProjection[15] + modelViewProjection[7];
        result[2].xyz(result[2].xyz().normalize());

        // Top Plane
        // col4 - col2
        result[3].x = modelViewProjection[12] - modelViewProjection[4];
        result[3].y = modelViewProjection[13] - modelViewProjection[5];
        result[3].z = modelViewProjection[14] - modelViewProjection[6];
        result[3].w = modelViewProjection[15] - modelViewProjection[7];
        result[3].xyz(result[3].xyz().normalize());

        // Near Plane
        // col4 + col3
        result[4].x = modelViewProjection[12] + modelViewProjection[8];
        result[4].y = modelViewProjection[13] + modelViewProjection[9];
        result[4].z = modelViewProjection[14] + modelViewProjection[10];
        result[4].w = modelViewProjection[15] + modelViewProjection[11];
        result[4].xyz(result[4].xyz().normalize());

        // Far Plane
        // col4 - col3
        result[5].x = modelViewProjection[12] - modelViewProjection[8];
        result[5].y = modelViewProjection[13] - modelViewProjection[9];
        result[5].z = modelViewProjection[14] - modelViewProjection[10];
        result[5].w = modelViewProjection[15] - modelViewProjection[11];
        result[5].xyz(result[5].xyz().normalize());

        return result;
    }

    Vector2f screen(
        Vector3f p,
        Matrix4f mvp,
        int width,
        int height)
    {
        Vector4f hp = mvp * Vector4f(p, 1.0f);
        hp /= hp.w;

        return Vector2f{
            ((hp.x + 1.0f) / 2.0f) * width,
            ((1.0f - hp.y) / 2.0f) * height };
    }

    Line clipToScreen(
        const Vector3f& a,
        const Vector3f& b,
        const Matrix4f& mvp,
        const Vector3f& cameraPosition,
        int cameraWidth,
        int cameraHeight,
        const std::vector<Vector4f>& frustumPlanes)
    {
        Vector3f clipA = a;
        Vector3f clipB = b;
        for (auto&& plane : frustumPlanes)
        {
            float aDistance = plane.xyz().normalize().dot(clipA - cameraPosition);
            float bDistance = plane.xyz().normalize().dot(clipB - cameraPosition);

            if (aDistance < 0 && bDistance < 0)
                return Line{ {},{}, true };

            if (aDistance >= 0 && bDistance >= 0)
                continue;

            float intersectionFactor = aDistance / (aDistance - bDistance);

            Vector3f intersectionPoint{
                clipA.x + intersectionFactor * (clipB.x - clipA.x),
                clipA.y + intersectionFactor * (clipB.y - clipA.y),
                clipA.z + intersectionFactor * (clipB.z - clipA.z)
            };
            if (aDistance < 0)
                clipA = intersectionPoint;
            if (bDistance < 0)
                clipB = intersectionPoint;
        }
        return Line{
            screen(clipA, mvp, cameraWidth, cameraHeight),
            screen(clipB, mvp, cameraWidth, cameraHeight),
            false };
    }

    void drawLine3d(
        Camera& camera,
        const std::vector<Vector4f>& frustumPlanes,
        const Matrix4f& mvp,
        ImDrawList* drawList,

        const Vector3f& a,
        const Vector3f& b,
        ImU32 color,
        float thickness)
    {
        auto clipped = clipToScreen(a, b, mvp, camera.position(), camera.width(), camera.height(), frustumPlanes);
        drawList->AddLine(
            ImVec2{ clipped.a.x, clipped.a.y },
            ImVec2{ clipped.b.x, clipped.b.y },
            color, thickness);
    }

    

    void drawCircle3d(
        Camera& camera,
        const std::vector<Vector4f>& frustumPlanes,
        const Matrix4f& mvp,
        const Matrix4f& modelMatrix,
        ImDrawList* drawList,
        float radius,
        int angles,
        ImU32 color,
        float thickness,
        float startAngleDegrees,
        float stopAngleDegrees,
        CirclePlane circlePlane)
    {
        std::vector<Vector4f> points(angles + 1);
        float startAngleRad = startAngleDegrees * DEG_TO_RAD;
        float stopAngleRad = stopAngleDegrees * DEG_TO_RAD;
        float angleDistanceRad = (stopAngleRad - startAngleRad);
        float angleIncrementRad = angleDistanceRad / static_cast<float>(angles);

        float currentAngle = startAngleRad;

        if (circlePlane == CirclePlane::XY)
        {
            for (int i = 0; i < angles; ++i)
            {
                Vector4f point{
                    radius * cosf(currentAngle),
                    radius * sinf(currentAngle),
                    0.0f,
                    1.0f };
                point = modelMatrix * point;
                points[i] = point;

                currentAngle += angleIncrementRad;
            }
            Vector4f point{
                radius * cosf(currentAngle),
                radius * sinf(currentAngle),
                0.0f,
                1.0f };
            point = modelMatrix * point;
            points[angles] = point;
        }
        else if (circlePlane == CirclePlane::XZ)
        {
            for (int i = 0; i < angles; ++i)
            {
                Vector4f point{
                    radius * cosf(currentAngle),
                    0.0f,
                    radius * sinf(currentAngle),
                    1.0f };
                point = modelMatrix * point;
                points[i] = point;

                currentAngle += angleIncrementRad;
            }
            Vector4f point{
                radius * cosf(currentAngle),
                0.0f,
                radius * sinf(currentAngle),
                1.0f };
            point = modelMatrix * point;
            points[angles] = point;
        }
        else if (circlePlane == CirclePlane::YZ)
        {
            for (int i = 0; i < angles; ++i)
            {
                Vector4f point{
                    0.0f,
                    radius * cosf(currentAngle),
                    radius * sinf(currentAngle),
                    1.0f };
                point = modelMatrix * point;
                points[i] = point;

                currentAngle += angleIncrementRad;
            }
            Vector4f point{
                0.0f,
                radius * cosf(currentAngle),
                radius * sinf(currentAngle),
                1.0f };
            point = modelMatrix * point;
            points[angles] = point;
        }

        for (int i = 0; i < points.size() - 1; ++i)
        {
            drawLine3d(camera, frustumPlanes, mvp, drawList, points[i].xyz(), points[i + 1].xyz(), color, thickness);
        }
    }
}
