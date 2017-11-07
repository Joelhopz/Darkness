#include "engine/Engine.h"
#include "engine/graphics/Common.h"
#include "engine/primitives/Matrix4.h"
#include "components/MeshRendererComponent.h"
#include "components/MaterialComponent.h"
#include "plugins/PluginManager.h"
#include "engine/graphics/Barrier.h"
#include "external/imgui/imgui.h"
#include "external/ImGuizmo/ImGuizmo.h"
#include "tools/measure.h"
#include "tools/RefCounted.h"
#include <vector>
#include <chrono>
#include <algorithm>

#include "entityx.h"

using namespace engine;

Engine::Engine(
    std::shared_ptr<platform::Window> window,
    const std::string& shaderRootPath)
    : m_window{ window }
    , m_renderSetup{ std::make_unique<RenderSetup>(window) }
    , m_shaderRootPath{ shaderRootPath }
    , m_cameraSizeRefresh{ true }
    , m_virtualResolution{ m_renderSetup->device().width(), m_renderSetup->device().height() }
    , m_modelRenderer{std::make_unique<ModelRenderer>( m_renderSetup->device(), m_renderSetup->shaderStorage(), m_virtualResolution) }
    , m_modelTransparentRenderer{ std::make_unique<ModelTransparentRenderer>(m_renderSetup->device(), m_renderSetup->shaderStorage()) }
    , m_shadowRenderer{ std::make_unique<ShadowRenderer>(m_renderSetup->device(), m_renderSetup->shaderStorage()) }
    , m_renderCubemap{ std::make_unique<RenderCubemap>(m_renderSetup->device(), m_renderSetup->shaderStorage()) }
    , m_postProcess{ std::make_unique<Postprocess>(m_renderSetup->device(), m_renderSetup->shaderStorage()) }
    , m_imguiRenderer{ std::make_unique<ImguiRenderer>( m_renderSetup->device(), m_renderSetup->shaderStorage() ) }
    , m_debugViewer{ std::make_unique<DebugView>(m_renderSetup->device(), m_renderSetup->shaderStorage()) }
    , m_defaultMaterial{ std::make_unique<MaterialComponent>( 
        "C:\\work\\darkness\\darkness-engine\\data\\default\\default_albedo.png",
        "C:\\work\\darkness\\darkness-engine\\data\\default\\default_specular.png",
        "C:\\work\\darkness\\darkness-engine\\data\\default\\default_normal.png",
        "C:\\work\\darkness\\darkness-engine\\data\\default\\default_metalness.png",
        "C:\\work\\darkness\\darkness-engine\\data\\default\\default_metalness.png"
        ) }
    , m_lightData{ std::make_unique<LightData>() }
    
    , m_rtv{ m_renderSetup->device().createTextureRTV(TextureDescription()
        .width(m_virtualResolution.x)
        .height(m_virtualResolution.y)
        .format(engine::Format::Format_R16G16B16A16_FLOAT)
        .usage(ResourceUsage::GpuRenderTargetReadWrite)
        .name("main render target")
        .dimension(ResourceDimension::Texture2D)
        .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 1.0f })
        ) }
    , m_srv{ m_renderSetup->device().createTextureSRV(m_rtv.texture()) }
    , m_dsv{ m_renderSetup->device().createTextureDSV(TextureDescription()
        .name("Depth Buffer")
        .format(Format::Format_D32_FLOAT)
        .width(m_virtualResolution.x)
        .height(m_virtualResolution.y)
        .usage(ResourceUsage::DepthStencil)
        .optimizedDepthClearValue(0.0f)
        .dimension(ResourceDimension::Texture2D)
        ) }
    , m_dsvSRV{ m_renderSetup->device().createTextureSRV(m_dsv) }
    , m_cameraTransform{ std::make_shared<engine::Transform>() }
    , m_camera{ m_cameraTransform }
    , m_cameraInputActive{ false }
    , m_lastEnvironmentMap{ "" }
    , m_simulating{ false }
{
    entityx::EntityX ex;
    entityx::Entity entity = ex.entities.create();
    entity.assign<Vector3f>(0.0f, 0.0f, 0.0f);

    entity.destroy();

    ///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
    m_collisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();

    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    m_dispatcher = std::make_unique<btCollisionDispatcher>(m_collisionConfiguration.get());

    ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    m_overlappingPairCache = std::make_unique<btDbvtBroadphase>();

    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    m_solver = std::make_unique<btSequentialImpulseConstraintSolver>();

    m_dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(m_dispatcher.get(), m_overlappingPairCache.get(), m_solver.get(), m_collisionConfiguration.get());

    m_dynamicsWorld->setGravity(btVector3(0.0f, -9.81f, 0.0f));

    ///-----initialization_end-----

    //keep track of the shapes, we release memory at exit.
    //make sure to re-use collision shapes among rigid bodies whenever possible!
    m_collisionShapes = std::make_unique<btAlignedObjectArray<btCollisionShape*>>();


    ///create a few basic rigid bodies

    //the ground is a cube of side 100 at position y = -56.
    //the sphere will hit it at y = -6, with center at -5
    {
        btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));

        m_collisionShapes->push_back(groundShape);

        btTransform groundTransform;
        groundTransform.setIdentity();
        groundTransform.setOrigin(btVector3(0, -50.0, 0));

        btScalar mass(0.);

        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0, 0, 0);
        if (isDynamic)
            groundShape->calculateLocalInertia(mass, localInertia);

        //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);

        //add the body to the dynamics world
        m_dynamicsWorld->addRigidBody(body);
    }


    m_defaultMaterial->gpuRefresh(m_renderSetup->device());

    m_camera.width(m_window->width());
    m_camera.height(m_window->height());
    m_camera.nearPlane(1.0f);
    m_camera.farPlane(0.0f);
    m_camera.fieldOfView(90.0f);
    m_camera.projection(Projection::Perspective);
    m_camera.rotation(Quaternionf::fromEulerAngles(0.0f, 0.0f, 0.0f));
    m_camera.position({ 0.0f, 2.0f, -10.0f});
}

