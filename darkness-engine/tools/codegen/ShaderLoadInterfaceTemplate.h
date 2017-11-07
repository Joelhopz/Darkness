#pragma once

#include "engine/graphics/Resources.h"
#include "engine/graphics/Sampler.h"
#include "shaders/ShaderTypes.h"
#include <vector>
#include <memory>
#include <string>

namespace engine
{
    class ShaderBinary;
    class ShaderStorage;
    class Device;
    class Sampler;

    namespace implementation
    {
        class PipelineImpl;
        class CommandListImpl;
    }

    namespace shaders
    {
        class {{shader_pipeline_configuration_class}};

        {% if has_constants -%}
        namespace {% filter lower %}{{ShaderClass}}_namespace{% endfilter %}
        {
        {% for item in constant_structures -%}
                struct {{item.name}}
            {
                {% for structure_identifier in item.identifiers -%}
                {{structure_identifier.type}} {{structure_identifier.name}};
                {% endfor -%}
};
        {% endfor -%}
}
        {% endif -%}

        class {{ShaderClass}} : public {{class_type}}
            {% if has_constants -%}
            {% for item in constant_structures -%}
            , public {% filter lower %}{{ShaderClass}}_namespace{% endfilter %}::{{item.name}}
            {% endfor -%}{% endif -%}
        {
        public:

            {{ShaderClass}}();

            {% for item in texture_srvs -%}
            {{item.type}} {{item.identifier}};
            {% endfor -%}
            {% for item in texture_uavs -%}
            {{item.type}} {{item.identifier}};
            {% endfor -%}

            {% for item in bindless_texture_srvs -%}
            {{item.type}} {{item.identifier}};
            {% endfor -%}
            {% for item in bindless_texture_uavs -%}
            {{item.type}} {{item.identifier}};
            {% endfor -%}

            {% for item in buffer_srvs -%}
            {{item.type}} {{item.identifier}};
            {% endfor -%}
            {% for item in buffer_uavs -%}
            {{item.type}} {{item.identifier}};
            {% endfor %}

            {% for item in bindless_buffer_srvs -%}
            {{item.type}} {{item.identifier}};
            {% endfor -%}
            {% for item in bindless_buffer_uavs -%}
            {{item.type}} {{item.identifier}};
            {% endfor %}

            {% for item in samplers -%}
            Sampler {{item.identifier}};
            {% endfor -%}

        protected:
            friend class implementation::CommandListImpl;
            const TextureSRV& textureSrv(const std::string& name) const override;
            const TextureUAV& textureUav(const std::string& name) const override;
            const BufferSRV& bufferSrv(const std::string& name) const override;
            const BufferUAV& bufferUav(const std::string& name) const override;
            const Sampler& sampler(const std::string& name) const override;
            
            const TextureBindlessSRV& bindlessTextureSrv(const std::string& name) const override;
            const TextureBindlessUAV& bindlessTextureUav(const std::string& name) const override;
            const BufferBindlessSRV& bindlessBufferSrv(const std::string& name) const override;
            const BufferBindlessUAV& bindlessBufferUav(const std::string& name) const override;

            bool hasTextureSrv(const std::string& name) const override;
            bool hasTextureUav(const std::string& name) const override;
            bool hasBufferSrv(const std::string& name) const override;
            bool hasBufferUav(const std::string& name) const override;

            bool hasBindlessTextureSrv(const std::string& name) const override;
            bool hasBindlessTextureUav(const std::string& name) const override;
            bool hasBindlessBufferSrv(const std::string& name) const override;
            bool hasBindlessBufferUav(const std::string& name) const override;

        private:
            friend class implementation::PipelineImpl;
            std::shared_ptr<const ShaderBinary> load(const Device& device, ShaderStorage& storage) const override;
            std::vector<TextureSRV> texture_srvs() const override;
            std::vector<TextureUAV> texture_uavs() const override;
            std::vector<BufferSRV> buffer_srvs() const override;
            std::vector<BufferUAV> buffer_uavs() const override;
            
            std::vector<TextureBindlessSRV> bindless_texture_srvs() const override;
            std::vector<TextureBindlessUAV> bindless_texture_uavs() const override;
            std::vector<BufferBindlessSRV> bindless_buffer_srvs() const override;
            std::vector<BufferBindlessUAV> bindless_buffer_uavs() const override;
            
            std::vector<Shader::ConstantRange>& constants() override;
            std::vector<Sampler> samplers() const override;
        private:
            friend class {{shader_pipeline_configuration_class}};
            uint32_t descriptorCount() const;
            std::vector<Shader::ConstantRange> m_constantRange;
        };
    }
}
