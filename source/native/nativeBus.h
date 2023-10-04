#pragma once
#include "../definitions.hpp"
#include "windows.h"
#define NBCALL __stdcall

typedef void *jobject; 
typedef void *jstring;
typedef void *jweak;
typedef void *jthrowable;
typedef void *jclass;
typedef void *jmethodID;
typedef void *jfieldID;
typedef void *jarray;
typedef void *jobjectArray;
typedef void *jbooleanArray;
typedef void *jbyteArray;
typedef void *jcharArray;
typedef void *jshortArray;
typedef void *jintArray;
typedef void *jlongArray;
typedef void *jfloatArray;
typedef void *jdoubleArray;
typedef unsigned char jboolean;
typedef signed char jbyte;
typedef unsigned short jchar;
typedef short jshort;
typedef int jint;
typedef long long jlong;
typedef float jfloat;
typedef double jdouble;
typedef jint jsize;

enum Types : u1 {
  ObjectTA,
  StringTA,
  WeakTA,
  ThrowableTA,
  ClassTA,
  MethodIDTA,
  FieldIDTA,
  ArrayTA,
  ObjectArrayTA,
  BooleanArrayTA,
  ByteArrayTA,
  CharArrayTA,
  ShortArrayTA,
  IntArrayTA,
  LongArrayTA,
  FloatArrayTA,
  DoubleArrayTA,
  BooleanTA,
  ByteTA,
  CharTA,
  ShortTA,
  IntTA,
  LongTA,
  FloatTA,
  DoubleTA,
  SizeTA,
  NULLTA = 234
};

struct DataHeader {
  u4 Size;
  u1 *Data;
};

struct TypesList {
  u4 Count;
  Types *List;
};

struct Object {
  Types Type;
  TypesList List;
  DataHeader Header;
};

struct StackReturnValue {
  void *stackTop; // NULL, если тип возвращаемого знания - VOID
};

struct ArgumentsList {
  TypesList types;
  Object *Objects;
};

struct ParameterBundle {
  ArgumentsList args;
  StackReturnValue returnValue;
};

/*
  Function name format:
    1. Java
    2. _
    3. <ClassName>
    4. _
    5. <FunctionName>

    Example: extern "C" NBCALL void Java_System_Printf(ParameterBundle BUNDLE);
*/