std::shared_ptr<engine::Camera> Engine::camera()
{
    return m_scene.getComponent<engine::Camera>();
}

engine::Scene& Engine::scene()
{
    return m_scene;
}

engine::ShaderStorage& Engine::shaderStorage()
{
    return m_renderSetup->shaderStorage();
}

void Engine::update()
{
    render();
    if (m_renderSetup->swapChain().needRefresh())
    {
        refresh();
    }
}

void Engine::playClicked(bool value)
{
    m_simulating = value;
    m_scene.root()->invalidate();
}

float Engine::delta()
{
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(now - m_lastUpdate).count()) / 1000000.0;
    m_lastUpdate = now;
    //m_physicsSystem->DoFrameDynamics(min(duration, 0.3));
    if(m_simulating)
        m_dynamicsWorld->stepSimulation(1.f / 60.f, 10);

    return 1.0f / 60.0f;
}

void Engine::resetCameraSize()
{
    auto flatScene = m_scene.flatten(false);
    for(auto&& camera : flatScene.cameras)
    {
        camera->width(m_renderSetup->window().width());
        camera->height(m_renderSetup->window().height());
    }
}
static Matrix4f id = Matrix4f::identity();

void Engine::render()
{
    CPU_MARKER("Render");
    engine::Device& device = m_renderSetup->device();

    float deltaTime = delta();
    auto& flatScene = m_scene.flatten(m_simulating);

    {
        CPU_MARKER("Cpu/Gpu refresh");
        for (auto&& node : flatScene.nodes)
        {
            if (node.rigidBody && node.rigidBody->body())
            {
                if (std::find(
                    m_addedRigidBodies.begin(),
                    m_addedRigidBodies.end(),
                    static_cast<void*>(node.rigidBody->body().get())) == m_addedRigidBodies.end())
                {
                    m_dynamicsWorld->addRigidBody(node.rigidBody->body().get());
                    m_addedRigidBodies.emplace_back(node.rigidBody->body().get());
                }
            }

            if (node.material)
            {
                node.material->cpuRefresh(device);
                node.material->gpuRefresh(device);
            }

            if (node.mesh)
            {
                node.mesh->cpuRefresh(device);
                node.mesh->gpuRefresh(device);
            }
        }
    }

    if (flatScene.selectedCamera != -1 && flatScene.cameras.size() > 0)
    {
        CPU_MARKER("Update camera and inputs");
        m_cameraInput.setCamera(flatScene.cameras[flatScene.selectedCamera]);

        if (m_cameraSizeRefresh)
        {
            for (auto&& camera : flatScene.cameras)
            {
                camera->width(m_renderSetup->window().width());
                camera->height(m_renderSetup->window().height());
            }
            m_cameraSizeRefresh = false;
        }

        for (auto&& camera : flatScene.cameras)
        {
            camera->cpuRefresh(device);
            camera->gpuRefresh(device);
        }

        if (m_cameraInputActive)
        {
            m_cameraInput.update(deltaTime);
        }
        else
        {
            for(int i = 1; i < flatScene.cameras.size(); ++i)
            { 
                m_cameraInput.setCamera(flatScene.cameras[i]);
                m_cameraInput.position(flatScene.cameras[i]->position());
                m_cameraInput.rotation(flatScene.cameras[i]->rotation().toEulerAngles());
                m_cameraInput.apply(false);
            }
            m_cameraInput.setCamera(flatScene.cameras[flatScene.selectedCamera]);
            m_cameraInput.position(flatScene.cameras[flatScene.selectedCamera]->position());
            m_cameraInput.rotation(flatScene.cameras[flatScene.selectedCamera]->rotation().toEulerAngles());
            m_cameraInput.apply(false);
        }
    }

    auto cmd = device.createCommandList();

    {
        GPU_MARKER(cmd, "Render");

        // Clear render targets
        clear(cmd);

        // Start imgui
        m_imguiRenderer->startFrame(device, deltaTime);

        // start imguizmo (ui widgets)
        //ImGuizmo::BeginFrame();

        // update environment cubemap
        if (flatScene.selectedCamera != -1 && flatScene.cameras.size() > 0 && flatScene.cameras[flatScene.selectedCamera])
            updateEnvironmentCubemap(device, *flatScene.cameras[flatScene.selectedCamera]);

        // lighting update
        updateLighting(device, cmd, flatScene);

        // render shadow maps
        renderShadows(device, cmd, flatScene);

        // render environment cubemap
        renderEnvironmentCubemap(device, cmd, flatScene);

        // render models
        renderModels(device, cmd, flatScene);

        // render models
        renderTransparentModels(device, cmd, flatScene);

        // postprocess
        renderPostprocess(device, cmd, flatScene);

        // render debug view
        renderDebugView(cmd);

        // render log window
        m_logWindow.render(m_renderSetup->window().width(), m_renderSetup->window().height());

        // draw gizmos
        /*if (flatScene.camera)
        {
            auto viewMat = flatScene.camera->viewMatrix().transpose();
            auto projMat = flatScene.camera->projectionMatrix().transpose();


            ImGuiIO& io = ImGui::GetIO();
            ImGuizmo::Enable(true);
            ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
            ImGuizmo::Manipulate(
                &viewMat.m00,
                &projMat.m00,
                ImGuizmo::OPERATION::ROTATE,
                ImGuizmo::MODE::WORLD,
                &id.m00);


            ImGuizmo::DrawCube(
                &viewMat.m00,
                &projMat.m00,
                &id.m00);
        }*/

        // draw imgui stuff
        {
            CPU_MARKER("Imgui");
            GPU_MARKER(cmd, "Imgui");

            m_imguiRenderer->render(flatScene);

            // Stop imgui
            m_imguiRenderer->endFrame(device, m_renderSetup->currentRTV(), m_dsv, cmd);
        }
    }

    // execute command list and present
    m_renderSetup->submit(std::move(cmd));
    m_renderSetup->present();
    //LOG("Picked object id: %u", m_modelRenderer->pickedObject(device));

    // process shader hotreload
    processShaderHotreload();

    // process window messages
    m_renderSetup->window().processMessages();
}

