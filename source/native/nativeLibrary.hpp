#pragma once
#include "../definitions.hpp"
#include <minwindef.h>
#include <windows.h>

struct NativeLoader;

struct NativeLibrary : public ILoadInteraction<NativeLoader> {
  std::string libraryName;
  HMODULE nativeLibrary;
  void Interact(NativeLoader *Object) override;
};

struct NativeLoader : public ILoadConflict<NativeLoader *, NativeLibrary *> {
  NativeLoader *Resolve(NativeLibrary *Object) override;
};
