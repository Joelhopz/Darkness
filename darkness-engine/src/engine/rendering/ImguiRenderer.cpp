#include "engine/rendering/ImguiRenderer.h"
#include "engine/graphics/SamplerDescription.h"
#include "engine/graphics/Viewport.h"
#include "engine/graphics/Resources.h"
#include "engine/graphics/Rect.h"
#include "engine/graphics/Barrier.h"
#include "engine/graphics/Common.h"

#include <math.h>
#include <algorithm>

namespace engine
{
    ImguiRenderer::ImguiRenderer(
        Device& device, ShaderStorage& shaderStorage)
        : m_shaderStorage{ shaderStorage }
        , m_pipeline{ device.createPipeline<shaders::ImguiRender>(shaderStorage) }
    {
        m_pipeline.setPrimitiveTopologyType(PrimitiveTopologyType::TriangleList);
        m_pipeline.setRenderTargetFormat(Format::Format_R8G8B8A8_UNORM, Format::Format_D32_FLOAT);
        m_pipeline.setRasterizerState(RasterizerDescription()
            .depthClipEnable(true)
            .cullMode(CullMode::None)
            .fillMode(FillMode::Solid)
            .frontCounterClockwise(false)
        );
        m_pipeline.ps.pointSampler = device.createSampler(
            SamplerDescription()
                .filter(engine::Filter::Point)
        );
        m_pipeline.setBlendState(BlendDescription().renderTarget(
            0, RenderTargetBlendDescription()
            .blendEnable(true)
            
            .srcBlend(Blend::SrcAlpha)
            .dstBlend(Blend::InvSrcAlpha)
            .blendOp(BlendOperation::Add)

            .srcBlendAlpha(Blend::InvSrcAlpha)
            .dstBlendAlpha(Blend::Zero)
            .blendOpAlpha(BlendOperation::Add)

            .renderTargetWriteMask(1 | 2 | 4 | 8)
        ));

        uint8_t* pixels = nullptr;
        int pixelsWidth;
        int pixelsHeight;

        ImGuiIO& io = ImGui::GetIO();
        io.DeltaTime = 1.0f / 60.0f;
        io.Fonts->AddFontDefault();
        io.Fonts->GetTexDataAsAlpha8(&pixels, &pixelsWidth, &pixelsHeight);

        ImGuiStyle &style = ImGui::GetStyle();
        auto color1 = Float4{ 70.0f / 255.0f, 70.0f / 255.0f, 70.0f / 255.0f, 77.0f / 255.0f };
        auto color2 = Float4{ 110.0f / 255.0f, 110.0f / 255.0f, 110.0f / 255.0f, 102.0f / 255.0f };
        auto color3 = Float4{ 200.0f / 255.0f, 70.0f / 255.0f, 70.0f / 255.0f, 102.0f / 255.0f };

        style.Colors[ImGuiCol_FrameBg] =        { color1.x, color1.y, color1.z, color1.w };
        style.Colors[ImGuiCol_FrameBgHovered] = { color2.x, color2.y, color2.z, color2.w };
        style.Colors[ImGuiCol_FrameBgActive] =    { color3.x, color3.y, color3.z, color3.w };

        m_fontAtlas = device.createTextureSRV(TextureDescription()
            .usage(ResourceUsage::CpuToGpu)
            .name("ImGui font atlas")
            .width(pixelsWidth)
            .height(pixelsHeight)
            .format(Format::Format_R8_UNORM)
            .arraySlices(1)
            .mipLevels(1)
            .setInitialData(TextureDescription::InitialData(
                tools::ByteRange(pixels, pixels + formatBytes(Format::Format_R8_UNORM, pixelsWidth, pixelsHeight)),
                static_cast<uint32_t>(formatBytes(Format::Format_R8_UNORM, pixelsWidth, 1)),
                static_cast<uint32_t>(formatBytes(
                    Format::Format_R8_UNORM, 
                    static_cast<unsigned int>(pixelsWidth), 
                    static_cast<unsigned int>(pixelsHeight)))))
        );
        m_pipeline.ps.tex = m_fontAtlas;

        m_mouseButtonStatus[MouseButton::Left] = false;
        m_mouseButtonStatus[MouseButton::Right] = false;
        m_mouseButtonStatus[MouseButton::Center] = false;

        m_mouseButtonWasDownAtleastOnce[MouseButton::Left] = false;
        m_mouseButtonWasDownAtleastOnce[MouseButton::Right] = false;
        m_mouseButtonWasDownAtleastOnce[MouseButton::Center] = false;

        m_lastKnownMousePosition = {};
    }

