#include "engine.hpp"
#include "memory.hpp"
#include <corecrt.h>
#include <cstdlib>
#include <cwchar>
#include <malloc.h>

Engine *Engine::Resolve(Class *Object) {
  for (auto &&methods : Object->methods.methods) {
    if (methods.name == "main") {
      Frame initFrame;
      initFrame.ClassPoolReference = Object;
      initFrame.MethodReference = &methods;
      initFrame.ReturnValue = nullptr;
      initFrame.CodeReference = &methods.attributes.attributes[0].code;
      Resolve(&initFrame);
    }
  }
  return nullptr;
}

Engine *Engine::Resolve(Frame *Object) {
  // interpreter here и доработать идею о нативных вызовах библиотек!
  // стандартную функцию загрузки засунуть в стороннюю либу
  this->PC = 0;
  for (; PC < Object->CodeReference->internalCode.instructions->size(); PC++) {
    switch (Object->CodeReference->internalCode.instructions->at(PC)) {
    case op_bipush: {
      struct Object Byte {};
      Byte.Type = ByteTA;
      Byte.Header.Size = 1;
      Byte.Header.Data =
          (u1 *)malloc(sizeof(byte)); // Добавить проверку на NULL
      *Byte.Header.Data =
          Object->CodeReference->internalCode.instructions->at(++PC);
      Object->OperandStack.push(&Byte);
      break;
    }
    case op_dup: {
      Object->OperandStack.push(Object->OperandStack.top());
      break;
    };
    case op_nop: {
    };
    case op_putfield: {
      byte FirstByte =
          Object->CodeReference->internalCode.instructions->at(PC++);
      byte SecondByte =
          Object->CodeReference->internalCode.instructions->at(PC++);
      u2 CPIndex = FirstByte | SecondByte;
      FieldLink Link =
          Object->ClassPoolReference->pool.data.at(--CPIndex).Field;
      struct Object Value = *Object->OperandStack.top();
      Object->OperandStack.pop();
      struct Object ObjectLink = *Object->OperandStack.top();
      Object->OperandStack.pop();
      memcpy((void *)ObjectLink.Header.Data, (void *)&Value,
             sizeof(struct Object));
      break;
    };
    case op_astore_0: {
      struct Object *Data{};
      Data = Object->OperandStack.top();
      Object->OperandStack.pop();
      Object->Locals.at(0) = *Data;
    }
    case op_astore_1: {
      struct Object *Data{};
      Data = Object->OperandStack.top();
      Object->OperandStack.pop();
      Object->Locals.at(1) = *Data;
    }
    case op_astore_2: {
      struct Object *Data{};
      Data = Object->OperandStack.top();
      Object->OperandStack.pop();
      Object->Locals.at(2) = *Data;
    }
    case op_astore_3: {
      struct Object *Data{};
      Data = Object->OperandStack.top();
      Object->OperandStack.pop();
      Object->Locals.at(3) = *Data;
    }
    case op_iconst_0: {
      struct Object IConst {};
      IConst.Type = IntTA;
      IConst.Header.Size = sizeof(jint);
      IConst.Header.Data = (u1 *)malloc(sizeof(jint));
      *IConst.Header.Data = 3;
      Object->OperandStack.push(&IConst);
      break;
    }
    case op_iconst_1: {
      struct Object IConst {};
      IConst.Type = IntTA;
      IConst.Header.Size = sizeof(jint);
      IConst.Header.Data = (u1 *)malloc(sizeof(jint));
      *IConst.Header.Data = 4;
      Object->OperandStack.push(&IConst);
      break;
    }
    case op_iconst_2: {
      struct Object IConst {};
      IConst.Type = IntTA;
      IConst.Header.Size = sizeof(jint);
      IConst.Header.Data = (u1 *)malloc(sizeof(jint));
      *IConst.Header.Data = 5;
      Object->OperandStack.push(&IConst);
      break;
    }
    case op_iconst_3: {
      struct Object IConst {};
      IConst.Type = IntTA;
      IConst.Header.Size = sizeof(jint);
      IConst.Header.Data = (u1 *)malloc(sizeof(jint));
      *IConst.Header.Data = 6;
      Object->OperandStack.push(&IConst);
      break;
    }
    case op_iconst_4: {
      struct Object IConst {};
      IConst.Type = IntTA;
      IConst.Header.Size = sizeof(jint);
      IConst.Header.Data = (u1 *)malloc(sizeof(jint));
      *IConst.Header.Data = 7;
      Object->OperandStack.push(&IConst);
      break;
    }
    case op_iconst_5: {
      struct Object IConst {};
      IConst.Type = IntTA;
      IConst.Header.Size = sizeof(jint);
      IConst.Header.Data = (u1 *)malloc(sizeof(jint));
      *IConst.Header.Data = 8;
      Object->OperandStack.push(&IConst);
      break;
    }
    case op_return: {
      this->Frames.Frames.pop_back();
      return nullptr;
    };
    case op_invokestatic: {
      std::vector<struct Object> Objects{};
      while (true) {
        struct Object Arg = *Object->OperandStack.top();
        Object->OperandStack.pop();
        Objects.push_back(Arg);
        if (Arg.Type == ClassTA) {
          break;
        }
      }
      Frame initFrame;
      for (auto &&arg : Objects) {
        initFrame.OperandStack.push(&arg);
      }
      byte FirstByte =
          Object->CodeReference->internalCode.instructions->at(PC++);
      byte SecondByte =
          Object->CodeReference->internalCode.instructions->at(PC++);
      u2 CPIndex = FirstByte | SecondByte;
      MethodLink Link =
          Object->ClassPoolReference->pool.data.at(--CPIndex).Method;
      initFrame.ClassPoolReference = Link.data.Class.link;
      initFrame.MethodReference = Link.Method;
      initFrame.CodeReference = &Link.Method->attributes.attributes.at(0).code;
      Resolve(&initFrame);
      break;
    }
    case op_invokevirtual: {
      bool isNative{};
      std::vector<struct Object> Objects{};
      while (true) {
        struct Object Arg = *Object->OperandStack.top();
        Object->OperandStack.pop();
        Objects.push_back(Arg);
        if (Arg.Type == ClassTA) {
          break;
        }
      }
      Frame initFrame;
      for (auto &&arg : Objects) {
        initFrame.OperandStack.push(&arg);
      }
      byte FirstByte =
          Object->CodeReference->internalCode.instructions->at(PC++);
      byte SecondByte =
          Object->CodeReference->internalCode.instructions->at(PC++);
      u2 CPIndex = FirstByte | SecondByte;
      MethodLink Link =
          Object->ClassPoolReference->pool.data.at(--CPIndex).Method;
      for (auto &&flag : Link.Method->methodFlags.flags) {
        if (flag == accessFlags::ACC_NATIVE) {
          if (Link.Method->name == "LoadNativeLibrary") {
            NativeLibrary lib;
            std::string name;
            for (int x = 0; x < Objects.at(0).Header.Size; x++) {
              name += ((char)Objects.at(0).Header.Data[x]);
            }
            lib.libraryName = name;
            this->NBus.Resolve(&lib);
            isNative = true;
            break;
          } else {
            std::string funName =
                "Java_" + Link.data.Class.className + "_" + Link.Method->name;
            ParameterBundle bundle;
            bundle.returnValue.stackTop = nullptr;
            bundle.args.Objects = &Objects.at(0);
            this->NBus.Resolve(&funName);
            if (NBus.buffer.BufferedCall) {

              NBus.buffer.BufferedCall(bundle);
              isNative = true;
            }
          }
        }
      }
      if (!isNative) {
        initFrame.ClassPoolReference = Link.data.Class.link;
        initFrame.MethodReference = Link.Method;
        initFrame.CodeReference =
            &Link.Method->attributes.attributes.at(0).code;
        Resolve(&initFrame);
        break;
      }
    };
    case op_invokespecial: {
      if (Object->ClassPoolReference->mainInfo.superClass.className == "NOPE") {
        this->Frames.Frames.pop_back();
        return nullptr;
      }
      std::vector<struct Object> Objects;
      bool reading = true;
      while (reading) {
        struct Object Arg = *Object->OperandStack.top();
        Object->OperandStack.pop();
        Objects.push_back(Arg);
        if (Arg.Type == ObjectTA) {
          reading = false;
          break;
        }
      }
      Frame initFrame;
      for (auto &&arg : Objects) {
        initFrame.Locals.push_front(arg);
      }
      byte FirstByte =
          Object->CodeReference->internalCode.instructions->at(++PC);
      byte SecondByte =
          Object->CodeReference->internalCode.instructions->at(++PC);
      u2 CPIndex = FirstByte | SecondByte;
      MethodLink Link =
          Object->ClassPoolReference->pool.data.at(--CPIndex).Method;
      initFrame.ClassPoolReference = Link.data.Class.link;
      initFrame.MethodReference = Link.Method;
      initFrame.CodeReference = &Link.Method->attributes.attributes.at(0).code;

      this->Frames.Frames.push_back(&initFrame);
      Resolve(&initFrame);
      break;
    };
    case op_castore: {
      struct Object Value = *Object->OperandStack.top();
      Object->OperandStack.pop();
      struct Object Index = *Object->OperandStack.top();
      Object->OperandStack.pop();
      struct Object Array = *Object->OperandStack.top();
      Object->OperandStack.pop();
      u1 *UIndex = (u1 *)Index.Header.Data;
      memcpy((void *)&Array.Header.Data[*UIndex * sizeof(struct Object)],
             (void *)&Value, sizeof(struct Object));
      break;
    };
    case op_sipush: {
      struct Object Short;
      Short.Type = ByteTA;
      Short.Header.Size = 2;
      Short.Header.Data = (u1 *)alloca(sizeof(short));
      *Short.Header.Data =
          Object->CodeReference->internalCode.instructions->at(++PC) |
          Object->CodeReference->internalCode.instructions->at(++PC);
      Object->OperandStack.push(&Short);
      break;
    }
    case op_aastore: {
      struct Object Value = *Object->OperandStack.top();
      Object->OperandStack.pop();
      struct Object Index = *Object->OperandStack.top();
      Object->OperandStack.pop();
      struct Object Array = *Object->OperandStack.top();
      Object->OperandStack.pop();
      u4 *UIndex = (u4 *)Index.Header.Data;
      memcpy((void *)&Array.Header.Data[*UIndex * sizeof(struct Object)],
             (void *)&Value, sizeof(struct Object));
      break;
    }
    case op_aconst_null: {
      struct Object Value;
      Value.Type = NULLTA;
      Object->OperandStack.push(&Value);
      break;
    }
    case op_aload: {
      struct Object Value;
      Value = Object->Locals.at(++PC);
      Object->OperandStack.push(&Value);
      break;
    };
    case op_aload_0: {
      struct Object Value;
      Value = Object->Locals.at(0);
      Object->OperandStack.push(&Value);
      break;
    }
    case op_aload_1: {
      struct Object Value;
      Value = Object->Locals.at(1);
      Object->OperandStack.push(&Value);
      break;
    }
    case op_aload_2: {
      struct Object Value;
      Value = Object->Locals.at(2);
      Object->OperandStack.push(&Value);
      break;
    }
    case op_aload_3: {
      struct Object Value;
      Value = Object->Locals.at(3);
      Object->OperandStack.push(&Value);
      break;
    }
    case op_new: {
      byte FirstByte =
          Object->CodeReference->internalCode.instructions->at(++PC);
      byte SecondByte =
          Object->CodeReference->internalCode.instructions->at(++PC);
      u2 CPIndex = FirstByte | SecondByte;
      Class *Link =
          Object->ClassPoolReference->pool.data.at(--CPIndex).Class.link;
      size_t Size = Link->fields.fields.size();
      struct Object *Obj = new struct Object();
      Obj->Type = ObjectTA;
      Obj->List.Count = 1;
      Obj->Header.Size = sizeof(struct Object) * Size;
      Obj->Header.Data = (u1 *)malloc(sizeof(struct Object) * Size);
      Object->OperandStack.push(Obj);
      break;
    }
    case op_newarray: {
      struct Object Index = *Object->OperandStack.top();
      u1 *UIndex = (u1 *)Index.Header.Data;
      Object->OperandStack.pop();
      byte FirstByte =
          Object->CodeReference->internalCode.instructions->at(++PC);
      switch (FirstByte) {
      case 4: {
        struct Object *Array = new struct Object();
        Array->Header.Size = *UIndex;
        Array->Type = BooleanArrayTA;
        Array->Header.Data = (u1 *)malloc(sizeof(jboolean) * *UIndex);
        Object->OperandStack.push(Array);
        break;
      }
      case 5: {
        struct Object *Array = new struct Object();
        Array->Header.Size = *UIndex;
        Array->Type = CharArrayTA;
        Array->Header.Data = (u1 *)malloc(sizeof(jchar) * *UIndex);
        Object->OperandStack.push(Array);
        break;
      }
      case 6: {
        struct Object *Array = new struct Object();
        Array->Header.Size = *UIndex;
        Array->Type = FloatArrayTA;
        Array->Header.Data = (u1 *)malloc(sizeof(jfloat) * *UIndex);
        Object->OperandStack.push(Array);
        break;
      }
      case 7: {
        struct Object *Array = new struct Object();
        Array->Header.Size = *UIndex;
        Array->Type = DoubleArrayTA;
        Array->Header.Data = (u1 *)malloc(sizeof(jdouble) * *UIndex);
        Object->OperandStack.push(Array);
        break;
      }
      case 8: {
        struct Object *Array = new struct Object();
        Array->Header.Size = *UIndex;
        Array->Type = ByteArrayTA;
        Array->Header.Data = (u1 *)malloc(sizeof(jbyte) * *UIndex);
        Object->OperandStack.push(Array);
        break;
      }
      case 9: {
        struct Object *Array = new struct Object();
        Array->Header.Size = *UIndex;
        Array->Type = ShortArrayTA;
        Array->Header.Data = (u1 *)malloc(sizeof(jshort) * *UIndex);
        Object->OperandStack.push(Array);
        break;
      }
      case 10: {
        struct Object *Array = new struct Object();
        Array->Header.Size = *UIndex;
        Array->Type = IntArrayTA;
        Array->Header.Data = (u1 *)malloc(sizeof(jint) * *UIndex);
        Object->OperandStack.push(Array);
        break;
      }
      case 11: {
        struct Object *Array = new struct Object();
        Array->Header.Size = *UIndex;
        Array->Type = LongArrayTA;
        Array->Header.Data = (u1 *)malloc(sizeof(jlong) * *UIndex);
        Object->OperandStack.push(Array);
        break;
      }
      }
      break;
    };
    case op_anewarray: {
      struct Object Index = *Object->OperandStack.top();
      Object->OperandStack.pop();
      byte FirstByte =
          Object->CodeReference->internalCode.instructions->at(PC++);
      byte SecondByte =
          Object->CodeReference->internalCode.instructions->at(PC++);
      u2 CPIndex = FirstByte | SecondByte;
      Class *Link =
          Object->ClassPoolReference->pool.data.at(--CPIndex).Class.link;
      size_t Size = Link->fields.fields.size();
      struct Object *Massive = new struct Object();
      Massive->Type = ObjectArrayTA;
      Massive->List.Count = 1;
      Massive->List.List = (Types *)malloc(1);
      *Massive->List.List = ObjectArrayTA;
      Massive->Header.Data = (u1 *)malloc(sizeof(struct Object) * Size);
      Object->OperandStack.push(Massive);
    };
    case op_aaload: {
      struct Object Index = *Object->OperandStack.top();
      struct Object Array = *Object->OperandStack.top();
      Object->OperandStack.pop();
      Object->OperandStack.pop();
      u4 *UIndex = (u4 *)Index.Header.Data;
      switch (Array.Type) {

      case ObjectArrayTA: {
        struct Object Element;
        Element.Type = ObjectTA;
        struct Object *ArrayElementLink =
            (struct Object *)&Array.Header
                .Data[*UIndex * (sizeof(struct Object))];
        Element.Header.Size = ArrayElementLink->Header.Size;
        Element.Header.Data = ArrayElementLink->Header.Data;
        Element.List = ArrayElementLink->List;
        Object->OperandStack.push(&Element);
        break;
      }
      case BooleanArrayTA: {
        struct Object Element;
        Element.Type = BooleanTA;
        struct Object *ArrayElementLink =
            (struct Object *)&Array.Header
                .Data[*UIndex * (sizeof(struct Object))];
        Element.Header.Size = ArrayElementLink->Header.Size;
        Element.Header.Data = ArrayElementLink->Header.Data;
        Element.List = ArrayElementLink->List;
        Object->OperandStack.push(&Element);
        break;
      }
      case ByteArrayTA: {
        struct Object Element;
        Element.Type = ByteTA;
        struct Object *ArrayElementLink =
            (struct Object *)&Array.Header
                .Data[*UIndex * (sizeof(struct Object))];
        Element.Header.Size = ArrayElementLink->Header.Size;
        Element.Header.Data = ArrayElementLink->Header.Data;
        Element.List = ArrayElementLink->List;
        Object->OperandStack.push(&Element);
        break;
      }
      case CharArrayTA: {
        struct Object Element;
        Element.Type = CharTA;
        struct Object *ArrayElementLink =
            (struct Object *)&Array.Header
                .Data[*UIndex * (sizeof(struct Object))];
        Element.Header.Size = ArrayElementLink->Header.Size;
        Element.Header.Data = ArrayElementLink->Header.Data;
        Element.List = ArrayElementLink->List;
        Object->OperandStack.push(&Element);
        break;
      }
      case ShortArrayTA: {
        struct Object Element;
        Element.Type = ShortTA;
        struct Object *ArrayElementLink =
            (struct Object *)&Array.Header
                .Data[*UIndex * (sizeof(struct Object))];
        Element.Header.Size = ArrayElementLink->Header.Size;
        Element.Header.Data = ArrayElementLink->Header.Data;
        Element.List = ArrayElementLink->List;
        Object->OperandStack.push(&Element);
        break;
      }
      case IntArrayTA: {
        struct Object Element;
        Element.Type = IntTA;
        struct Object *ArrayElementLink =
            (struct Object *)&Array.Header
                .Data[*UIndex * (sizeof(struct Object))];
        Element.Header.Size = ArrayElementLink->Header.Size;
        Element.Header.Data = ArrayElementLink->Header.Data;
        Element.List = ArrayElementLink->List;
        Object->OperandStack.push(&Element);
        break;
      }
      case LongArrayTA: {
        struct Object Element;
        Element.Type = LongTA;
        struct Object *ArrayElementLink =
            (struct Object *)&Array.Header
                .Data[*UIndex * (sizeof(struct Object))];
        Element.Header.Size = ArrayElementLink->Header.Size;
        Element.Header.Data = ArrayElementLink->Header.Data;
        Element.List = ArrayElementLink->List;
        Object->OperandStack.push(&Element);
        break;
      }
      case FloatArrayTA: {
        struct Object Element;
        Element.Type = FloatTA;
        struct Object *ArrayElementLink =
            (struct Object *)&Array.Header
                .Data[*UIndex * (sizeof(struct Object))];
        Element.Header.Size = ArrayElementLink->Header.Size;
        Element.Header.Data = ArrayElementLink->Header.Data;
        Element.List = ArrayElementLink->List;
        Object->OperandStack.push(&Element);
        break;
      }
      case DoubleArrayTA: {
        struct Object Element;
        Element.Type = DoubleTA;
        struct Object *ArrayElementLink =
            (struct Object *)&Array.Header
                .Data[*UIndex * (sizeof(struct Object))];
        Element.Header.Size = ArrayElementLink->Header.Size;
        Element.Header.Data = ArrayElementLink->Header.Data;
        Element.List = ArrayElementLink->List;
        Object->OperandStack.push(&Element);
        break;
      }
      default: {
        throw "NotImplemented";
        break;
      }
      }
      break;
    }
    }
  }
  return nullptr;
}

