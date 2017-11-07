#pragma once

#include "engine/EngineComponent.h"
#include "engine/primitives/Quaternion.h"
#include "engine/primitives/Vector3.h"
#include "engine/primitives/Matrix4.h"
#include "engine/graphics/Resources.h"
#include "engine/graphics/Device.h"
#include "tools/image/Image.h"
#include "tools/Property.h"
#include "components/Transform.h"
#include "tools/hash/Hash.h"
#include <memory>

namespace engine
{
    struct ViewCornerRays
    {
        Vector3f topLeft;
        Vector3f topRight;
        Vector3f bottomLeft;
        Vector3f bottomRight;
    };

    enum class Projection
    {
        Perspective,
        Orthographic
    };

    std::string projectionToString(const Projection& projection);
    Projection stringToProjection(const std::string& projection);

    constexpr int HaltonValueCount = 16;

    class Camera : public EngineComponent
    {
        Property m_width;
        Property m_height;
        Property m_nearPlane;
        Property m_farPlane;
        Property m_fieldOfView;
        Property m_projection;
        Property m_followSpeed;
        Property m_environmentMap;
        Property m_environmentMapStrength;
        Property m_exposure;
        Property m_pbrShadingModel;

        std::shared_ptr<Transform> m_transform;
        std::shared_ptr<image::ImageIf> m_environmentMapImage;
        std::unique_ptr<TextureSRV> m_environmentMapSRV;

        std::unique_ptr<TextureSRV> m_environmentIrradianceSRV;
        std::unique_ptr<TextureSRV> m_environmentBrdfLUTSRV;
        std::unique_ptr<TextureSRV> m_environmentSpecularSRV;

        bool m_cpuDirty;
        bool m_gpuDirty;
    public:

        std::shared_ptr<EngineComponent> clone() const override
        {
            auto res = std::make_shared<engine::Camera>();
            res->name(m_name);
            res->width(width());
            res->height(height());
            res->nearPlane(nearPlane());
            res->farPlane(farPlane());
            res->fieldOfView(fieldOfView());
            res->projection(projection());
            res->followSpeed(followSpeed());
            res->environmentMapPath(environmentMapPath());
            res->environmentMapStrength(environmentMapStrength());
            res->exposure(exposure());
            res->pbrShadingModel(pbrShadingModel());
            res->smoothPosition(smoothPosition());
            res->target(target());
            return res;
        }

        Camera()
            : m_width{ this, "width", int(1024) }
            , m_height{ this, "height", int(1024) }
            , m_nearPlane{ this, "near", float(0.1f) }
            , m_farPlane{ this, "far", float(1000.0f) }
            , m_fieldOfView{ this, "fov", float(60.0f) }
            , m_projection{ this, "projection", Projection::Perspective }
            , m_followSpeed{ this, "followspeed", float(1.0f) }
            , m_environmentMap{ this, "environment map", std::string(""), [this]() { this->m_cpuDirty = true; } }
            , m_environmentMapStrength{ this, "environment strength", float(1.0f) }
            , m_exposure{ this, "exposure", 1.0f }
            , m_pbrShadingModel{ this, "PBR Shading Model", engine::ButtonToggle::NotPressed }
            , m_smoothPosition{ 0.0f, 0.0f, 0.0f }
            , m_forward{ 0.0f, 0.0f, 1.0f }
            , m_up{ 0.0f, 1.0f, 0.0f }
            , m_right{ 1.0f, 0.0f, 0.0f }
            , m_environmentMapSRV{ std::make_unique<TextureSRV>() }
            , m_environmentIrradianceSRV{ std::make_unique<TextureSRV>() }
            , m_environmentSpecularSRV{ std::make_unique<TextureSRV>() }
            , m_environmentBrdfLUTSRV{ std::make_unique<TextureSRV>() }
        {
            m_name = "Camera";
            createHaltonValues();
        }

        Camera(std::shared_ptr<Transform> transform)
            : m_width{ this, "width", int(1024) }
            , m_height{ this, "height", int(1024) }
            , m_nearPlane{ this, "near", float(0.1f) }
            , m_farPlane{ this, "far", float(1000.0f) }
            , m_fieldOfView{ this, "fov", float(60.0f) }
            , m_projection{ this, "projection", Projection::Perspective }
            , m_followSpeed{ this, "followspeed", float(1.0f) }
            , m_environmentMap{ this, "environment map", std::string(""), [this]() { this->m_cpuDirty = true; } }
            , m_environmentMapStrength{ this, "environment strength", float(1.0f) }
            , m_exposure{ this, "exposure", 1.0f }
            , m_pbrShadingModel{ this, "PBR Shading Model", engine::ButtonToggle::NotPressed }
            , m_smoothPosition{ 0.0f, 0.0f, 0.0f }
            , m_forward{ 0.0f, 0.0f, 1.0f }
            , m_up{ 0.0f, 1.0f, 0.0f }
            , m_right{ 1.0f, 0.0f, 0.0f }
            , m_transform{ transform }
            , m_environmentMapSRV{ std::make_unique<TextureSRV>() }
            , m_environmentIrradianceSRV{ std::make_unique<TextureSRV>() }
            , m_environmentSpecularSRV{ std::make_unique<TextureSRV>() }
            , m_environmentBrdfLUTSRV{ std::make_unique<TextureSRV>() }
        {
            m_name = "Camera";
            createHaltonValues();
        }

        void invalidateGpu()
        {
            m_gpuDirty = true;
        }

        void cpuRefresh(Device& device)
        {
            if (m_cpuDirty)
            {
                m_cpuDirty = false;
                if (m_environmentMap.value<std::string>() != "")
                    m_environmentMapImage = device.createImage(
                        tools::hash(m_environmentMap.value<std::string>()),
                        m_environmentMap.value<std::string>());
                else
                    m_environmentMapImage = nullptr;

                m_gpuDirty = true;
            }
        }

