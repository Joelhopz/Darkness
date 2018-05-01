#include "engine/graphics/dx12/DX12ShaderBinary.h"
#include "engine/graphics/dx12/DX12Headers.h"
#include "engine/graphics/Device.h"
#include "tools/Recompile.h"
#include "tools/StringTools.h"
#include "tools/PathTools.h"
#include "tools/Debug.h"

#include <iostream>
#include <fstream>

using namespace std;

namespace engine
{
    namespace implementation
    {
        ShaderBinaryImpl::ShaderBinaryImpl(
            const Device&, 
            const std::string& binaryPath, 
            const std::string& supportPath, 
            int permutationId,
            const std::vector<std::string>& defines,
            platform::FileWatcher& watcher)
            : m_buffer{ nullptr }
            , m_shaderBinary{ new D3D12_SHADER_BYTECODE() }
            , m_shaderSupport{ supportPath }
            , m_watchHandle{ watcher.addWatch(m_shaderSupport.file, [this](const std::string& changedPath)->std::string { return this->onFileChange(changedPath); }) }
            , m_change{}
            , m_permutationId{ permutationId }
            , m_defines{ defines }
        {
            // read binary
            readFile(binaryPath);
        }

        std::string ShaderBinaryImpl::onFileChange(const std::string& /*path*/)
        {
            auto res = recompile(m_shaderSupport, m_permutationId, m_defines);
            auto binFile = permutationName(m_shaderSupport.binaryFile, m_permutationId);

            readFile(binFile);
            for (auto&& change : m_change)
            {
                change.second();
            }
            return res;
        }

        void ShaderBinaryImpl::registerForChange(void* client, std::function<void(void)> change) const
        {
            m_change[client] = change;
        }

        void ShaderBinaryImpl::unregisterForChange(void* client) const
        {
            m_change.erase(client);
        }

        void ShaderBinaryImpl::readFile(const std::string& path)
        {
            ASSERT(path.data());
            ifstream shaderFile;
            shaderFile.open(path, ios::in | ios::binary);
            if (shaderFile.is_open())
            {
                auto begin = shaderFile.tellg();
                shaderFile.seekg(0, ios::end);
                auto end = shaderFile.tellg();
                size_t size = static_cast<size_t>(end - begin);
                m_buffer.reset(new char[size]);
                m_shaderBinary->pShaderBytecode = m_buffer.get();
                m_shaderBinary->BytecodeLength = size;
                shaderFile.seekg(0, ios::beg);
                shaderFile.read(m_buffer.get(), end - begin);
                shaderFile.close();
            }
        }

        ShaderBinaryImpl::~ShaderBinaryImpl()
        {
            if (m_shaderBinary)
            {
                delete m_shaderBinary;
                m_shaderBinary = nullptr;
            }
        }

        D3D12_SHADER_BYTECODE& ShaderBinaryImpl::native() const
        {
            return *m_shaderBinary;
        }

        ShaderBinaryImpl::ShaderBinaryImpl(const ShaderBinaryImpl& shaderBinary)
            : m_buffer{ nullptr }
            , m_shaderBinary{ new D3D12_SHADER_BYTECODE() }
        {
            if (shaderBinary.m_buffer)
            {
                m_buffer.reset(new char[shaderBinary.m_shaderBinary->BytecodeLength]);
                memcpy(m_buffer.get(), shaderBinary.m_buffer.get(), shaderBinary.m_shaderBinary->BytecodeLength);
                m_shaderBinary->BytecodeLength = shaderBinary.m_shaderBinary->BytecodeLength;
                m_shaderBinary->pShaderBytecode = m_buffer.get();
            }
        }

        ShaderBinaryImpl::ShaderBinaryImpl(ShaderBinaryImpl&& shaderBinary)
            : m_buffer{ nullptr }
            , m_shaderBinary{ new D3D12_SHADER_BYTECODE() }
        {
            m_buffer.swap(shaderBinary.m_buffer);
            m_shaderBinary->BytecodeLength = shaderBinary.m_shaderBinary->BytecodeLength;
            m_shaderBinary->pShaderBytecode = m_buffer.get();
            shaderBinary.m_shaderBinary->BytecodeLength = 0;
        }

        void ShaderBinaryImpl::clear()
        {
            m_buffer.reset(nullptr);
            m_shaderBinary->BytecodeLength = 0;
            m_shaderBinary->pShaderBytecode = nullptr;
        }

        ShaderBinaryImpl& ShaderBinaryImpl::operator=(const ShaderBinaryImpl& shaderBinary)
        {
            clear();

            if (shaderBinary.m_buffer)
            {
                m_buffer.reset(new char[shaderBinary.m_shaderBinary->BytecodeLength]);
                memcpy(m_buffer.get(), shaderBinary.m_buffer.get(), shaderBinary.m_shaderBinary->BytecodeLength);
                m_shaderBinary->BytecodeLength = shaderBinary.m_shaderBinary->BytecodeLength;
                m_shaderBinary->pShaderBytecode = m_buffer ? m_buffer.get() : nullptr;
            }

            return *this;
        }

        ShaderBinaryImpl& ShaderBinaryImpl::operator=(ShaderBinaryImpl&& shaderBinary)
        {
            clear();
            m_buffer.swap(shaderBinary.m_buffer);
            m_shaderBinary->BytecodeLength = shaderBinary.m_shaderBinary->BytecodeLength;
            m_shaderBinary->pShaderBytecode = m_buffer ? m_buffer.get() : nullptr;
            shaderBinary.m_shaderBinary->BytecodeLength = 0;

            return *this;
        }
    }
}
