#include "Type.h"

namespace serialization
{
    Type::Type(
        const std::string& typeIdStr,
        std::function<void*()> construct,
        std::function<void(void*)> destroy,
        std::function<void(Stream&, void*)> serialize,
        std::function<void(Stream&, void*)> deserialize)
        : m_construct{ construct }
        , m_destroy{ destroy }
        , m_serialize{ serialize }
        , m_deserialize{ deserialize }
        , m_typeId{ typeIdStr }
    {}

    const std::string& Type::typeId() const
    {
        return m_typeId;
    }

    void* Type::construct()
    {
        return m_construct();
    }

    void Type::destroy(void* ptr)
    {
        m_destroy(ptr);
    }

    void Type::serialize(Stream& stream, void* ptr)
    {
        return m_serialize(stream, ptr);
    }

    void Type::deserialize(Stream& stream, void* ptr)
    {
        m_deserialize(stream, ptr);
    }
}