        void gpuRefresh(Device& device)
        {
            if (m_gpuDirty)
            {
                m_gpuDirty = false;
                auto path = m_environmentMap.value<std::string>();
                if (path != "")
                {
                    m_environmentMapSRV = std::make_unique<TextureSRV>(device.createTextureSRV(
                        tools::hash(path),
                        TextureDescription()
                        .usage(ResourceUsage::CpuToGpu)
                        .name("environment irradiance map")
                        .width(static_cast<uint32_t>(m_environmentMapImage->width()))
                        .height(static_cast<uint32_t>(m_environmentMapImage->height()))
                        .format(m_environmentMapImage->format())
                        .arraySlices(static_cast<uint32_t>(m_environmentMapImage->arraySlices()))
                        .dimension(m_environmentMapImage->arraySlices() == 6 ? ResourceDimension::TextureCubemap : ResourceDimension::Texture2D)
                        .mipLevels(static_cast<uint32_t>(m_environmentMapImage->mipCount()))
                        .setInitialData(TextureDescription::InitialData(
                            std::vector<uint8_t>(m_environmentMapImage->data(), m_environmentMapImage->data() + m_environmentMapImage->bytes()),
                            static_cast<uint32_t>(m_environmentMapImage->width()), static_cast<uint32_t>(m_environmentMapImage->width() * m_environmentMapImage->height())))));
                }
                else
                {
                    m_environmentMapSRV = std::make_unique<TextureSRV>();
                }
            }
        }

        void start() override
        {
            m_transform = getComponent<Transform>();
        }

        int width() const;
        void width(int _width);

        int height() const;
        void height(int _height);

        float nearPlane() const;
        void nearPlane(float _near);

        float farPlane() const;
        void farPlane(float _far);

        float fieldOfView() const;
        void fieldOfView(float _fov);

        Projection projection() const;
        void projection(Projection _projection);

        Quaternionf rotation() const;
        void rotation(Quaternionf _rotation);

        Vector3f position() const;
        void position(Vector3f _position);

        void forward(Vector3f forward);
        Vector3f forward() const;

        void up(Vector3f up);
        Vector3f up() const;

        void right(Vector3f right);
        Vector3f right() const;

        void updateDelta(float delta);
        Vector3f smoothPosition() const;
        void smoothPosition(const Vector3f& smooth)
        {
            m_smoothPosition = smooth;
        }

        Vector3f target() const;
        void target(Vector3f target);

        Matrix4f viewMatrix() const;
        Matrix4f viewMatrix(Vector3f withPosition) const;
        Matrix4f projectionMatrix() const;
        Matrix4f projectionMatrix(Vector2<int> screenSize) const;

        Matrix4f jitterMatrix(uint64_t frameNumber, Vector2<int> screenSize) const;
        Vector2f jitterValue(uint64_t frameNumber) const;

        Matrix4f lookAt(
            const Vector3f& position,
            const Vector3f& target,
            const Vector3f& up) const;

        TextureSRV& environmentMap()
        {
            return *m_environmentMapSRV;
        }

        void environmentMap(TextureSRV& srv)
        {
            m_environmentMapSRV = std::make_unique<TextureSRV>(srv);
        }

        TextureSRV& environmentIrradiance()
        {
            return *m_environmentIrradianceSRV;
        }

        void environmentIrradiance(TextureSRV& srv)
        {
            m_environmentIrradianceSRV = std::make_unique<TextureSRV>(srv);
        }

        TextureSRV& environmentBrdfLUT()
        {
            return *m_environmentBrdfLUTSRV;
        }

        void environmentBrdfLUT(TextureSRV& srv)
        {
            m_environmentBrdfLUTSRV = std::make_unique<TextureSRV>(srv);
        }

        TextureSRV& environmentSpecular()
        {
            return *m_environmentSpecularSRV;
        }

        void environmentSpecular(TextureSRV& tex)
        {
            m_environmentSpecularSRV = std::make_unique<TextureSRV>(tex);
        }

        const std::string& environmentMapPath() const
        {
            return m_environmentMap.value<std::string>();
        }
        void environmentMapPath(const std::string& path)
        {
            m_environmentMap.value<std::string>(path);
        }

        float environmentMapStrength() const
        {
            return m_environmentMapStrength.value<float>();
        }

        void environmentMapStrength(float val)
        {
            m_environmentMapStrength.value<float>(val);
        }

        float followSpeed() const
        {
            return m_followSpeed.value<float>();
        }

        void followSpeed(float val)
        {
            m_followSpeed.value<float>(val);
        }

        float exposure() const { return m_exposure.value<float>(); }
        void exposure(float val) { m_exposure.value<float>(val); }

        bool pbrShadingModel() const { return static_cast<bool>(m_pbrShadingModel.value<engine::ButtonToggle>()); }
        void pbrShadingModel(bool val) { m_pbrShadingModel.value<engine::ButtonToggle>(static_cast<engine::ButtonToggle>(val)); }

        ViewCornerRays viewRays() const;
    private:
        Vector3f m_target;
        Vector3f m_smoothPosition;

        Vector3f m_forward;
        Vector3f m_up;
        Vector3f m_right;

        Matrix4f orthographicMatrix() const;
        Matrix4f perspectiveMatrix() const;

        Matrix4f orthographicMatrix(Vector2<int> screenSize) const;
        Matrix4f perspectiveMatrix(Vector2<int> screenSize) const;

        void createHaltonValues();
        std::vector<Vector2<double>> m_haltonValues;
    };
}
