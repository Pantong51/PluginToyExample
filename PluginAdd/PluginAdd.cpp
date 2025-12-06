#include "../Core/PluginAPI.h"
#include <iostream>

class PluginAdd : public IPlugin
{
public:
  const char* GetName() const override
  {
    return "PluginAdd (+1)";
  }

  void OnTick(int& value) override
  {
    value += 1;
    std::cout << "[PluginAdd] value += 1\n";
  }
};

PLUGIN_EXPORT IPlugin* CreatePlugin()
{
  return new PluginAdd();
}

PLUGIN_EXPORT void DestroyPlugin(IPlugin* p)
{
  delete p;
}