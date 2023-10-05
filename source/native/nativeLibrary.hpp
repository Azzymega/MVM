#pragma once
#include "../definitions.hpp"
#include "nativeBus.h"
#include <minwindef.h>
#include <string>
#include <vector>
#include <windows.h>

typedef void (*NativeCall)(ParameterBundle BUNDLE);
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
  NativeCall BufferedCall;
};

struct NativeBus : ILoadConflict<NativeBus *, NativeLibrary *>,
                   ILoadConflict<NativeBus *, std::string *> {
  NativeLoader loader;
  NativeLibrariesBuffer buffer;
  NativeBus *Resolve(NativeLibrary *Object) override;
  NativeBus *Resolve(std::string *Object) override;
};