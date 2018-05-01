#pragma once

#include "engine/graphics/Device.h"
#include "engine/graphics/Resources.h"
#include "engine/graphics/Sampler.h"
#include "shaders/core/tools/DepthDownsample.h"
#include "shaders/core/tools/DepthDownsampleSingle.h"
#include <vector>

#include "shaders/core/tools/cleartexture/ClearTexture2df.h"
#include "shaders/core/tools/copytexture/CopyTexture2df.h"

namespace engine
{
    class ShaderStorage;
    class CommandList;
    class DepthPyramid
    {
    public:
        DepthPyramid(
            Device& device,
            ShaderStorage& shaderStorage,
            int width,
            int height);

        void refresh(engine::Vector2<int> virtualResolution);

        TextureDSV& dsv()
        {
            return m_dsv;
        }

        TextureSRV& srv()
        {
            return m_dsvSRV;
        }

        TextureSRV& depth()
        {
            return m_depthPyramidSRV;
        }

        void performDownsample(CommandList& cmd);

    private:
        Device& m_device;
        engine::Pipeline<shaders::DepthDownsample> m_depthDownsample;
        engine::Pipeline<shaders::DepthDownsampleSingle> m_depthDownsampleSingle;

        engine::Pipeline<shaders::ClearTexture2df> m_clearTexture;
        engine::Pipeline<shaders::CopyTexture2df> m_copyTexture;

        Sampler m_sampler;
        int m_width;
        int m_height;
        int m_widthPow2;
        int m_heightPow2;
        bool m_needClean;

        TextureDSV m_dsv;
        TextureSRV m_dsvSRV;

        Texture m_depthPyramid;
        TextureSRV m_depthPyramidSRV;
        std::vector<TextureSRV> m_srvs;
        std::vector<TextureUAV> m_uavs;

    };
}