pointer *Heap::Resolve(Engine *Object) {
  if (this->arrAllocationBuffer != nullptr) {
    pointer *pnt = new pointer();
    objRef *ref = new objRef();
    ref->type = arrAllocationBuffer->arrayType;
    ref->count = arrAllocationBuffer->ArrayCount;
    ref->pointers =
        malloc(ref->count * sizeof(arrAllocationBuffer->ArraySizeofElement));
    this->objHeap.push_back(ref);
    pnt->type = ref->type;
    pnt->point = this->objHeap.back();
    return pnt;
  } else if (this->objAllocationBuffer != nullptr) {
    ClassLink *link = this->objAllocationBuffer;
    pointer *pnt = new pointer();
    objRef *ref = new objRef();
    ref->type = ObjectTA;
    ref->count = link->link->fields.fields.size();
    ref->pointers = malloc(ref->count * sizeof(objRef));
    this->objHeap.push_back(ref);
    pnt->type = ObjectTA;
    pnt->point = this->objHeap.back();
    return pnt;
  } else if (this->prAllocationBuffer != nullptr) {
    pointer *pnt = new pointer();
    valRef *ref = new valRef();
    ref->type = prAllocationBuffer->primType;
    ref->size = prAllocationBuffer->size;
    ref->data = malloc(ref->size);
    *(u1*)ref->data = prAllocationBuffer->data;
    pnt->type = ref->type;
    pnt->point = this->objHeap.back();
    return pnt;
  } else {
    throw "[HEAP CONTROLLER] NO ALLOCATION INFO";
  }
  this->objAllocationBuffer = nullptr;
  this->arrAllocationBuffer = nullptr;
  this->prAllocationBuffer = nullptr;
  return nullptr;
}
