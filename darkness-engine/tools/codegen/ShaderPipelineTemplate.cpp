#include "{{pipeline_interface_filepath}}"
#include "tools/Debug.h"

namespace engine
{
    namespace shaders
    {
        bool {{pipeline_type_name}}::hasVertexShader() const
        {
            return {{chas_vertex_shader}};
        }

        bool {{pipeline_type_name}}::hasPixelShader() const
        {
            return {{chas_pixel_shader}};
        }

        bool {{pipeline_type_name}}::hasGeometryShader() const
        {
            return {{chas_geometry_shader}};
        }

        bool {{pipeline_type_name}}::hasHullShader() const
        {
            return {{chas_hull_shader}};
        }

        bool {{pipeline_type_name}}::hasDomainShader() const
        {
            return {{chas_domain_shader}};
        }

        bool {{pipeline_type_name}}::hasComputeShader() const
        {
            return {{chas_compute_shader}};
        }

        uint32_t {{pipeline_type_name}}::descriptorCount() const
        {
            return 
                {% if has_vertex_shader -%}vs.descriptorCount()+{% endif %}
                {% if has_pixel_shader -%}ps.descriptorCount()+{% endif %}
                {% if has_geometry_shader -%}gs.descriptorCount()+{% endif %}
                {% if has_hull_shader -%}hs.descriptorCount()+{% endif %}
                {% if has_domain_shader -%}ds.descriptorCount()+{% endif %}
                {% if has_compute_shader -%}cs.descriptorCount()+{% endif %}
                0;
        }

        const Shader* {{pipeline_type_name}}::vertexShader() const
        {
            {% if has_vertex_shader -%}
            const Shader* shader = &vs;
            ASSERT(shader);
            return shader;
            {% else %}
            return nullptr;
            {% endif %}
        }

        const Shader* {{pipeline_type_name}}::pixelShader() const
        {
            {% if has_pixel_shader -%}
            const Shader* shader = &ps;
            ASSERT(shader);
            return shader;
            {% else %}
            return nullptr;
            {% endif %}
        }

        const Shader* {{pipeline_type_name}}::geometryShader() const
        {
            {% if has_geometry_shader -%}
            const Shader* shader = &gs;
            ASSERT(shader);
            return shader;
            {% else %}
            return nullptr;
            {% endif %}
        }

        const Shader* {{pipeline_type_name}}::hullShader() const
        {
            {% if has_hull_shader -%}
            const Shader* shader = &hs;
            ASSERT(shader);
            return shader;
            {% else %}
            return nullptr;
            {% endif %}
        }

        const Shader* {{pipeline_type_name}}::domainShader() const
        {
            {% if has_domain_shader -%}
            const Shader* shader = &ds;
            ASSERT(shader);
            return shader;
            {% else %}
            return nullptr;
            {% endif %}
        }

        const Shader* {{pipeline_type_name}}::computeShader() const
        {
            {% if has_compute_shader -%}
            const Shader* shader = &cs;
            ASSERT(shader);
            return shader;
            {% else %}
            return nullptr;
            {% endif %}
        }
    }
}
