#pragma once
#include "definitions.hpp"

struct sourceFile {
  u2 index;
};

struct constantValue {
  u2 index;
};

struct exceptionTable {
  u2 startPc;
  u2 endPc;
  u2 handlerPc;
  u2 catchType;
};

struct localVariable {
  u2 start_pc;
  u2 length;
  u2 name_index;
  u2 signature_index;
  u2 slot;
};

struct lineNumber {
  u2 startPc;
  u2 lineNumber;
};

struct localVariableTable {
  u2 length;
  std::vector<localVariable> *table;
};

struct lineNumberTable {
  u2 length;
  std::vector<lineNumber> *table;
};

struct exceptions {
  u2 exceptionCount;
  std::vector<u2> *tableExceptions;
};

struct exceptionsTables {
  u2 exceptionTableLength;
  std::vector<exceptionTable> *table;
};

struct loader;
struct attribute;

struct attributes : public ILoadInteraction<loader> {
  u2 attributes_count;
  std::vector<attribute> *attr;
  void Interact(loader *Object) override;
};

struct bytecode {
  u4 length;
  std::vector<u1> *instructions;
};

struct code {
  u2 maxStack;
  u2 maxLocals;
  bytecode internalCode;
  exceptionsTables tables;
  attributes attr;
};

struct attribute // переделать на умные атрибуты!!!
{
  u2 name;
  u4 length;
  sourceFile sourceFile;
  constantValue constantValue;
  exceptions exceptions;
  code code;
  lineNumberTable lineNumberTable;
  localVariableTable localVariableTable;
  attribute();
  ~attribute();
};