#include "tools/Serialization.h"
#include "engine/primitives/Vector2.h"
#include "engine/primitives/Vector3.h"
#include "engine/primitives/Vector4.h"
#include "engine/primitives/Matrix4.h"
#include "engine/primitives/Quaternion.h"
#include "tools/StringTools.h"
#include "components/Camera.h"
#include "components/LightComponent.h"
#include "components/CollisionShapeComponent.h"

using namespace engine::serialization;

namespace engine
{
    template <>
    void writeJsonValue<char>(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& name, char& value)
    {
        std::string key = keyPrefix<KeyTypes, TypeId>(PropertyName, serialization::TypeId::Char);
        writer.Key(key.data());
        writer.String(name.data());

        key = keyPrefix<KeyTypes, TypeId>(PropertyValue, serialization::TypeId::Char);
        writer.Key(key.data());
        writer.Int(static_cast<int>(value));
    }

    template <>
    void writeJsonValue<short>(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& name, short& value)
    {
        std::string key = keyPrefix<KeyTypes, TypeId>(PropertyName, serialization::TypeId::Short);
        writer.Key(key.data());
        writer.String(name.data());

        key = keyPrefix<KeyTypes, TypeId>(PropertyValue, serialization::TypeId::Short);
        writer.Key(key.data());
        writer.Int(static_cast<int>(value));
    }

    template <>
    void writeJsonValue<int>(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& name, int& value)
    {
        std::string key = keyPrefix<KeyTypes, TypeId>(PropertyName, serialization::TypeId::Int);
        writer.Key(key.data());
        writer.String(name.data());

        key = keyPrefix<KeyTypes, TypeId>(PropertyValue, serialization::TypeId::Int);
        writer.Key(key.data());
        writer.Int(value);
    }

    template <>
    void writeJsonValue<unsigned char>(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& name, unsigned char& value)
    {
        std::string key = keyPrefix<KeyTypes, TypeId>(PropertyName, serialization::TypeId::UnsignedChar);
        writer.Key(key.data());
        writer.String(name.data());

        key = keyPrefix<KeyTypes, TypeId>(PropertyValue, serialization::TypeId::UnsignedChar);
        writer.Key(key.data());
        writer.Uint(static_cast<unsigned int>(value));
    }

    template <>
    void writeJsonValue<unsigned short>(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& name, unsigned short& value)
    {
        std::string key = keyPrefix<KeyTypes, TypeId>(PropertyName, serialization::TypeId::UnsignedShort);
        writer.Key(key.data());
        writer.String(name.data());

        key = keyPrefix<KeyTypes, TypeId>(PropertyValue, serialization::TypeId::UnsignedShort);
        writer.Key(key.data());
        writer.Uint(static_cast<unsigned int>(value));
    }

    template <>
    void writeJsonValue<unsigned int>(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& name, unsigned int& value)
    {
        std::string key = keyPrefix<KeyTypes, TypeId>(PropertyName, serialization::TypeId::UnsignedInt);
        writer.Key(key.data());
        writer.String(name.data());

        key = keyPrefix<KeyTypes, TypeId>(PropertyValue, serialization::TypeId::UnsignedInt);
        writer.Key(key.data());
        writer.Uint(value);
    }

    template <>
    void writeJsonValue<bool>(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& name, bool& value)
    {
        std::string key = keyPrefix<KeyTypes, TypeId>(PropertyName, serialization::TypeId::Bool);
        writer.Key(key.data());
        writer.String(name.data());

        key = keyPrefix<KeyTypes, TypeId>(PropertyValue, serialization::TypeId::Bool);
        writer.Key(key.data());
        writer.Bool(value);
    }

    template <>
    void writeJsonValue<float>(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& name, float& value)
    {
        std::string key = keyPrefix<KeyTypes, TypeId>(PropertyName, serialization::TypeId::Float);
        writer.Key(key.data());
        writer.String(name.data());

        key = keyPrefix<KeyTypes, TypeId>(PropertyValue, serialization::TypeId::Float);
        writer.Key(key.data());
        writer.Double(static_cast<double>(value));
    }

