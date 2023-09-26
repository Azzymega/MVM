#pragma once
#include "classfile.hpp"
#include <vector>

struct Class;

struct AccessFlags {
  std::vector<accessFlags> flags;
};

struct ClassLink {
  std::string className;
  Class *link;
};

using Interface = ClassLink;
using Double = constantDouble;
using Long = constantLong;
using Float = constantFloat;
using Integer = constantInteger;

struct Field {
  AccessFlags fieldFlags;
  std::string name;
  std::string signature;
  attributes attributes;
};

struct Method {
  AccessFlags methodFlags;
  std::string name;
  std::string signature;
  attributes attributes;
};

struct NameAndType {
  std::string name;
  std::string descriptor;
};

struct TypeMetadata {
  ClassLink Class;
  NameAndType info;
};

struct MethodLink {
  TypeMetadata data;
  Method *Method;
};

struct FieldLink {
  TypeMetadata data;
  Field *Field;
};

struct String {
  std::string info;
};

struct Fields {
  std::vector<Field> fields;
};

struct Methods {
  std::vector<Method> methods;
};

struct Interfaces {
  std::vector<Interface> interfaces;
};

enum DataPoolTypes {
  ClassT,
  MethodT,
  FieldT,
  DoubleT,
  StringT,
  LongT,
  FloatT,
  IntegerT,
  UnownT
};

struct Boiler {
  u1 magic;
};

struct DataPoolType {
  DataPoolTypes tag;
  ClassLink Class;
  MethodLink Method;
  FieldLink Field;
  Double Double;
  String String;
  Long Long;
  Float Float;
  Integer Integer;
  Boiler Boiler;
};

struct DataPool {
  std::vector<DataPoolType> data;
};

struct ClassHeader {
  u2 minVersion;
  u2 maxVersion;
  AccessFlags flags;
  ClassLink Class;
  ClassLink superClass;
};

struct Class {
  ClassHeader mainInfo;
  DataPool pool;
  Interfaces interfaces;
  Fields fields;
  Methods methods;
  attributes attr;
};

struct Classes {
  std::vector<Class> classes;
};

struct ClassFileBuffer {
  classFile *buffer;
};

struct Memory;

struct Linker : public ILoadConflict<Linker *, Memory *> {
  Linker *Resolve(Memory *Object) override;
}; // начать писать класс Machine внутри которого будет Memory и интерпретатор, дописать кучу в Memory!!!

struct Memory : public ILoadInteraction<Linker>,
                ILoadConflict<AccessFlags, u2 *>,
                ILoadConflict<Memory *, classFile *>,
                ILoadConflict<Memory *, ClassHeader *>,
                ILoadConflict<Memory *, DataPool *>,
                ILoadConflict<Memory *, Interfaces *>,
                ILoadConflict<Memory *, Fields *>,
                ILoadConflict<Memory *, Methods *> {
  Classes info;
  Linker linker;
  ClassFileBuffer buffer;

  void Interact(Linker *Object) override;
  Memory *Resolve(classFile *Object) override;
  Memory *Resolve(ClassHeader *Object) override;
  AccessFlags Resolve(unsigned short *Object) override;
  Memory *Resolve(DataPool *Object) override;
  Memory *Resolve(Interfaces *Object) override;
  Memory *Resolve(Fields *Object) override;
  Memory *Resolve(Methods *Object) override;
};