void Engine::clear(engine::CommandList& cmd)
{
    CPU_MARKER("Clear render targets");
    GPU_MARKER(cmd, "Clear render targets");
    cmd.clearRenderTargetView(m_renderSetup->currentRTV(), { 0.0f, 0.0f, 0.0f, 1.0f });
    cmd.clearRenderTargetView(m_rtv, { 0.0f, 0.0f, 0.0f, 1.0f });
    cmd.clearDepthStencilView(m_dsv, 0.0f);
}

void Engine::updateLighting(engine::Device& device, engine::CommandList& cmd, engine::FlatScene& flatScene)
{
    CPU_MARKER("Update lighting");
    GPU_MARKER(cmd, "Update lighting");
    m_defaultMaterial->cpuRefresh(device);
    m_defaultMaterial->gpuRefresh(m_renderSetup->device());
    m_lightData->updateLightInfo(m_renderSetup->device(), cmd, flatScene.lights);
    if (m_lightData->changeHappened())
    {
        m_scene.root()->invalidate();
        m_shadowRenderer->refresh();
    }
}

void Engine::updateEnvironmentCubemap(engine::Device& device, engine::Camera& camera)
{
    CPU_MARKER("Update environment cubemap");
    if (camera.environmentMap().valid())// && )
    {
        if (camera.environmentMapPath() != m_lastEnvironmentMap)
        {
            // only one slice. we need equirect mapping to cubemap
            if (camera.environmentMap().texture().arraySlices() == 1)
            {
                m_renderCubemap->createCubemapFromEquirect(device, camera.environmentMap());
                m_renderCubemap->createIrradianceCubemap(device, m_renderCubemap->cubemap());
                m_renderCubemap->prefilterConvolution(device, m_renderCubemap->cubemap());
                m_renderCubemap->brdfConvolution(device);
            }
            else
            {
                m_renderCubemap->cubemap(camera.environmentMap());
                m_renderCubemap->createIrradianceCubemap(device, camera.environmentMap());
                m_renderCubemap->prefilterConvolution(device, camera.environmentMap());
                m_renderCubemap->brdfConvolution(device);
            }
            
            camera.environmentBrdfLUT(m_renderCubemap->brdfConvolution());
            camera.environmentIrradiance(m_renderCubemap->irradiance());
            camera.environmentSpecular(m_renderCubemap->prefilteredEnvironmentMap());

            m_lastEnvironmentMap = camera.environmentMapPath();
        }
    }
}