    template <>
    void writeJsonValue<double>(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& name, double& value)
    {
        std::string key = keyPrefix<KeyTypes, TypeId>(PropertyName, serialization::TypeId::Double);
        writer.Key(key.data());
        writer.String(name.data());

        key = keyPrefix<KeyTypes, TypeId>(PropertyValue, serialization::TypeId::Double);
        writer.Key(key.data());
        writer.Double(value);
    }

    template <>
    void writeJsonValue<Vector2f>(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& name, Vector2f& value)
    {
        std::string key = keyPrefix<KeyTypes, TypeId>(PropertyName, TypeId::Vector2f);
        writer.Key(key.data());
        writer.String(name.data());

        key = keyPrefix<KeyTypes, TypeId>(PropertyValue, TypeId::Vector2f);
        std::string val;
        val += std::to_string(value.x); val += ":";
        val += std::to_string(value.y);
        writer.Key(key.data());
        writer.String(val.data());
    }

    template <>
    void writeJsonValue<Vector3f>(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& name, Vector3f& value)
    {
        std::string key = keyPrefix<KeyTypes, TypeId>(PropertyName, TypeId::Vector3f);
        writer.Key(key.data());
        writer.String(name.data());

        key = keyPrefix<KeyTypes, TypeId>(PropertyValue, TypeId::Vector3f);
        std::string val;
        val += std::to_string(value.x); val += ":";
        val += std::to_string(value.y); val += ":";
        val += std::to_string(value.z);
        writer.Key(key.data());
        writer.String(val.data());
    }

    template <>
    void writeJsonValue<Vector4f>(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& name, Vector4f& value)
    {
        std::string key = keyPrefix<KeyTypes, TypeId>(PropertyName, TypeId::Vector4f);
        writer.Key(key.data());
        writer.String(name.data());

        key = keyPrefix<KeyTypes, TypeId>(PropertyValue, TypeId::Vector4f);
        std::string val;
        val += std::to_string(value.x); val += ":";
        val += std::to_string(value.y); val += ":";
        val += std::to_string(value.z); val += ":";
        val += std::to_string(value.w);
        writer.Key(key.data());
        writer.String(val.data());
    }

    template <>
    void writeJsonValue<Matrix3f>(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& name, Matrix3f& value)
    {
        std::string key = keyPrefix<KeyTypes, TypeId>(PropertyName, TypeId::Matrix3f);
        writer.Key(key.data());
        writer.String(name.data());

        key = keyPrefix<KeyTypes, TypeId>(PropertyValue, TypeId::Matrix3f);
        std::string val;
        val += std::to_string(value.m00); val += ":";
        val += std::to_string(value.m01); val += ":";
        val += std::to_string(value.m02); val += ":";

        val += std::to_string(value.m10); val += ":";
        val += std::to_string(value.m11); val += ":";
        val += std::to_string(value.m12); val += ":";

        val += std::to_string(value.m20); val += ":";
        val += std::to_string(value.m21); val += ":";
        val += std::to_string(value.m22);

        writer.Key(key.data());
        writer.String(val.data());
    }

    template <>
    void writeJsonValue<Matrix4f>(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& name, Matrix4f& value)
    {
        std::string key = keyPrefix<KeyTypes, TypeId>(PropertyName, TypeId::Matrix4f);
        writer.Key(key.data());
        writer.String(name.data());

        key = keyPrefix<KeyTypes, TypeId>(PropertyValue, TypeId::Matrix4f);
        std::string val;
        val += std::to_string(value.m00); val += ":";
        val += std::to_string(value.m01); val += ":";
        val += std::to_string(value.m02); val += ":";
        val += std::to_string(value.m03); val += ":";

        val += std::to_string(value.m10); val += ":";
        val += std::to_string(value.m11); val += ":";
        val += std::to_string(value.m12); val += ":";
        val += std::to_string(value.m13); val += ":";

        val += std::to_string(value.m20); val += ":";
        val += std::to_string(value.m21); val += ":";
        val += std::to_string(value.m22); val += ":";
        val += std::to_string(value.m23); val += ":";

        val += std::to_string(value.m30); val += ":";
        val += std::to_string(value.m31); val += ":";
        val += std::to_string(value.m32); val += ":";
        val += std::to_string(value.m33);

        writer.Key(key.data());
        writer.String(val.data());
    }

