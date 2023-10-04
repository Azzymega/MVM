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
struct Method;
struct Field;

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
  UnknownT
};

enum SmartAttributesType {
  NTSourceFile,
  NTConstantValue,
  NTExceptions,
  NTCode,
  NTLineNumberTable,
  NTLocalVariableTable,
  NTSignature
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

using NExceptionsTable = exceptionsTables;
using NBytecode = bytecode;

struct NExceptions {
  std::vector<DataPoolType> exceptions;
};

struct NSourceFile {
  DataPoolType *name;
};

using NSignature = NSourceFile;

struct NConstantValue {
  DataPoolType *constant;
};

struct DataPool {
  std::vector<DataPoolType> data;
};

struct NCode;
struct NAttribute;

struct NLineNumber {
  u2 startPC;
  u2 lineNumber;
};

struct NLocalVariable {
  u2 start_pc;
  u2 length;
  DataPoolType *nameIndex;
  DataPoolType *signature;
  u2 slot;
};

struct NLocalVariableTable {
  std::vector<NLocalVariable> table;
};

struct NLineNumberTable {
  std::vector<NLineNumber> table;
};

struct NAttributes {
  std::vector<NAttribute> attributes;
};

struct NCode {
  u2 maxStack;
  u2 maxLocals;
  NBytecode internalCode;
  NExceptionsTable tables;
  NAttributes attr;
};

struct NAttribute {
  SmartAttributesType type;
  DataPoolType *name;
  u4 length;
  NSourceFile sourceFile;
  NConstantValue constantValue;
  NExceptions exceptions;
  NCode code;
  NLocalVariableTable lTable;
  NLineNumberTable liTable;
  NSignature signature;
};

struct Field {
  AccessFlags fieldFlags;
  std::string name;
  std::string signature;
  NAttributes attributes;
};

struct Method {
  AccessFlags methodFlags;
  std::string name;
  std::string signature;
  NAttributes attributes;
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
  NAttributes attr;
};

struct Classes {
  std::vector<Class *> classes;
};

struct ClassFileBuffer {
  classFile *buffer;
  Class *klassBuffer;
  attributes *attrBuffer;
};

struct Memory;

struct Linker : public ILoadConflict<Linker *, Memory *>,
                ILoadConflict<Linker *, DataPool *>,
                ILoadConflict<Linker *, Class *>,
                ILoadConflict<Linker *, Classes *>,
                ILoadConflict<Linker *, ClassLink *> {
  Memory *buffer;
  Linker *Resolve(Memory *Object) override;
  Linker *Resolve(DataPool *Object) override;
  Linker *Resolve(Class *Object) override;
  Linker *Resolve(Classes *Object) override;
  Linker *Resolve(ClassLink *Object) override;
}; // начать писать класс Machine внутри которого будет Memory и интерпретатор,
   // дописать кучу в Memory!!!

struct Memory : public ILoadInteraction<Linker>,
                ILoadConflict<AccessFlags, u2 *>,
                ILoadConflict<Memory *, classFile *>,
                ILoadConflict<Memory *, ClassHeader *>,
                ILoadConflict<Memory *, DataPool *>,
                ILoadConflict<Memory *, Interfaces *>,
                ILoadConflict<Memory *, Fields *>,
                ILoadConflict<Memory *, Methods *>,
                ILoadConflict<Memory *, NAttributes *> {
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
  Memory *Resolve(NAttributes *Object) override;
};
