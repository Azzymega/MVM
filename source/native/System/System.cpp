#include "../nativeBus.h"
#include <corecrt.h>
#include <cstdio>

 
extern "C" void NBCALL Java_System_Printf(ParameterBundle BUNDLE){
    u4 length = BUNDLE.args.Objects->Header.Size-4;
    for (size_t x = 0; x < length; x+=2) {
        printf("%ls\n", ((wchar_t)(BUNDLE.args.Objects->Header.Data[x] | BUNDLE.args.Objects->Header.Data[x+1])));
    }
};
