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
        std::shared_ptr<const ShaderBinary> {{ShaderClass}}::load(const Device& device, ShaderStorage& storage) const
        {
            return storage.loadShader(device, "{{ShaderBinaryPath}}", "{{ShaderSupportPath}}");
        }

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
        {}

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

// warning C4172: returning address of local variable or temporary
// this will never happen as the name will always match the correct resource
#pragma warning( push )
#pragma warning( disable : 4172 )

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