    template <>
    void writeJsonValue<Quaternionf>(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& name, Quaternionf& value)
    {
        std::string key = keyPrefix<KeyTypes, TypeId>(PropertyName, TypeId::Quaternionf);
        writer.Key(key.data());
        writer.String(name.data());

        key = keyPrefix<KeyTypes, TypeId>(PropertyValue, TypeId::Quaternionf);
        std::string val;
        val += std::to_string(value.x); val += ":";
        val += std::to_string(value.y); val += ":";
        val += std::to_string(value.z); val += ":";
        val += std::to_string(value.w);
        writer.Key(key.data());
        writer.String(val.data());
    }

    template <>
    void writeJsonValue<std::string>(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& name, std::string& value)
    {
        std::string key = keyPrefix<KeyTypes, TypeId>(PropertyName, TypeId::String);
        writer.Key(key.data());
        writer.String(name.data());

        key = keyPrefix<KeyTypes, TypeId>(PropertyValue, TypeId::String);
        writer.Key(key.data());
        writer.String(value.data());
    }

    template <>
    void writeJsonValue<engine::Projection>(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& name, engine::Projection& value)
    {
        std::string key = keyPrefix<KeyTypes, TypeId>(PropertyName, TypeId::Projection);
        writer.Key(key.data());
        writer.String(name.data());

        key = keyPrefix<KeyTypes, TypeId>(PropertyValue, TypeId::Projection);
        writer.Key(key.data());
        writer.String(projectionToString(value).data());
    }

    template <>
    void writeJsonValue<engine::CollisionShape>(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& name, engine::CollisionShape& value)
    {
        std::string key = keyPrefix<KeyTypes, TypeId>(PropertyName, TypeId::CollisionShape);
        writer.Key(key.data());
        writer.String(name.data());

        key = keyPrefix<KeyTypes, TypeId>(PropertyValue, TypeId::CollisionShape);
        writer.Key(key.data());
        writer.String(collisionShapeToString(value).data());
    }

    template <>
    void writeJsonValue<engine::ButtonPush>(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& name, engine::ButtonPush& value)
    {
        std::string key = keyPrefix<KeyTypes, TypeId>(PropertyName, serialization::TypeId::ButtonPush);
        writer.Key(key.data());
        writer.String(name.data());

        key = keyPrefix<KeyTypes, TypeId>(PropertyValue, serialization::TypeId::ButtonPush);
        writer.Key(key.data());
        writer.Bool(static_cast<bool>(value));
    }

    template <>
    void writeJsonValue<engine::ButtonToggle>(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& name, engine::ButtonToggle& value)
    {
        std::string key = keyPrefix<KeyTypes, TypeId>(PropertyName, serialization::TypeId::ButtonToggle);
        writer.Key(key.data());
        writer.String(name.data());

        key = keyPrefix<KeyTypes, TypeId>(PropertyValue, serialization::TypeId::ButtonToggle);
        writer.Key(key.data());
        writer.Bool(static_cast<bool>(value));
    }

    template <>
    void writeJsonValue<engine::LightType>(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer, const std::string& name, engine::LightType& value)
    {
        std::string key = keyPrefix<KeyTypes, TypeId>(PropertyName, TypeId::LightType);
        writer.Key(key.data());
        writer.String(name.data());

        key = keyPrefix<KeyTypes, TypeId>(PropertyValue, TypeId::LightType);
        writer.Key(key.data());
        writer.String(lightTypeToString(value).data());
    }