    bool ImguiRenderer::usesMouse() const
    {
        ImGuiIO& io = ImGui::GetIO();
        return io.WantCaptureMouse;
    }

    void ImguiRenderer::startFrame(
        Device& device,
        float delta)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = { static_cast<float>(device.width()), static_cast<float>(device.height()) };
        io.DeltaTime = delta;

        io.MousePos = { 
            static_cast<float>(m_lastKnownMousePosition.first),
            static_cast<float>(m_lastKnownMousePosition.second) };

        io.MousePosPrev = {
            static_cast<float>(m_lastKnownMousePositionPrev.first),
            static_cast<float>(m_lastKnownMousePositionPrev.second) };
        m_lastKnownMousePositionPrev = m_lastKnownMousePosition;
        
        for (int i = 0; i < 3; ++i)
        {
            io.MouseDown[i] = m_mouseButtonStatus[static_cast<MouseButton>(i)];
            io.MouseClicked[i] = false;
            io.MouseClickedPos[i] = {};

            if (m_mouseButtonWasDownAtleastOnce[static_cast<MouseButton>(i)] && !m_mouseButtonStatus[static_cast<MouseButton>(i)])
            {
                m_mouseButtonWasDownAtleastOnce[static_cast<MouseButton>(i)] = false;

                auto xDiff = m_lastKnownMousePosition.first - m_lastKnownClickPosition[static_cast<MouseButton>(i)].first;
                auto yDiff = m_lastKnownMousePosition.second - m_lastKnownClickPosition[static_cast<MouseButton>(i)].second;
                if (std::abs(xDiff) < 5 && std::abs(yDiff) < 5)
                {
                    io.MouseClicked[i] = true;
                    io.MouseClickedPos[i] = {
                        static_cast<float>(m_lastKnownClickPosition[static_cast<MouseButton>(i)].first),
                        static_cast<float>(m_lastKnownClickPosition[static_cast<MouseButton>(i)].second) };
                }
            }
        }

        ImGui::NewFrame();


