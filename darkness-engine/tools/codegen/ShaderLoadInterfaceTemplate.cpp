#include "{{ShaderLoadInterfaceHeader}}"
#include "engine/graphics/ShaderStorage.h"
#include "engine/graphics/Sampler.h"
#include "tools/ByteRange.h"
#include "tools/Debug.h"
#include <memory>

namespace engine
{
    namespace shaders
    {
#pragma warning( push )
#pragma warning( disable : 4702 )
        std::shared_ptr<const ShaderBinary> {{ShaderClass}}::load(const Device& device, ShaderStorage& storage) const
        {
            {% for combination in permutations %}
            if((true)
            {% for single_value in combination["list"] %}
            && ({{single_value["variable_name"]}} == {{single_value["value"]}})
            {% endfor %}
            )
            {
                return storage.loadShader(device, "{{BasePathAndFile}}_{{combination["id"]}}{{BaseExt}}", "{{ShaderSupportPath}}", {{loop.index}}-1, {
                    {% for define in combination["defines"] %}
                    "{{define}}"{% if not loop.last %},{% endif %}
                    {% endfor %}
                    });
            }
            {% else %}
            return storage.loadShader(device, "{{ShaderBinaryPath}}", "{{ShaderSupportPath}}", -1, {});
            {% endfor %}
            ASSERT(false, "Could not load the permutation necessary. This is a bug.");
            return {};
        }
#pragma warning( pop )

        {{ShaderClass}}::{{ShaderClass}}()
            : m_constantRange{
            {% if has_constants %}
            {% for item in constant_structures %}
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const {{item.name}}*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const {{item.name}}*>(this)) + sizeof({{item.name}})),
                    nullptr,
                    "{{item.name}}"
                }
                {% if not loop.last %}
                ,
                {% endif %}
            {% endfor %}
            {% endif %}
            }
            , m_inputParameters
            {
            {% for parameter in input_parameters %}
            ShaderInputParameter{"{{parameter.name}}", "{{parameter.semantic}}", "{{parameter.type}}"}
            {% if not loop.last %}
            ,
            {% endif %}
            {% endfor %}
            }
        {}

