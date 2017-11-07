#include "components/LightComponent.h"

namespace engine
{
    std::string lightTypeToString(const LightType& type)
    {
        switch (type)
        {
        case LightType::Directional: return "Directional";
        case LightType::Spot: return "Spot";
        case LightType::Point: return "Point";
        }
        return "Point";
    }

    LightType stringToLightType(const std::string& type)
    {
        if (type == "Directional")
            return LightType::Directional;
        else if (type == "Spot")
            return LightType::Spot;
        return LightType::Point;
    }
}