void Engine::renderEnvironmentCubemap(engine::Device& device, engine::CommandList& cmd, FlatScene& flatScene)
{
    CPU_MARKER("Render environment cubemap");
    GPU_MARKER(cmd, "Render environment cubemap");
    m_renderCubemap->render(
        device,
        m_rtv,
        m_dsv,
        ((flatScene.selectedCamera != -1) && flatScene.cameras[flatScene.selectedCamera]) ? *flatScene.cameras[flatScene.selectedCamera] : m_camera,
        cmd);
}

void Engine::renderShadows(engine::Device& device, engine::CommandList& cmd, engine::FlatScene& flatScene)
{
    CPU_MARKER("Render shadows");
    GPU_MARKER(cmd, "Render shadows");
    m_shadowRenderer->render(
        device,
        m_rtv,
        m_dsv,
        cmd,
        *m_defaultMaterial,
        ((flatScene.selectedCamera != -1) && flatScene.cameras[flatScene.selectedCamera]) ? *flatScene.cameras[flatScene.selectedCamera] : m_camera,
        *m_lightData,
        flatScene);
}

void Engine::renderModels(engine::Device& device, engine::CommandList& cmd, FlatScene& flatScene)
{
    CPU_MARKER("Render models");
    GPU_MARKER(cmd, "Render models");
    m_modelRenderer->render(
        device,
        m_rtv,
        m_dsv,
        m_dsvSRV,
        cmd,
        *m_defaultMaterial,
        ((flatScene.selectedCamera != -1) && flatScene.cameras[flatScene.selectedCamera]) ? *flatScene.cameras[flatScene.selectedCamera] : m_camera,
        *m_lightData,
        flatScene,
        m_shadowRenderer->shadowMap(),
        m_shadowRenderer->shadowVP(),
        m_cameraInput.mousePosition().first,
        m_cameraInput.mousePosition().second);
}

