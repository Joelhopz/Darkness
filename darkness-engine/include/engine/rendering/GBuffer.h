#pragma once

#include "engine/graphics/Resources.h"

namespace engine
{
    class Device;
    class CommandList;
    enum class GBufferType
    {
        Normal,
        Uv,
        Motion,
        InstanceId
    };

    class GBuffer
    {
    public:
        GBuffer(
            Device& device,
            int width,
            int height);

        TextureRTV& rtv(GBufferType type);
        TextureSRV& srv(GBufferType type);

        void clear(CommandList& cmd);
    private:
        TextureDescription m_desc;

        TextureRTV m_normalRTV;
        TextureSRV m_normalSRV;

        TextureRTV m_uvRTV;
        TextureSRV m_uvSRV;

        TextureRTV m_motionRTV;
        TextureSRV m_motionSRV;

        TextureRTV m_instanceIdRTV;
        TextureSRV m_instanceIdSRV;
    };
}