#pragma warning( push )
#pragma warning( disable : 4100 )
        {{ShaderClass}}::{{ShaderClass}}(const {{ShaderClass}}& cl)
            : m_constantRange{
            {% if has_constants %}
            {% for item in constant_structures %}
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const {{item.name}}*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const {{item.name}}*>(this)) + sizeof({{item.name}})),
                    nullptr,
                    "{{item.name}}"
                }
                {% if not loop.last %}
                ,
                {% endif %}
            {% endfor %}
            {% endif %}
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            {% for item in texture_srvs %}
            {{item.identifier}} = cl.{{item.identifier}};
            {% endfor %}

            {% for item in texture_uavs %}
            {{item.identifier}} = cl.{{item.identifier}};
            {% endfor %}

            {% for item in buffer_srvs %}
            {{item.identifier}} = cl.{{item.identifier}};
            {% endfor %}

            {% for item in buffer_uavs %}
            {{item.identifier}} = cl.{{item.identifier}};
            {% endfor %}

            {% for item in bindless_texture_srvs %}
            {{item.identifier}} = cl.{{item.identifier}};
            {% endfor %}

            {% for item in bindless_texture_uavs %}
            {{item.identifier}} = cl.{{item.identifier}};
            {% endfor %}

            {% for item in bindless_buffer_srvs %}
            {{item.identifier}} = cl.{{item.identifier}};
            {% endfor %}

            {% for item in bindless_buffer_uavs %}
            {{item.identifier}} = cl.{{item.identifier}};
            {% endfor %}

            {% for item in samplers %}
            {{item.identifier}} = cl.{{item.identifier}};
            {% endfor %}

            {% for item in options %}
            {{item["value"]}} = cl.{{item["value"]}};
            {% endfor -%}

            {% for enum_item in enums -%}
            {% for enum_type, enum_values in enum_item.items() -%}
            {% for enum_value in enum_values -%}
            {%- if loop.first and loop.last %}
            {{enum_type|lower}} = cl.{{enum_type|lower}};
            {% endif -%}
            {%- if loop.first and not loop.last %}
            {{enum_type|lower}} = cl.{{enum_type|lower}};
            {% endif -%}
            {% endfor -%}
            {% endfor -%}
            {% endfor %}

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        {{ShaderClass}}::{{ShaderClass}}({{ShaderClass}}&& cl)
            : m_constantRange{
            {% if has_constants %}
            {% for item in constant_structures %}
                ConstantRange{
                    tools::ByteRange(
                        reinterpret_cast<const uint8_t*>(static_cast<const {{item.name}}*>(this)),
                        reinterpret_cast<const uint8_t*>(static_cast<const {{item.name}}*>(this)) + sizeof({{item.name}})),
                    nullptr,
                    "{{item.name}}"
                }
                {% if not loop.last %}
                ,
                {% endif %}
            {% endfor %}
            {% endif %}
            }
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            {% for item in texture_srvs %}
            {{item.identifier}} = std::move(cl.{{item.identifier}});
            {% endfor %}

            {% for item in texture_uavs %}
            {{item.identifier}} = std::move(cl.{{item.identifier}});
            {% endfor %}

            {% for item in buffer_srvs %}
            {{item.identifier}} = std::move(cl.{{item.identifier}});
            {% endfor %}

            {% for item in buffer_uavs %}
            {{item.identifier}} = std::move(cl.{{item.identifier}});
            {% endfor %}

            {% for item in bindless_texture_srvs %}
            {{item.identifier}} = std::move(cl.{{item.identifier}});
            {% endfor %}

            {% for item in bindless_texture_uavs %}
            {{item.identifier}} = std::move(cl.{{item.identifier}});
            {% endfor %}

            {% for item in bindless_buffer_srvs %}
            {{item.identifier}} = std::move(cl.{{item.identifier}});
            {% endfor %}

            {% for item in bindless_buffer_uavs %}
            {{item.identifier}} = std::move(cl.{{item.identifier}});
            {% endfor %}

            {% for item in samplers %}
            {{item.identifier}} = std::move(cl.{{item.identifier}});
            {% endfor %}

            {% for item in options %}
            {{item["value"]}} = std::move(cl.{{item["value"]}});
            {% endfor -%}

            {% for enum_item in enums -%}
            {% for enum_type, enum_values in enum_item.items() -%}
            {% for enum_value in enum_values -%}
            {%- if loop.first and loop.last %}
            {{enum_type|lower}} = cl.{{enum_type|lower}};
            {% endif -%}
            {%- if loop.first and not loop.last %}
            {{enum_type|lower}} = cl.{{enum_type|lower}};
            {% endif -%}
            {% endfor -%}
            {% endfor -%}
            {% endfor %}

        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        {{ShaderClass}}& {{ShaderClass}}::operator=(const {{ShaderClass}}& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = cl.m_constantRange[i].buffer;
            }

            {% for item in texture_srvs %}
            {{item.identifier}} = cl.{{item.identifier}};
            {% endfor %}

            {% for item in texture_uavs %}
            {{item.identifier}} = cl.{{item.identifier}};
            {% endfor %}

            {% for item in buffer_srvs %}
            {{item.identifier}} = cl.{{item.identifier}};
            {% endfor %}

            {% for item in buffer_uavs %}
            {{item.identifier}} = cl.{{item.identifier}};
            {% endfor %}

            {% for item in bindless_texture_srvs %}
            {{item.identifier}} = cl.{{item.identifier}};
            {% endfor %}

            {% for item in bindless_texture_uavs %}
            {{item.identifier}} = cl.{{item.identifier}};
            {% endfor %}

            {% for item in bindless_buffer_srvs %}
            {{item.identifier}} = cl.{{item.identifier}};
            {% endfor %}

            {% for item in bindless_buffer_uavs %}
            {{item.identifier}} = cl.{{item.identifier}};
            {% endfor %}

            {% for item in samplers %}
            {{item.identifier}} = cl.{{item.identifier}};
            {% endfor %}

            {% for item in options %}
            {{item["value"]}} = cl.{{item["value"]}};
            {% endfor -%}

            {% for enum_item in enums -%}
            {% for enum_type, enum_values in enum_item.items() -%}
            {% for enum_value in enum_values -%}
            {%- if loop.first and loop.last %}
            {{enum_type|lower}} = cl.{{enum_type|lower}};
            {% endif -%}
            {%- if loop.first and not loop.last %}
            {{enum_type|lower}} = cl.{{enum_type|lower}};
            {% endif -%}
            {% endfor -%}
            {% endfor -%}
            {% endfor %}

            return *this;
        }
