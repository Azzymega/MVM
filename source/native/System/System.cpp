/*
 *
 *  * PROJECT:     MVM - System native calls
 *  * LICENSE:     GPL - See COPYING in the top level directory
 *  * PROGRAMMER:  Maltsev Daniil <brickexberiment@lenta.ru>
 * 
 */


#include "../nativeBus.h"
#include <corecrt.h>
#include <cstdio>
#include <cstdlib>
#include <malloc.h>

extern "C" void NBCALL Java_System_printf(ParameterBundle BUNDLE) {
  u4 length = ((objRef *)BUNDLE.args.Objects)->count;
  objRef *str = ((objRef *)BUNDLE.args.Objects);
  char *string = (char *)alloca(length + 1);
  for (size_t i = 0; i < str->count * 16; i += sizeof(valRef)) {
    valRef* val = (valRef*)((char*)str->pointers+i);
    size_t num = i / 16;
    string[num] = *(char*)val->data;
  }
  string[length] = '\0';
  puts(string);
};
