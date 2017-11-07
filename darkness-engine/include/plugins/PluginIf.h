#pragma once

#include <string>

namespace engine
{
    class PluginIf
    {
    public:
        virtual ~PluginIf() {};
        virtual std::string name() const = 0;
    };

    class Plugin : public PluginIf
    {
        std::string name() const override;
    };
}

extern "C"
{
    engine::PluginIf* createInstance();
    void destroyInstance(engine::PluginIf*);
}