#pragma warning( pop )

#pragma warning( push )
#pragma warning( disable : 4100 )
        {{ShaderClass}}& {{ShaderClass}}::operator=({{ShaderClass}}&& cl)
        {
            for (int i = 0; i < m_constantRange.size(); ++i)
            {
                m_constantRange[i].buffer = std::move(cl.m_constantRange[i].buffer);
            }

            {% for item in texture_srvs %}
            {{item.identifier}} = std::move(cl.{{item.identifier}});
            {% endfor %}

            {% for item in texture_uavs %}
            {{item.identifier}} = std::move(cl.{{item.identifier}});
            {% endfor %}

            {% for item in buffer_srvs %}
            {{item.identifier}} = std::move(cl.{{item.identifier}});
            {% endfor %}

            {% for item in buffer_uavs %}
            {{item.identifier}} = std::move(cl.{{item.identifier}});
            {% endfor %}

            {% for item in bindless_texture_srvs %}
            {{item.identifier}} = std::move(cl.{{item.identifier}});
            {% endfor %}

            {% for item in bindless_texture_uavs %}
            {{item.identifier}} = std::move(cl.{{item.identifier}});
            {% endfor %}

            {% for item in bindless_buffer_srvs %}
            {{item.identifier}} = std::move(cl.{{item.identifier}});
            {% endfor %}

            {% for item in bindless_buffer_uavs %}
            {{item.identifier}} = std::move(cl.{{item.identifier}});
            {% endfor %}

            {% for item in samplers %}
            {{item.identifier}} = std::move(cl.{{item.identifier}});
            {% endfor %}

            {% for item in options %}
            {{item["value"]}} = std::move(cl.{{item["value"]}});
            {% endfor -%}

            {% for enum_item in enums -%}
            {% for enum_type, enum_values in enum_item.items() -%}
            {% for enum_value in enum_values -%}
            {%- if loop.first and loop.last %}
            {{enum_type|lower}} = cl.{{enum_type|lower}};
            {% endif -%}
            {%- if loop.first and not loop.last %}
            {{enum_type|lower}} = cl.{{enum_type|lower}};
            {% endif -%}
            {% endfor -%}
            {% endfor -%}
            {% endfor %}

            return *this;
        }
