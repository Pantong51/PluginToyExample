#include "PluginAPI.h"

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include <windows.h>
using LibHandle = HMODULE;

struct PluginInstance
{
  LibHandle Handle = nullptr;
  IPlugin* Instance = nullptr;
  DestroyPluginFn DestroyFunc = nullptr;
};

LibHandle LoadLibraryCross(const std::string& path)
{
  return ::LoadLibraryA(path.c_str());
}

void UnloadLibraryCross(LibHandle lib)
{
  if (!lib)
  {
    return;
  }

  ::FreeLibrary(lib);
}

void* GetSymbolCross(LibHandle lib, const char* Name)
{
  return reinterpret_cast<void*>(::GetProcAddress(lib, Name));
}

bool LoadPlugin(const std::string& Path, PluginInstance& Out)
{
  if (Out.Instance)
  {
    Out.DestroyFunc(Out.Instance);
    Out.Instance = nullptr;
  }

  if (Out.Handle)
  {
    UnloadLibraryCross(Out.Handle);
    Out.Handle = nullptr;
  }

  LibHandle lib = LoadLibraryCross(Path);
  if (!lib)
  {
    std::cerr << "Failed to load library: " << Path.c_str() << "\n";
    return false;
  }

  auto CreateFn = reinterpret_cast<CreatePluginFn>(GetSymbolCross(lib, CREATE_SYMBOL_NAME));
  auto DestroyFn = reinterpret_cast<DestroyPluginFn>(GetSymbolCross(lib, DESTROY_SYMBOL_NAME));

  if (!CreateFn || !DestroyFn)
  {
    std::cerr << "Missing create/destroy plugin in: " << Path << "\n";
    UnloadLibraryCross(lib);
    return false;
  }

  IPlugin* Plugin = CreateFn();
  if (!Plugin)
  {
    std::cerr << "CreatePlugin failed for: " << Path << "\n";
    UnloadLibraryCross(lib);
    return false;
  }

  Out.Handle = lib;
  Out.Instance = Plugin;
  Out.DestroyFunc = DestroyFn;

  std::cout << "Loaded Plugin: " << Out.Instance->GetName() << " from " << Path << "\n";
  return true;
}

void UnloadPlugin(PluginInstance& Plugin)
{
  if (Plugin.Instance)
  {
    Plugin.DestroyFunc(Plugin.Instance);
    Plugin.Instance = nullptr;
  }

  if (Plugin.Handle)
  {
    UnloadLibraryCross(Plugin.Handle);
    Plugin.Handle = nullptr;
  }

  Plugin.DestroyFunc = nullptr;
}

int main()
{
  PluginInstance plugin;

  std::string addPath = "PluginAdd.dll";
  std::string mulPath = "PluginMul.dll";

  if (!LoadPlugin(addPath, plugin))
  {
    return 1;
  }

  int value = 1;
  for (int tick = 0; tick < 20; ++tick)
  {
    if (tick == 10)
    {
      std::cout << "\n---Hot-Swapping Add for Mul ---\n";
      if (!LoadPlugin(mulPath, plugin))
      {
        std::cerr << "Failed to hot-swap plugin\n";
        break;
      }
    }

    if (plugin.Instance)
    {
      plugin.Instance->OnTick(value);
    }

    std::cout << "Tick " << tick
      << " | Active plugin: "
      << (plugin.Instance ? plugin.Instance->GetName() : "<none>")
      << " | value = " << value << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }

  UnloadPlugin(plugin);
  return 0;
}