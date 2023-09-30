#include "nativeLibrary.hpp"
#include <libloaderapi.h>

void NativeLibrary::Interact(NativeLoader *Object) { Object->Resolve(this); }

void NativeLibrary::Interact(NativeBus *Object) { Object->Resolve(this); }

NativeLoader *NativeLoader::Resolve(NativeLibrary *Object) {
  Object->nativeLibrary = LoadLibrary(Object->libraryName.c_str());
  return nullptr;
}

NativeBus *NativeBus::Resolve(NativeLibrary *Object) {
  loader.Resolve(Object);
  buffer.libraries.push_back(*Object);
  return nullptr;
}
