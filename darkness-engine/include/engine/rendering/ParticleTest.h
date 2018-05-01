#pragma once

#include "engine/graphics/Resources.h"
#include "engine/graphics/Pipeline.h"
#include "engine/graphics/CommonNoDep.h"
#include "engine/rendering/LightData.h"
#include "shaders/core/particle/Particle.h"
#include <vector>
#include <string>
#include <chrono>

namespace engine
{
    class Device;
    class ShaderStorage;
    class ParticleTest
    {
    public:
        ParticleTest(Device& device, ShaderStorage& shaderStorage);
        void createParticles(Device& device, const std::vector<Vector3f>& positions);
        void render(
            Device& device,
            CommandList& cmd, 
            TextureRTV& rtv, 
            TextureSRV& dsvSRV,
            Camera& camera,
            const Matrix4f& cameraProjectionMatrix,
            const Matrix4f& cameraViewMatrix,
            const Matrix4f& jitterMatrix,
            LightData& lights,
            TextureSRV& shadowMap,
            BufferSRV& shadowVP);
    private:
        engine::Pipeline<shaders::Particle> m_particlePipeline;
        void loadTextures(Device& device);
        TextureSRV loadTexture(Device& device, std::string filepath);
        std::string createFilename(std::string basename, std::string extension, int number, int digits);
        BufferSRV m_particles;
        std::chrono::high_resolution_clock::time_point m_lastTime;
        float m_frameTimeIncrease;
        int m_currentFrame;
    };
}
