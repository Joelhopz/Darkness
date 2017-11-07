#pragma once

#include "shaders/ShaderTypes.h"
#include <memory>
{% if has_vertex_shader %}#include "{{vertex_shader_if}}"{% endif %}
{% if has_pixel_shader %}#include "{{pixel_shader_if}}"{% endif %}
{% if has_geometry_shader %}#include "{{geometry_shader_if}}"{% endif %}
{% if has_hull_shader %}#include "{{hull_shader_if}}"{% endif %}
{% if has_domain_shader %}#include "{{domain_shader_if}}"{% endif %}
{% if has_compute_shader %}#include "{{compute_shader_if}}"{% endif %}

namespace engine
{
    namespace implementation
    {
        class PipelineImpl;
    }

    namespace shaders
    {
        class Shader;

        class {{pipeline_type_name}} : public PipelineConfiguration
        {
        public:
            {% if has_vertex_shader -%}{{vertex_shader_type}} vs;{% endif %}
            {% if has_pixel_shader -%}{{pixel_shader_type}} ps;{% endif %}
            {% if has_geometry_shader -%}{{geometry_shader_type}} gs;{% endif %}
            {% if has_hull_shader -%}{{hull_shader_type}} hs;{% endif %}
            {% if has_domain_shader -%}{{domain_shader_type}} ds;{% endif %}
            {% if has_compute_shader -%}{{compute_shader_type}} cs;{% endif %}

            uint32_t descriptorCount() const override;

        private:
            friend class PipelineImpl;
            const Shader* vertexShader() const override;
            const Shader* pixelShader() const override;
            const Shader* geometryShader() const override;
            const Shader* hullShader() const override;
            const Shader* domainShader() const override;
            const Shader* computeShader() const override;

            bool hasVertexShader() const override;
            bool hasPixelShader() const override;
            bool hasGeometryShader() const override;
            bool hasHullShader() const override;
            bool hasDomainShader() const override;
            bool hasComputeShader() const override;
        };
    }
}
