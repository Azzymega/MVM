#pragma once
#include "definitions.hpp"
#include "native/nativeBus.h"
#include <stack>
#include <vector>

struct Memory;

struct MemoryBuffer {
  Memory *Buffer;
};

struct Object;

/*
struct NativeStack {
    std::vector<Object> Objects; // Unused
};
*/

struct Class;
struct Method;
struct NCode;

struct Frame {
  Method *MethodReference;
  void *ReturnValue;
  std::vector<Object> Locals;
  std::stack<Object> OperandStack;
  Class *ClassPoolReference;
  NCode *CodeReference;
};

struct Stack {
  std::vector<Frame> Frames;
};

struct Thread {
  Stack FStack;
};

struct Heap {
  std::vector<Object> ObjectHeap;
};

struct StackBuffer {
  void *ReturnValue;
};

struct ExecutionBuffer {
  Class *ExecutingClass;
  Method *ExecutingMethod;
};

struct Engine : ILoadConflict<Engine *, Class *>,
                ILoadConflict<Engine *, Frame *> {
  u4 PC;
  Stack Frames;
  MemoryBuffer Buffer;
  Heap CurrentHeap;
  StackBuffer SBuffer;
  ExecutionBuffer EBuffer;
  Engine *Resolve(Class *Object) override;

private:
  Engine *Resolve(Frame *Object) override;
};