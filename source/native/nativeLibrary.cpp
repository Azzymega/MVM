#include "nativeLibrary.hpp"
#include <libloaderapi.h>

void NativeLibrary::Interact(NativeLoader *Object) {
    Object->Resolve(this);
}

NativeLoader *NativeLoader::Resolve(NativeLibrary *Object) {
    Object->nativeLibrary = LoadLibrary(Object->libraryName.c_str());
    return nullptr;
}
