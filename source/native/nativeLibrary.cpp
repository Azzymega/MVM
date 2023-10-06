/*
 *
 *  * PROJECT:     MVM
 *  * LICENSE:     GPL - See COPYING in the top level directory
 *  * PROGRAMMER:  Maltsev Daniil <brickexberiment@lenta.ru>
 * 
 */


#include "nativeLibrary.hpp"
#include <libloaderapi.h>

void NativeLibrary::Interact(NativeLoader *Object) { Object->Resolve(this); }

void NativeLibrary::Interact(NativeBus *Object) { Object->Resolve(this); }

NativeLoader *NativeLoader::Resolve(NativeLibrary *Object) {
  Object->nativeLibrary = LoadLibrary(Object->libraryName.c_str());
  if (!Object->nativeLibrary) {
    throw "[Native Library Loader] Loading failes!";
  }
  return nullptr;
}

NativeBus *NativeBus::Resolve(NativeLibrary *Object) {
  loader.Resolve(Object);
  buffer.libraries.push_back(*Object);
  return nullptr;
}

NativeBus * NativeBus::Resolve(std::string *Object) {
  for (auto &&libs : this->buffer.libraries) {
    this->buffer.BufferedCall = (NativeCall)GetProcAddress(libs.nativeLibrary, Object->c_str());
    if (this->buffer.BufferedCall) {
      return nullptr;
    }
  }
  return nullptr;
}
