#include "engine/rendering/LightData.h"
#include "tools/ByteRange.h"

using namespace tools;

namespace engine
{
    void LightData::updateLightInfo(Device& device, CommandList& commandList, const std::vector<FlatSceneLightNode>& lights)
    {
        bool sizeChanged = static_cast<uint32_t>(lights.size()) != m_lightCount;
        std::vector<Vector4f> positions;
        std::vector<Vector4f> directions;
        std::vector<Vector4f> colors;
        std::vector<float> intensities;
        std::vector<float> ranges;
        std::vector<unsigned int> types;
        std::vector<Vector4f> parameters;

        bool positionsChanged = false;
        bool directionsChanged = false;
        bool colorsChanged = false;
        bool intensitiesChanged = false;
        bool parametersChanged = false;
        bool rangesChanged = false;
        bool typesChanged = false;
        for (auto&& light : lights)
        {
            positionsChanged |= light.positionChanged;
            directionsChanged |= light.rotationChanged;
            colorsChanged |= light.light->colorChanged(true);
            intensitiesChanged |= light.light->intensityChanged(true);
            rangesChanged |= light.light->rangeChanged(true);
            typesChanged |= light.light->lightTypeChanged(true);
            parametersChanged |= light.light->lightParametersChanged(true);
        }

        m_changeHappened =
            //positionsChanged |
            //directionsChanged |
            //colorsChanged |
            //intensitiesChanged |
            //rangesChanged |
            typesChanged |
            parametersChanged;

        m_transforms.clear();
        m_positions.clear();
        m_directions.clear();
        m_ranges.clear();
        m_types.clear();
        m_shadowCaster.clear();

        for (auto&& light : lights)
        {
            if (sizeChanged || positionsChanged)
                positions.emplace_back(Vector4f(light.position, 1.0f));

            if (sizeChanged || directionsChanged)
                directions.emplace_back(Vector4f(light.direction, 1.0f));

            if (sizeChanged || colorsChanged)
                colors.emplace_back(Vector4f(light.light->color(), 1.0f));

            if (sizeChanged || intensitiesChanged)
                intensities.emplace_back(light.light->intensity());

            if (sizeChanged || rangesChanged)
                ranges.emplace_back(light.light->range());

            if (sizeChanged || typesChanged)
                types.emplace_back(static_cast<unsigned int>(light.light->lightType()));

            if (sizeChanged || parametersChanged)
                parameters.emplace_back(light.light->parameters());

            m_transforms.emplace_back(light.transform);
            m_positions.emplace_back(light.position);
            m_directions.emplace_back(light.direction);
            m_ranges.emplace_back(light.range);
            m_types.emplace_back(static_cast<unsigned int>(light.type));
            m_shadowCaster.emplace_back(light.shadowCaster);
        }

        if (sizeChanged)
        {
            m_lightCount = static_cast<uint32_t>(lights.size());

            if (m_lightCount > 0)
            {
                m_lightWorlPositions = std::make_shared<BufferSRV>(device.createBufferSRV(BufferDescription()
                    .name("lightPositions")
                    .format(Format::R32G32B32A32_FLOAT)
                    .setInitialData(BufferDescription::InitialData(positions))));

                m_lightDirections = std::make_shared<BufferSRV>(device.createBufferSRV(BufferDescription()
                    .name("lightDirections")
                    .format(Format::R32G32B32A32_FLOAT)
                    .setInitialData(BufferDescription::InitialData(directions))));

                m_lightColors = std::make_shared<BufferSRV>(device.createBufferSRV(BufferDescription()
                    .name("lightColors")
                    .format(Format::R32G32B32A32_FLOAT)
                    .setInitialData(BufferDescription::InitialData(colors))));

                m_lightIntensities = std::make_shared<BufferSRV>(device.createBufferSRV(BufferDescription()
                    .name("lightIntensities")
                    .format(Format::R32_FLOAT)
                    .setInitialData(BufferDescription::InitialData(intensities))));

                m_lightRanges = std::make_shared<BufferSRV>(device.createBufferSRV(BufferDescription()
                    .name("lightRanges")
                    .format(Format::R32_FLOAT)
                    .setInitialData(BufferDescription::InitialData(ranges))));

                m_lightTypes = std::make_shared<BufferSRV>(device.createBufferSRV(BufferDescription()
                    .name("lightTypes")
                    .format(Format::R32_UINT)
                    .setInitialData(BufferDescription::InitialData(types))));

                m_lightParameters = std::make_shared<BufferSRV>(device.createBufferSRV(BufferDescription()
                    .name("lightParameters")
                    .format(Format::R32G32B32A32_FLOAT)
                    .setInitialData(BufferDescription::InitialData(parameters))));

                m_saved.push(*m_lightWorlPositions);
                m_saved.push(*m_lightDirections);
                m_saved.push(*m_lightColors);
                m_saved.push(*m_lightIntensities);
                m_saved.push(*m_lightRanges);
                m_saved.push(*m_lightTypes);
                m_saved.push(*m_lightParameters);

            }
            else
            {
                m_lightWorlPositions = nullptr;
                m_lightDirections = nullptr;
                m_lightColors = nullptr;
                m_lightIntensities = nullptr;
                m_lightRanges = nullptr;
                m_lightTypes = nullptr;
                m_lightParameters = nullptr;
            }
        }
        else
        {
            if (m_lightCount == 0)
            {
                m_lightWorlPositions = nullptr;
                m_lightDirections = nullptr;
                m_lightColors = nullptr;
                m_lightIntensities = nullptr;
                m_lightRanges = nullptr;
                m_lightTypes = nullptr;
                m_lightParameters = nullptr;
            }
            else
            {
                // update existing textures
                if (positionsChanged)
                    device.uploadBuffer(commandList, *m_lightWorlPositions, ByteRange{ positions });

                if (directionsChanged)
                    device.uploadBuffer(commandList, *m_lightDirections, ByteRange{ directions });

                if (colorsChanged)
                    device.uploadBuffer(commandList, *m_lightColors, ByteRange{ colors });

                if (intensitiesChanged)
                    device.uploadBuffer(commandList, *m_lightIntensities, ByteRange{ intensities });

                if (rangesChanged)
                    device.uploadBuffer(commandList, *m_lightRanges, ByteRange{ ranges });

                if (typesChanged)
                    device.uploadBuffer(commandList, *m_lightTypes, ByteRange{ types });

                if (parametersChanged)
                    device.uploadBuffer(commandList, *m_lightParameters, ByteRange{ parameters });
            }
        }

        while (m_saved.size() > 14)
        {
            m_saved.pop();
        }
    }
}
