#include "plugins/PluginIf.h"

namespace engine
{
    std::string Plugin::name() const
    {
        return "";
    }
}

engine::PluginIf* createInstance()
{
    return new engine::Plugin();
}

void destroyInstance(engine::PluginIf* plugin)
{
    delete plugin;
}