void Engine::renderTransparentModels(
    engine::Device& /*device*/, 
    engine::CommandList& cmd, 
    engine::FlatScene& /*flatScene*/)
{
    CPU_MARKER("Render transparent models");
    GPU_MARKER(cmd, "Render transparent models");
    /*m_modelTransparentRenderer->render(
        device,
        m_rtv,
        m_dsv,
        cmd,
        *m_defaultMaterial,
        flatScene.camera ? *flatScene.camera : m_camera,
        *m_lightData,
        flatScene,
        m_shadowRenderer->shadowMap(),
        m_shadowRenderer->shadowVP());*/
}

void Engine::renderPostprocess(engine::Device& device, engine::CommandList& cmd, engine::FlatScene& flatScene)
{
    CPU_MARKER("Render postprocess");
    GPU_MARKER(cmd, "Render postprocess");
    if(flatScene.postprocess)
        m_postProcess->render(
            device, 
            m_renderSetup->currentRTV(), 
            m_modelRenderer->finalFrame(),//m_srv, 
            cmd, 
            *flatScene.postprocess);
}

void Engine::renderDebugView(engine::CommandList& cmd)
{
    CPU_MARKER("Render debug view");
    GPU_MARKER(cmd, "Render debug view");
    /*m_debugViewer->render(
        m_renderSetup->device(),
        m_renderSetup->currentRTV(),
        cmd,
        m_modelRenderer->motionVectors()
        //m_shadowRenderer->shadowMap()
    );*/

    /*m_debugViewer->render(
        m_renderSetup->device(),
        m_renderSetup->currentRTV(),
        cmd,
        m_modelRenderer->ssaoSRV()
        //m_shadowRenderer->shadowMap()
    );*/

    /*m_debugViewer->render(
        m_renderSetup->device(),
        m_renderSetup->currentRTV(),
        cmd,
        m_renderCubemap->brdfConvolution()
        //m_shadowRenderer->shadowMap()
    );*/

    /*m_debugViewer->render(
        m_renderSetup->device(),
        m_renderSetup->currentRTV(),
        cmd,
        m_renderCubemap->posx()
        //m_shadowRenderer->shadowMap()
    );*/

    


    /*m_debugViewer->render(
        m_renderSetup->device(),
        m_renderSetup->currentRTV(),
        cmd,
        m_modelTransparentRenderer->colorAccumulateSRV()
        //m_shadowRenderer->shadowMap()
    );*/
}

void Engine::processShaderHotreload()
{
    CPU_MARKER("Process shader hotreload");
    if (m_renderSetup->shaderStorage().fileWatcher().hasChanges())
    {
        m_renderSetup->device().waitForIdle();
        if (m_renderSetup->shaderStorage().fileWatcher().processChanges())
        {
            // we had changes, pump the compilation messages to logging
            std::vector<std::string>& lastMessages = m_renderSetup->shaderStorage().fileWatcher().lastMessages();
            m_logWindow.pushMessages(lastMessages);
            lastMessages.clear();
        }
    }
}

void Engine::refresh()
{
    m_renderSetup->device().waitForIdle();
    m_window->refreshSize();
    m_virtualResolution = Vector2<int>{ m_renderSetup->device().width(), m_renderSetup->device().height() };

    auto weakChain = m_renderSetup->device().currentSwapChain();
    auto swapChain = weakChain.lock();
    if (swapChain)
    {
        m_renderSetup->releaseSwapChainSRVs();
        swapChain->resize(m_renderSetup->device() , { static_cast<uint32_t>(m_renderSetup->device().width()), static_cast<uint32_t>(m_renderSetup->device().height()) });
        m_renderSetup->createSwapChainSRVs();
    }

    m_rtv = m_renderSetup->device().createTextureRTV(TextureDescription()
        .width(m_virtualResolution.x)
        .height(m_virtualResolution.y)
        .format(engine::Format::Format_R16G16B16A16_FLOAT)
        .usage(ResourceUsage::GpuRenderTargetReadWrite)
        .name("main render target")
        .dimension(ResourceDimension::Texture2D)
        .optimizedClearValue({ 0.0f, 0.0f, 0.0f, 1.0f })
    );

    m_srv = m_renderSetup->device().createTextureSRV(m_rtv.texture());

    m_dsv = m_renderSetup->device().createTextureDSV(TextureDescription()
        .name("Depth Buffer")
        .format(Format::Format_D32_FLOAT)
        .width(m_virtualResolution.x)
        .height(m_virtualResolution.y)
        .usage(ResourceUsage::DepthStencil)
        .optimizedDepthClearValue(0.0f)
        .dimension(ResourceDimension::Texture2D)
    );
    m_dsvSRV = m_renderSetup->device().createTextureSRV(m_dsv);

    m_modelRenderer->resize(static_cast<uint32_t>(m_virtualResolution.x), static_cast<uint32_t>(m_virtualResolution.y));

    m_cameraSizeRefresh = true;

    m_camera.width(m_window->width());
    m_camera.height(m_window->height());

}