#pragma warning( pop )

        std::vector<std::string> {{ShaderClass}}::textureSrvNames() const
        {
            return {
                {% for item in texture_srvs %}
                "{{item.identifier}}"
                {% if not loop.last %}
                ,
                {% endif %}
                {% endfor %}
            };
        }

        std::vector<std::string> {{ShaderClass}}::textureUavNames() const
        {
            return {
                {% for item in texture_uavs %}
                "{{item.identifier}}"
                {% if not loop.last %}
                ,
                {% endif %}
                {% endfor %}
            };
        }

        std::vector<std::string> {{ShaderClass}}::bufferSrvNames() const
        {
            return {
                {% for item in buffer_srvs %}
                "{{item.identifier}}"
                {% if not loop.last %}
                ,
                {% endif %}
                {% endfor %}
            };
        }

        std::vector<std::string> {{ShaderClass}}::bufferUavNames() const
        {
            return {
                {% for item in buffer_uavs %}
                "{{item.identifier}}"
                {% if not loop.last %}
                ,
                {% endif %}
                {% endfor %}
            };
        }

        std::vector<std::string> {{ShaderClass}}::samplerNames() const
        {
            return {
                {% for item in samplers %}
                "{{item.identifier}}"
                {% if not loop.last %}
                ,
                {% endif %}
                {% endfor %}
            };
        }

        std::vector<std::string> {{ShaderClass}}::srvNames() const
        {
            return {
                {% for item in srvs %}
                "{{item.identifier}}"
                {% if not loop.last %}
                ,
                {% endif %}
                {% endfor %}
            };
        }

        std::vector<std::string> {{ShaderClass}}::uavNames() const
        {
            return {
                {% for item in uavs %}
                "{{item.identifier}}"
                {% if not loop.last %}
                ,
                {% endif %}
                {% endfor %}
            };
        }

#pragma warning( push )
#pragma warning( disable : 4100 )
        engine::ResourceDimension {{ShaderClass}}::textureDimension(const std::string& name) const
        {
            {% for item in dimensions %}
            if("{{item.identifier}}" == name) return engine::ResourceDimension::{{item.dimension}};
            {% endfor %}
            return engine::ResourceDimension::Unknown;
        }