    char readJsonValue_char(int value) { return static_cast<char>(value); }
    short readJsonValue_short(int value) { return static_cast<short>(value); }
    int readJsonValue_int(int value) { return value; }
    unsigned char readJsonValue_unsignedchar(unsigned int value) { return static_cast<unsigned char>(value); }
    unsigned short readJsonValue_unsignedshort(unsigned int value) { return static_cast<unsigned short>(value); }
    unsigned int readJsonValue_unsignedint(unsigned int value) { return value; }

    bool readJsonValue_bool(bool value) { return value; }
    float readJsonValue_float(double value) { return static_cast<float>(value); }
    double readJsonValue_double(double value) { return value; }

    Vector2f readJsonValue_vector2f(const std::string& value)
    {
        auto parts = tokenize(value);
        return{ 
            stringToNumber<float>(parts[0]), 
            stringToNumber<float>(parts[1]) };
    }

    Vector3f readJsonValue_vector3f(const std::string& value)
    {
        auto parts = tokenize(value);
        return{
            stringToNumber<float>(parts[0]),
            stringToNumber<float>(parts[1]),
            stringToNumber<float>(parts[2]) };
    }

    Vector4f readJsonValue_vector4f(const std::string& value)
    {
        auto parts = tokenize(value);
        return{
            stringToNumber<float>(parts[0]),
            stringToNumber<float>(parts[1]),
            stringToNumber<float>(parts[2]),
            stringToNumber<float>(parts[3]) };
    }

    Matrix3f readJsonValue_matrix3f(const std::string& value)
    {
        auto parts = tokenize(value);
        return{
            stringToNumber<float>(parts[0]),
            stringToNumber<float>(parts[1]),
            stringToNumber<float>(parts[2]),
            
            stringToNumber<float>(parts[3]),
            stringToNumber<float>(parts[4]),
            stringToNumber<float>(parts[5]),

            stringToNumber<float>(parts[6]),
            stringToNumber<float>(parts[7]),
            stringToNumber<float>(parts[8])
        };
    }

    Matrix4f readJsonValue_matrix4f(const std::string& value)
    {
        auto parts = tokenize(value);
        return{
            stringToNumber<float>(parts[0]),
            stringToNumber<float>(parts[1]),
            stringToNumber<float>(parts[2]),
            stringToNumber<float>(parts[3]),

            stringToNumber<float>(parts[4]),
            stringToNumber<float>(parts[5]),
            stringToNumber<float>(parts[6]),
            stringToNumber<float>(parts[7]),

            stringToNumber<float>(parts[8]),
            stringToNumber<float>(parts[9]),
            stringToNumber<float>(parts[10]),
            stringToNumber<float>(parts[11]),

            stringToNumber<float>(parts[12]),
            stringToNumber<float>(parts[13]),
            stringToNumber<float>(parts[14]),
            stringToNumber<float>(parts[15])
        };
    }

    Quaternionf readJsonValue_quaternionf(const std::string& value)
    {
        auto parts = tokenize(value);
        return{
            stringToNumber<float>(parts[0]),
            stringToNumber<float>(parts[1]),
            stringToNumber<float>(parts[2]),
            stringToNumber<float>(parts[3]) };
    }

    std::string readJsonValue_string(const std::string& value)
    {
        return value;
    }

    engine::Projection readJsonValue_projection(const std::string& value)
    {
        return stringToProjection(value);
    }

    engine::CollisionShape readJsonValue_collisionShape(const std::string& value)
    {
        return stringToCollisionShape(value);
    }

    engine::ButtonPush readJsonValue_buttonPush(bool value) { return static_cast<engine::ButtonPush>(value); }
    engine::ButtonToggle readJsonValue_buttonToggle(bool value) { return static_cast<engine::ButtonToggle>(value); }

    LightType readJsonValue_lightType(const std::string& value)
    {
        if (value == "Directional")
            return LightType::Directional;
        else if (value == "Spot")
            return LightType::Spot;
        else return LightType::Point;
    }
}
