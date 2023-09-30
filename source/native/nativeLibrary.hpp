#pragma once
#include "../definitions.hpp"
#include <windows.h>
#include <minwindef.h>
#include <vector>


struct NativeLoader;
struct NativeBus;

struct NativeLibrary : public ILoadInteraction<NativeLoader>,
                       ILoadInteraction<NativeBus> {
  std::string libraryName;
  HMODULE nativeLibrary;
  void Interact(NativeLoader *Object) override;
  void Interact(NativeBus *Object) override;
};

struct NativeLoader : public ILoadConflict<NativeLoader *, NativeLibrary *> {
  NativeLoader *Resolve(NativeLibrary *Object) override;
};

struct NativeLibrariesBuffer {
  std::vector<NativeLibrary> libraries;
};

struct NativeBus : ILoadConflict<NativeBus*, NativeLibrary*>{
  NativeLoader loader;
  NativeLibrariesBuffer buffer;
  NativeBus * Resolve(NativeLibrary *Object) override;
};