#pragma warning( pop )

        std::vector<TextureSRV> {{ShaderClass}}::texture_srvs() const
        {
            std::vector<TextureSRV> result;
            {% for item in texture_srvs %}
            result.emplace_back({{item.identifier}});
            {% endfor %}
            return result;
        }

        std::vector<TextureUAV> {{ShaderClass}}::texture_uavs() const
        {
            std::vector<TextureUAV> result;
            {% for item in texture_uavs %}
            result.emplace_back({{item.identifier}});
            {% endfor %}
            return result;
        }

        std::vector<BufferSRV> {{ShaderClass}}::buffer_srvs() const
        {
            std::vector<BufferSRV> result;
            {% for item in buffer_srvs %}
            result.emplace_back({{item.identifier}});
            {% endfor %}
            return result;
        }

        std::vector<BufferUAV> {{ShaderClass}}::buffer_uavs() const
        {
            std::vector<BufferUAV> result;
            {% for item in buffer_uavs %}
            result.emplace_back({{item.identifier}});
            {% endfor %}
            return result;
        }

        std::vector<TextureBindlessSRV> {{ShaderClass}}::bindless_texture_srvs() const
        {
            std::vector<TextureBindlessSRV> result;
            {% for item in bindless_texture_srvs %}
            result.emplace_back({{item.identifier}});
            {% endfor %}
            return result;
        }

        std::vector<TextureBindlessUAV> {{ShaderClass}}::bindless_texture_uavs() const
        {
            std::vector<TextureBindlessUAV> result;
            {% for item in bindless_texture_uavs %}
            result.emplace_back({{item.identifier}});
            {% endfor %}
            return result;
        }

        std::vector<BufferBindlessSRV> {{ShaderClass}}::bindless_buffer_srvs() const
        {
            std::vector<BufferBindlessSRV> result;
            {% for item in bindless_buffer_srvs %}
            result.emplace_back({{item.identifier}});
            {% endfor %}
            return result;
        }

        std::vector<BufferBindlessUAV> {{ShaderClass}}::bindless_buffer_uavs() const
        {
            std::vector<BufferBindlessUAV> result;
            {% for item in bindless_buffer_uavs %}
            result.emplace_back({{item.identifier}});
            {% endfor %}
            return result;
        }

        std::vector<Shader::ConstantRange>& {{ShaderClass}}::constants()
        {
            return m_constantRange;
        }

        std::vector<Sampler> {{ShaderClass}}::samplers() const
        {
            std::vector<Sampler> result;
            {% for item in samplers %}
            result.emplace_back({{item.identifier}});
            {% endfor %}
            return result;
        }

        const std::vector<ShaderInputParameter>& {{ShaderClass}}::inputParameters() const
        {
            return m_inputParameters;
        }

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4100 )

        bool {{ShaderClass}}::hasTextureSrv(const std::string& name) const
        {
            {% if has_texture_srvs %}
            {% for item in texture_srvs %}
            if(name == std::string("{{item.identifier}}")) return true;
            {% endfor %}
            {% endif %}
            return false;
        }

        bool {{ShaderClass}}::hasTextureUav(const std::string& name) const
        {
            {% if has_texture_uavs %}
            {% for item in texture_uavs %}
            if (name == std::string("{{item.identifier}}")) return true;
            {% endfor %}
            {% endif %}
            return false;
        }

        bool {{ShaderClass}}::hasBufferSrv(const std::string& name) const
        {
            {% if has_buffer_srvs %}
            {% for item in buffer_srvs %}
            if (name == std::string("{{item.identifier}}")) return true;
            {% endfor %}
            {% endif %}
            return false;
        }

        bool {{ShaderClass}}::hasBufferUav(const std::string& name) const
        {
            {% if has_buffer_uavs %}
            {% for item in buffer_uavs %}
            if (name == std::string("{{item.identifier}}")) return true;
            {% endfor %}
            {% endif %}
            return false;
        }

        bool {{ShaderClass}}::hasBindlessTextureSrv(const std::string& name) const
        {
            {% if has_bindless_texture_srvs %}
            {% for item in bindless_texture_srvs %}
            if (name == std::string("{{item.identifier}}")) return true;
            {% endfor %}
            {% endif %}
            return false;
        }

        bool {{ShaderClass}}::hasBindlessTextureUav(const std::string& name) const
        {
            {% if has_bindless_texture_uavs %}
            {% for item in bindless_texture_uavs %}
            if (name == std::string("{{item.identifier}}")) return true;
            {% endfor %}
            {% endif %}
            return false;
        }

        bool {{ShaderClass}}::hasBindlessBufferSrv(const std::string& name) const
        {
            {% if has_bindless_buffer_srvs %}
            {% for item in bindless_buffer_srvs %}
            if (name == std::string("{{item.identifier}}")) return true;
            {% endfor %}
            {% endif %}
            return false;
        }

        bool {{ShaderClass}}::hasBindlessBufferUav(const std::string& name) const
        {
            {% if has_bindless_buffer_uavs %}
            {% for item in bindless_buffer_uavs %}
            if (name == std::string("{{item.identifier}}")) return true;
            {% endfor %}
            {% endif %}
            return false;
        }

        const TextureSRV& {{ShaderClass}}::textureSrv(const std::string& name) const
        {
            {% if has_texture_srvs %}
            {% for item in texture_srvs %}
            if(name == std::string("{{item.identifier}}")) return {{item.identifier}};
            {% endfor %}
            {% endif %}
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureSRV();
        }

        const TextureUAV& {{ShaderClass}}::textureUav(const std::string& name) const
        {
            {% if has_texture_uavs %}
            {% for item in texture_uavs %}
            if(name == std::string("{{item.identifier}}")) return {{item.identifier}};
            {% endfor %}
            {% endif %}
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureUAV();
        }

        const BufferSRV& {{ShaderClass}}::bufferSrv(const std::string& name) const
        {
            {% if has_buffer_srvs %}
            {% for item in buffer_srvs %}
            if(name == std::string("{{item.identifier}}")) return {{item.identifier}};
            {% endfor %}
            {% endif %}
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferSRV();
        }

        const BufferUAV& {{ShaderClass}}::bufferUav(const std::string& name) const
        {
            {% if has_buffer_uavs %}
            {% for item in buffer_uavs %}
            if(name == std::string("{{item.identifier}}")) return {{item.identifier}};
            {% endfor %}
            {% endif %}
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferUAV();
        }

        void {{ShaderClass}}::textureSrv(const std::string& name, TextureSRV& texture)
        {
            {% if has_texture_srvs %}
            {% for item in texture_srvs %}
            if(name == std::string("{{item.identifier}}")) { {{item.identifier}} = texture; return; }
            {% endfor %}
            {% endif %}
            ASSERT(false, "Tried to set non-existing resource");
        }

        void {{ShaderClass}}::textureUav(const std::string& name, TextureUAV& texture)
        {
            {% if has_texture_uavs %}
            {% for item in texture_uavs %}
            if(name == std::string("{{item.identifier}}")) { {{item.identifier}} = texture; return; }
            {% endfor %}
            {% endif %}
            ASSERT(false, "Tried to set non-existing resource");
        }

        void {{ShaderClass}}::bufferSrv(const std::string& name, BufferSRV& buffer)
        {
            {% if has_buffer_srvs %}
            {% for item in buffer_srvs %}
            if(name == std::string("{{item.identifier}}")) { {{item.identifier}} = buffer; return; }
            {% endfor %}
            {% endif %}
            ASSERT(false, "Tried to set non-existing resource");
        }

        void {{ShaderClass}}::bufferUav(const std::string& name, BufferUAV& buffer)
        {
            {% if has_buffer_uavs %}
            {% for item in buffer_uavs %}
            if(name == std::string("{{item.identifier}}")) { {{item.identifier}} = buffer; return; }
            {% endfor %}
            {% endif %}
            ASSERT(false, "Tried to set non-existing resource");
        }

        const Sampler& {{ShaderClass}}::sampler(const std::string& name) const
        {
            {% if has_samplers %}
            {% for sampler in samplers %}
            if(name == std::string("{{sampler.identifier}}")) return {{sampler.identifier}};
            {% endfor %}
            {% endif %}
            ASSERT(false, "Tried to look for non-existing resource");
            return Sampler();
        }

        const TextureBindlessSRV& {{ShaderClass}}::bindlessTextureSrv(const std::string& name) const
        {
            {% if has_bindless_texture_srvs %}
            {% for item in bindless_texture_srvs %}
            if(name == std::string("{{item.identifier}}")) return {{item.identifier}};
            {% endfor %}
            {% endif %}
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessSRV();
        }

        const TextureBindlessUAV& {{ShaderClass}}::bindlessTextureUav(const std::string& name) const
        {
            {% if has_bindless_texture_uavs %}
            {% for item in bindless_texture_uavs %}
            if(name == std::string("{{item.identifier}}")) return {{item.identifier}};
            {% endfor %}
            {% endif %}
            ASSERT(false, "Tried to look for non-existing resource");
            return TextureBindlessUAV();
        }

        const BufferBindlessSRV& {{ShaderClass}}::bindlessBufferSrv(const std::string& name) const
        {
            {% if has_bindless_buffer_srvs %}
            {% for item in bindless_buffer_srvs %}
            if(name == std::string("{{item.identifier}}")) return {{item.identifier}};
            {% endfor %}
            {% endif %}
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessSRV();
        }

        const BufferBindlessUAV& {{ShaderClass}}::bindlessBufferUav(const std::string& name) const
        {
            {% if has_bindless_buffer_uavs %}
            {% for item in bindless_buffer_uavs %}
            if(name == std::string("{{item.identifier}}")) return {{item.identifier}};
            {% endfor %}
            {% endif %}
            ASSERT(false, "Tried to look for non-existing resource");
            return BufferBindlessUAV();
        }

#pragma warning( pop )

        uint32_t {{ShaderClass}}::descriptorCount() const
        {
            return {{descriptor_count}};
        }
    }
}