void Engine::onMouseMove(int x, int y)
{
    m_imguiRenderer->onMouseMove(x, y);
    if (!m_imguiRenderer->usesMouse())
    {
        m_cameraInput.onMouseMove(x, y);
    }
}

void Engine::onMouseDown(MouseButton button, int x, int y)
{
    m_imguiRenderer->onMouseDown(button, x, y);
    if (!m_imguiRenderer->usesMouse())
    {
        m_cameraInput.onMouseDown(button, x, y);
    }

    if(button == MouseButton::Left)
        m_modelRenderer->setSelectedObject(static_cast<int64_t>(m_modelRenderer->pickedObject(m_renderSetup->device())));
}

void Engine::onMouseUp(MouseButton button, int x, int y)
{
    m_imguiRenderer->onMouseUp(button, x, y);
    if (!m_imguiRenderer->usesMouse())
    {
        m_cameraInput.onMouseUp(button, x, y);
    }
}

void Engine::onMouseDoubleClick(MouseButton button, int x, int y)
{
    m_imguiRenderer->onMouseDoubleClick(button, x, y);
    if (!m_imguiRenderer->usesMouse())
    {
        m_cameraInput.onMouseDoubleClick(button, x, y);
    }
}

void Engine::shutdown()
{
    m_renderSetup->device().resourceCache().clear();
    m_scene.clear();
    m_renderSetup->shutdown();
    m_modelRenderer->clearResources();
    m_modelTransparentRenderer->clearResources();
    m_shadowRenderer->clearResources();
    m_renderCubemap->clearResources();
    m_postProcess->clearResources();

    m_lightData.reset(nullptr);
    m_modelRenderer.reset(nullptr);
    m_modelTransparentRenderer.reset(nullptr);
    m_shadowRenderer.reset(nullptr);
    m_renderCubemap.reset(nullptr);
    m_postProcess.reset(nullptr);
    m_imguiRenderer.reset(nullptr);
    m_renderSetup.reset(nullptr);
}

void Engine::onKeyDown(engine::Key key, const engine::ModifierState& modifierState)
{
    m_cameraInput.onKeyDown(key, modifierState);
}

void Engine::onKeyUp(engine::Key key, const engine::ModifierState& modifierState)
{
    m_cameraInput.onKeyUp(key, modifierState);
}

void Engine::cameraInputActive(bool active)
{
    m_cameraInputActive = active;
}

void CameraInput::onMouseMove(int x, int y)
{
    if (m_camera && m_mouseDown)
    {
        auto d = delta(x, y);
        
        applyYaw(static_cast<float>(-d.first));
        applyPitch(static_cast<float>(-d.second));
    }

    m_lastX = x;
    m_lastY = y;
}

std::pair<int, int> CameraInput::delta(int x, int y) const
{
    return { x - m_lastX, y - m_lastY };
}

void CameraInput::onMouseDown(engine::MouseButton button, int x, int y)
{
    if (button == engine::MouseButton::Right)
    {
        m_lastX = x;
        m_lastY = y;
        m_mouseDown = true;
    }
}

void CameraInput::onMouseUp(engine::MouseButton button, int x, int y)
{
    if (button == engine::MouseButton::Right)
    {
        m_lastX = x;
        m_lastY = y;
        m_mouseDown = false;
    }
}

void CameraInput::onMouseDoubleClick(engine::MouseButton /*button*/, int /*x*/, int /*y*/)
{

}

