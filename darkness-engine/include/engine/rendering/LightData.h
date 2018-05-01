#pragma once

#include "engine/graphics/CommandList.h"
#include "engine/Scene.h"
#include "engine/primitives/Matrix4.h"
#include <vector>
#include <queue>

namespace engine
{
    class LightData
    {
    public:
        void updateLightInfo(Device& device, CommandList& commandList, const std::vector<FlatSceneLightNode>& lights);

        uint32_t count() const
        {
            return m_lightCount;
        }

        BufferSRV& worldPositions()
        {
            return *m_lightWorlPositions;
        }
        BufferSRV& directions()
        {
            return *m_lightDirections;
        }
        BufferSRV& colors()
        {
            return *m_lightColors;
        }
        BufferSRV& intensities()
        {
            return *m_lightIntensities;
        }
        BufferSRV& ranges()
        {
            return *m_lightRanges;
        }
        BufferSRV& types()
        {
            return *m_lightTypes;
        }

        BufferSRV& parameters()
        {
            return *m_lightParameters;
        }

        const std::vector<engine::Matrix4f>& transforms() const
        {
            return m_transforms;
        }

        const std::vector<engine::Vector3f>& positions() const
        {
            return m_positions;
        }

        const std::vector<engine::Vector3f>& directionVectors() const
        {
            return m_directions;
        }

        const std::vector<float>& cpuranges() const
        {
            return m_ranges;
        }

        const std::vector<bool>& shadowCaster() const
        {
            return m_shadowCaster;
        }

        const std::vector<unsigned int>& engineTypes() const
        {
            return m_types;
        }

        bool changeHappened() const
        {
            return m_changeHappened;
        }

    private:
        bool m_changeHappened = false;
        uint32_t m_lightCount;
        std::shared_ptr<BufferSRV> m_lightWorlPositions;
        std::shared_ptr<BufferSRV> m_lightDirections;
        std::shared_ptr<BufferSRV> m_lightColors;
        std::shared_ptr<BufferSRV> m_lightIntensities;
        std::shared_ptr<BufferSRV> m_lightRanges;
        std::shared_ptr<BufferSRV> m_lightTypes;
        std::shared_ptr<BufferSRV> m_lightParameters;
        std::queue<BufferSRV> m_saved;

        std::vector<engine::Matrix4f> m_transforms;
        std::vector<engine::Vector3f> m_positions;
        std::vector<engine::Vector3f> m_directions;
        std::vector<float> m_ranges;
        std::vector<unsigned int> m_types;
        std::vector<bool> m_shadowCaster;
    };
}
