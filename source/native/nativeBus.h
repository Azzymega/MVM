#pragma once
#include "../definitions.hpp"
#define NBCALL _stdcall

typedef void *jobject; // в будущем расписать вид расположения в памяти
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

enum ArgTypes { Object, Bool, Byte, Char, Short, Integer, Long, Double, Float, String };

struct ArgumentTypes {
  u4 length;
  ArgTypes *types;
};

struct StackReturnValue {
  void *stackTop; // NULL, если тип возвращаемого знания - VOID
};

struct ArgumentsList {
  ArgumentTypes types;
  u4 length;
  u1 *data;
};

struct ParameterBundle {
  ArgumentsList args;
  StackReturnValue returnValue;
};

NBCALL void NBTEST(ParameterBundle BUNDLE); 