        //ImGui::ShowTestWindow();
    }

    void ImguiRenderer::recreateBuffersIfNeeded(Device& device, CommandList& cmd, ImVector<ImDrawVert>& vbv, ImVector<ImDrawIdx>& ibv)
    {
        if (!m_vbv.valid() || (m_vbv.valid() && vbv.size() > m_vbv.desc().elements))
        {
            m_vbvs.emplace_back(m_vbv);
            m_vbv = device.createBufferVBV(BufferDescription()
                .name("ImGui vbv")
                .usage(ResourceUsage::CpuToGpu)
                .structured(true)
                .setInitialData(BufferDescription::InitialData(
                    tools::ByteRange(vbv.begin(), vbv.end()), 1)));
            //cmd.transition(m_vbv, ResourceState::CopyDest, ResourceState::VertexAndConstantBuffer);
            //cmd.transition(m_vbv, ResourceState::VertexAndConstantBuffer);
        }
        else
        {
            //cmd.transition(m_vbv, ResourceState::VertexAndConstantBuffer, ResourceState::CopyDest);
            //cmd.transition(m_vbv, ResourceState::CopyDest);
            device.uploadBuffer(cmd, m_vbv, tools::ByteRange(vbv.begin(), vbv.end()));
            //cmd.transition(m_vbv, ResourceState::CopyDest, ResourceState::VertexAndConstantBuffer);
            //cmd.transition(m_vbv, ResourceState::VertexAndConstantBuffer);
        }
        
        if (!m_ibv.valid() || (m_ibv.valid() && ibv.size() > m_ibv.desc().elements))
        {
            m_ibvs.emplace_back(m_ibv);
            m_ibv = device.createBufferIBV(BufferDescription()
                .name("ImGui ibv")
                .usage(ResourceUsage::CpuToGpu)
                .setInitialData(BufferDescription::InitialData(
                    tools::ByteRange(ibv.begin(), ibv.end()), 1)));
            //cmd.transition(m_ibv, ResourceState::CopyDest, ResourceState::IndexBuffer);
            //cmd.transition(m_ibv, ResourceState::IndexBuffer);
        }
        else
        {
            //cmd.transition(m_ibv, ResourceState::IndexBuffer, ResourceState::CopyDest);
            //cmd.transition(m_ibv, ResourceState::CopyDest);
            device.uploadBuffer(cmd, m_ibv, tools::ByteRange(ibv.begin(), ibv.end()));
            //cmd.transition(m_ibv, ResourceState::CopyDest, ResourceState::IndexBuffer);
            //cmd.transition(m_ibv, ResourceState::IndexBuffer);
        }

        if (m_vbvs.size() > BackBufferCount)
            m_vbvs.erase(m_vbvs.begin());
        if (m_ibvs.size() > BackBufferCount)
            m_ibvs.erase(m_ibvs.begin());
    }

    void ImguiRenderer::render(const FlatScene& scene)
    {
        auto frustumPlanes = extractFrustumPlanes(scene.cameras[scene.selectedCamera]->projectionMatrix() * scene.cameras[scene.selectedCamera]->viewMatrix());

        ImGuiIO& io = ImGui::GetIO();
        ImGui::Begin("hud", NULL, io.DisplaySize, 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus);

        ImDrawList* drawList = ImGui::GetWindowDrawList();

        ImGui::End();
        
        auto cam = scene.cameras[scene.selectedCamera];
        if (!cam)
            return;

        auto modelTrans = 
            Matrix4f::translate({0.0f, 0.0f, 0.0f}) *
            Matrix4f::rotation({ 0.0f, 0.0f, 0.0f }) *
            Matrix4f::scale({ 1.0f, 1.0f, 1.0f });
        auto viewMat = cam->viewMatrix();
        auto projMat = cam->projectionMatrix();

        auto mvp = projMat * viewMat * modelTrans;

        // draw grid lines
        constexpr float gridXInc = 1.0f;
        constexpr float gridZInc = 1.0f;
        constexpr int gridXCount = 40;
        constexpr int gridZCount = 40;
        float gridWidth = gridXInc * static_cast<float>(gridXCount);
        float gridHeight = gridZInc * static_cast<float>(gridZCount);

        float startX = -(gridWidth / 2.0f);
        float startZ = -(gridHeight / 2.0f);
        float origX = startX;
        float origZ = startZ;
        ImU32 gridColor = 0x88333333;
        float gridThickness = 0.8f;
        ImU32 gridAxisColor = 0xff333333;
        float gridAxisThickness = 1.0f;

        for (int x = 0; x < gridXCount+1; ++x)
        {
            Vector3f lineBegin{ startX, 0.0f, origZ };
            Vector3f lineEnd{ startX, 0.0f, origZ + gridHeight };

            if (x == gridXCount / 2)
                drawLine3d(*cam, frustumPlanes, mvp, drawList, lineBegin, lineEnd, gridAxisColor, gridAxisThickness);
            else
                drawLine3d(*cam, frustumPlanes, mvp, drawList, lineBegin, lineEnd, gridColor, gridThickness);

            //
            Vector3f lineBegin2{ origX, 0.0f, startZ };
            Vector3f lineEnd2{ origX + gridWidth, 0.0f, startZ };

            if (x == gridXCount / 2)
                drawLine3d(*cam, frustumPlanes, mvp, drawList, lineBegin2, lineEnd2, gridAxisColor, gridAxisThickness);
            else
                drawLine3d(*cam, frustumPlanes, mvp, drawList, lineBegin2, lineEnd2, gridColor, gridThickness);

            startX += gridXInc;
            startZ += gridZInc;
        }

        // light UI
        for (auto&& light : scene.lights)
        {
            if (light.type == LightType::Spot)
            {
                ImU32 spotColor = 0x881080FF;
                float spotThickness = 0.7f;
                int sectorCount = 50;

                // light direction line
                Vector3f wstart = light.position;
                Vector3f wstop = light.position - (light.direction * light.light->range());
                drawLine3d(*cam, frustumPlanes, mvp, drawList, wstart, wstop, spotColor, spotThickness);
                
                // full circle at edge of outer angle
                float outDist = cos(light.outerCone * DEG_TO_RAD) * light.light->range();
                float radius = tan(light.outerCone * DEG_TO_RAD) * outDist;
                Matrix4f spotMat = Matrix4f::translate(light.direction.invert() * outDist) * light.transform;
                drawCircle3d(*cam, frustumPlanes, mvp, spotMat, drawList, radius, sectorCount, spotColor, spotThickness);

                // full circle at edge of inner angle
                float innerCone = min(light.innerCone, light.outerCone);
                float innerDist = cos(innerCone * DEG_TO_RAD) * light.light->range();
                float innerRadius = tan(innerCone * DEG_TO_RAD) * innerDist;
                Matrix4f innerSpotMat = Matrix4f::translate(light.direction.invert() * innerDist) * light.transform;
                drawCircle3d(*cam, frustumPlanes, mvp, innerSpotMat, drawList, innerRadius, sectorCount, spotColor, spotThickness);

                // horizontal curve
                Matrix4f spotCurveMat = light.transform;
                drawCircle3d(*cam, frustumPlanes, mvp, spotCurveMat, drawList, light.light->range(), sectorCount, spotColor, spotThickness,
                    -light.outerCone - 90.0f, light.outerCone - 90.0f, 
                    CirclePlane::XZ);

                // vertical curve
                drawCircle3d(*cam, frustumPlanes, mvp, spotCurveMat, drawList, light.light->range(), sectorCount, spotColor, spotThickness,
                    -light.outerCone - 90.0f, light.outerCone - 90.0f, 
                    CirclePlane::YZ);

                // edge lines
                Vector4f pointR = spotMat * Vector4f{radius, 0.0f, 0.0f, 1.0f };
                drawLine3d(*cam, frustumPlanes, mvp, drawList, light.position, pointR.xyz(), spotColor, spotThickness);

                Vector4f pointL = spotMat * Vector4f{ -radius, 0.0f, 0.0f, 1.0f };
                drawLine3d(*cam, frustumPlanes, mvp, drawList, light.position, pointL.xyz(), spotColor, spotThickness);

                Vector4f pointT = spotMat * Vector4f{ 0.0f, radius, 0.0f, 1.0f };
                drawLine3d(*cam, frustumPlanes, mvp, drawList, light.position, pointT.xyz(), spotColor, spotThickness);

                Vector4f pointB = spotMat * Vector4f{ 0.0f, -radius, 0.0f, 1.0f };
                drawLine3d(*cam, frustumPlanes, mvp, drawList, light.position, pointB.xyz(), spotColor, spotThickness);
            }

            else if (light.type == LightType::Point)
            {
                ImU32 spotColor = 0x881080FF;
                float spotThickness = 0.7f;
                int sectorCount = 50;
                int vertRotationCount = 8;
                float currentRotation = 0;
                float rotationInc = 180.0f / vertRotationCount;
                for (int i = 0; i < vertRotationCount; ++i)
                {
                    Matrix4f rot = Matrix4f::rotation({ 0.0f, currentRotation, 0.0f });
                    Matrix4f circleRotation = rot * light.transform;
                    drawCircle3d(*cam, frustumPlanes, mvp, circleRotation, drawList, light.light->range(), sectorCount, spotColor, spotThickness, 0.0f, 360.0f, CirclePlane::XY);
                    currentRotation += rotationInc;
                }
                drawCircle3d(*cam, frustumPlanes, mvp, light.transform, drawList, light.light->range(), sectorCount, spotColor, spotThickness, 0.0f, 360.0f, CirclePlane::XZ);
                drawCircle3d(*cam, frustumPlanes, mvp, light.transform, drawList, light.light->range(), sectorCount, spotColor, spotThickness, 0.0f, 360.0f, CirclePlane::YZ);
            }

            else if (light.type == LightType::Directional)
            {
                ImU32 spotColor = 0x881080FF;
                float spotThickness = 0.7f;
                int sectorCount = 20;

                // light direction line
                Vector3f wstart = light.position;
                Vector3f wstop = light.position - (light.direction * 4.0f);
                drawLine3d(*cam, frustumPlanes, mvp, drawList, wstart, wstop, spotColor, spotThickness * 2.0f);

                int vertRotationCount = 8;
                float currentRotation = 0;
                float rotationInc = 360.0f / vertRotationCount;
                float radius = 1.0f;
                for (int i = 0; i < vertRotationCount; ++i)
                {
                    Vector4f point{
                        radius * cosf(currentRotation),
                        radius * sinf(currentRotation),
                        0.0f,
                        1.0f };
                    point = light.transform * point;
                    
                    Vector3f pointEnd = point.xyz() - (light.direction * 2.0f);
                    drawLine3d(*cam, frustumPlanes, mvp, drawList, point.xyz(), pointEnd, spotColor, spotThickness);

                    currentRotation += rotationInc;
                }

                drawCircle3d(*cam, frustumPlanes, mvp, light.transform, drawList, 1.0f, sectorCount, spotColor, spotThickness);
            }
        }

#if 0
        gridAxisColor = 0xffaa3333;
        for (auto&& camera : scene.cameras)
        {
            auto cornerRays = camera->viewRays();
            drawLine3d(*cam, frustumPlanes, mvp, drawList, 
                camera->position() + (cornerRays.topLeft * camera->nearPlane()), 
                camera->position() + (cornerRays.topLeft * camera->farPlane()),
                gridAxisColor, gridAxisThickness);
            drawLine3d(*cam, frustumPlanes, mvp, drawList,
                camera->position() + (cornerRays.topRight * camera->nearPlane()),
                camera->position() + (cornerRays.topRight * camera->farPlane()),
                gridAxisColor, gridAxisThickness);
            drawLine3d(*cam, frustumPlanes, mvp, drawList,
                camera->position() + (cornerRays.bottomLeft * camera->nearPlane()),
                camera->position() + (cornerRays.bottomLeft * camera->farPlane()),
                gridAxisColor, gridAxisThickness);
            drawLine3d(*cam, frustumPlanes, mvp, drawList,
                camera->position() + (cornerRays.bottomRight * camera->nearPlane()),
                camera->position() + (cornerRays.bottomRight * camera->farPlane()),
                gridAxisColor, gridAxisThickness);

            drawLine3d(*cam, frustumPlanes, mvp, drawList,
                camera->position() + (cornerRays.topLeft * camera->nearPlane()),
                camera->position() + (cornerRays.topRight * camera->nearPlane()),
                gridAxisColor, gridAxisThickness);
            drawLine3d(*cam, frustumPlanes, mvp, drawList,
                camera->position() + (cornerRays.topRight * camera->nearPlane()),
                camera->position() + (cornerRays.bottomRight * camera->nearPlane()),
                gridAxisColor, gridAxisThickness);
            drawLine3d(*cam, frustumPlanes, mvp, drawList,
                camera->position() + (cornerRays.bottomRight * camera->nearPlane()),
                camera->position() + (cornerRays.bottomLeft * camera->nearPlane()),
                gridAxisColor, gridAxisThickness);
            drawLine3d(*cam, frustumPlanes, mvp, drawList,
                camera->position() + (cornerRays.bottomLeft * camera->nearPlane()),
                camera->position() + (cornerRays.topLeft * camera->nearPlane()),
                gridAxisColor, gridAxisThickness);

            drawLine3d(*cam, frustumPlanes, mvp, drawList,
                camera->position() + (cornerRays.topLeft * camera->farPlane()),
                camera->position() + (cornerRays.topRight * camera->farPlane()),
                gridAxisColor, gridAxisThickness);
            drawLine3d(*cam, frustumPlanes, mvp, drawList,
                camera->position() + (cornerRays.topRight * camera->farPlane()),
                camera->position() + (cornerRays.bottomRight * camera->farPlane()),
                gridAxisColor, gridAxisThickness);
            drawLine3d(*cam, frustumPlanes, mvp, drawList,
                camera->position() + (cornerRays.bottomRight * camera->farPlane()),
                camera->position() + (cornerRays.bottomLeft * camera->farPlane()),
                gridAxisColor, gridAxisThickness);
            drawLine3d(*cam, frustumPlanes, mvp, drawList,
                camera->position() + (cornerRays.bottomLeft * camera->farPlane()),
                camera->position() + (cornerRays.topLeft * camera->farPlane()),
                gridAxisColor, gridAxisThickness);
        }
#endif
    }

    void ImguiRenderer::endFrame(
        Device& device,
        TextureRTV& currentRenderTarget,
        TextureDSV& currentDepthTarget,
        CommandList& cmd)
    {
        ImGui::Render();

        auto drawData = ImGui::GetDrawData();
        ASSERT(drawData->Valid, "ImGui draw data was invalid");

        m_pipeline.vs.reciprocalResolution = {
            1.0f / static_cast<float>(currentRenderTarget.width()),
            1.0f / static_cast<float>(currentRenderTarget.height()),
        };

        cmd.setRenderTargets({ currentRenderTarget }, currentDepthTarget);

        cmd.setViewPorts({ Viewport{
            0.0f,
            0.0f,
            static_cast<float>(device.width()),
            static_cast<float>(device.height()),
            0.0f, 1.0f } });
        cmd.setScissorRects({ Rectangle{
            0, 0,
            static_cast<unsigned int>(device.width()),
            static_cast<unsigned int>(device.height()) } });

        for (int i = 0; i < drawData->CmdListsCount; ++i)
        {
            auto list = drawData->CmdLists[i];

            recreateBuffersIfNeeded(device, cmd, list->VtxBuffer, list->IdxBuffer);

            cmd.bindVertexBuffer(m_vbv);
            cmd.bindIndexBuffer(m_ibv);

            unsigned int indexOffset = 0;
            for (auto &d : list->CmdBuffer)
            {
                if (d.UserCallback)
                {
                    d.UserCallback(list, &d);
                }
                else
                {
                    cmd.setScissorRects({ Rectangle{
                        static_cast<unsigned int>(d.ClipRect.x), 
                        static_cast<unsigned int>(d.ClipRect.y),
                        static_cast<unsigned int>(d.ClipRect.z),
                        static_cast<unsigned int>(d.ClipRect.w) } });

                    cmd.bindPipe(m_pipeline);

                    cmd.drawIndexed(d.ElemCount, 1, indexOffset, 0, 0);
                }

                indexOffset += d.ElemCount;
            }
        }
    }

    void ImguiRenderer::onMouseMove(int x, int y)
    {
        m_lastKnownMousePosition.first = x;
        m_lastKnownMousePosition.second = y;
    }

    void ImguiRenderer::onMouseDown(MouseButton button, int x, int y)
    {
        m_lastKnownMousePosition.first = x;
        m_lastKnownMousePosition.second = y;

        m_lastKnownClickPosition[button] = { x, y };

        m_mouseButtonStatus[button] = true;
        m_mouseButtonWasDownAtleastOnce[button] = true;
    }

    void ImguiRenderer::onMouseUp(MouseButton button, int x, int y)
    {
        m_lastKnownMousePosition.first = x;
        m_lastKnownMousePosition.second = y;

        m_mouseButtonStatus[button] = false;
    }

    void ImguiRenderer::onMouseDoubleClick(MouseButton /*button*/, int x, int y)
    {
        m_lastKnownMousePosition.first = x;
        m_lastKnownMousePosition.second = y;

        // TODO: dunno if we use doubleclick
    }
}
