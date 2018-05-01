#pragma once

#include "engine/graphics/CommonNoDep.h"
#include "engine/primitives/Vector2.h"
#include "engine/primitives/Vector3.h"
#include "engine/primitives/Vector4.h"
#include "engine/primitives/Matrix4.h"
#include "engine/primitives/BoundingBox.h"
#include <vector>

constexpr int BackBufferCount = 2;

#include "external/imgui/imgui.h"


namespace engine
{
    constexpr double HALF_PI = 1.5707963267948966192313216916397514420985;
    constexpr double PI = 3.1415926535897932384626433832795028841971;
    constexpr double TWO_PI = 6.2831853071795864769252867665590057683942;
    constexpr double ONE_DIV_PI = 0.3183098861837906715377675267450287240689;
    constexpr double FLT_EPS = 1e-10;

    struct Vertex
    {
        Vector3f pos;
        Vector3f color;
        Vector2f texCoord;

        bool operator==(const Vertex& other) const {
            return pos == other.pos && color == other.color && texCoord == other.texCoord;
        }
    };

    struct UniformBufferObject {
        Matrix4f model;
        Matrix4f view;
        Matrix4f proj;
    };

    int mipCount(int width, int height);

    std::vector<Vector4f> extractFrustumPlanes(const Matrix4f& modelViewProjection);

    struct Line
    {
        Vector2f a;
        Vector2f b;
        bool clip;
    };

    enum class CirclePlane
    {
        XY,
        XZ,
        YZ
    };

    Vector2f screen(Vector3f p, Matrix4f mvp, int width, int height);

    Line clipToScreen(
        const Vector3f& a,
        const Vector3f& b,
        const Matrix4f& mvp,
        const Vector3f& cameraPosition,
        int cameraWidth,
        int cameraHeight,
        const std::vector<Vector4f>& frustumPlanes);

    class Camera;

    void drawLine3d(
        Camera& camera,
        const std::vector<Vector4f>& frustumPlanes,
        const Matrix4f& mvp,
        ImDrawList* drawList,

        const Vector3f& a,
        const Vector3f& b,
        ImU32 color,
        float thickness);

    void drawCube3d(
        Camera& camera,
        const std::vector<Vector4f>& frustumPlanes,
        const Matrix4f& mvp,
        ImDrawList* drawList,

        const BoundingBox& box,
        ImU32 color,
        float thickness);

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
        float startAngleDegrees = 0.0f,
        float stopAngleDegrees = 360.0f,
        CirclePlane circlePlane = CirclePlane::XY);


    /*
    * Concatenate preprocessor tokens A and B without expanding macro definitions
    * (however, if invoked from a macro, macro arguments are expanded).
    */
#define CONCATENATE_NX(A, B) A ## B

    /*
    * Concatenate preprocessor tokens A and B after macro-expanding them.
    */
#define CONCATENATE(A, B) CONCATENATE_NX(A, B)

    class CommandList;

    class GpuMarker
    {
    public:
        GpuMarker(CommandList& cmd, char const* formatString, ...);
        ~GpuMarker();
    private:
        void* ptr;
    };

    class CpuMarker
    {
    public:
        CpuMarker(char const* formatString, ...);
        ~CpuMarker();
    };

#define GPU_MARKER(cmd, region, ...) engine::GpuMarker CONCATENATE(marker, __COUNTER__)(cmd, region, __VA_ARGS__)
#define CPU_MARKER(region, ...) engine::CpuMarker CONCATENATE(marker, __COUNTER__)(region, __VA_ARGS__)
}
