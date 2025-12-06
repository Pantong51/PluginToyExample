#include "../Core/PluginAPI.h"
#include <iostream>

class PluginMul : public IPlugin
{
public:
  const char* GetName() const override
  {
    return "PluginMul (*2)";
  }

  void OnTick(int& value) override
  {
    value *= 2;
    std::cout << "[PluginMul] value *= 2\n";
  }
};

PLUGIN_EXPORT IPlugin* CreatePlugin()
{
  return new PluginMul();
}

PLUGIN_EXPORT void DestroyPlugin(IPlugin* p)
{
  delete p;
}