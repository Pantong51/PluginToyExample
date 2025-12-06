#pragma once

#include <cstdint>

#define PLUGIN_EXPORT extern "C" __declspec(dllexport)

struct IPlugin
{
  virtual ~IPlugin() = default;
  virtual const char* GetName() const = 0;
  virtual void OnTick(int& value) = 0;
};

using CreatePluginFn = IPlugin * (*)();
using DestroyPluginFn = void (*)(IPlugin*);

constexpr const char* CREATE_SYMBOL_NAME = "CreatePlugin";
constexpr const char* DESTROY_SYMBOL_NAME = "DestroyPlugin";