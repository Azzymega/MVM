/*
 *
 *  * PROJECT:     MVM
 *  * LICENSE:     GPL - See COPYING in the top level directory
 *  * PROGRAMMER:  Maltsev Daniil <brickexberiment@lenta.ru>
 * 
 */

#include "engine.hpp"
#include "memory.hpp"
#include <corecrt.h>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <cwchar>
#include <deque>
#include <malloc.h>
#include <windef.h>

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
  if (Object->Locals.empty()) {
    Object->Locals = std::deque<pointer *>(Object->CodeReference->maxLocals +
                                           5); // Пусть будет
  }
  Object->PC = 0;
  for (; Object->PC < Object->CodeReference->internalCode.instructions->size();
       Object->PC++) {
    switch (Object->CodeReference->internalCode.instructions->at(Object->PC)) {
    case op_bipush: {
      struct PrimAllocationInfo Info {};
      Info.primType = ByteTA;
      Info.size = 1;
      Info.data =
          Object->CodeReference->internalCode.instructions->at(++Object->PC);
      this->CurrentHeap.prAllocationBuffer = &Info;
      Object->OperandStack.push(this->CurrentHeap.Resolve(this));
      break;
    }
    case op_dup: {
      Object->OperandStack.push(Object->OperandStack.top());
      break;
    };
    case op_areturn: {
      *(objRef *)Object->ReturnValue = *(objRef *)Object->OperandStack.top();
      this->Frames.Frames.pop_back();
      return nullptr;
    }
    case op_ldc: {
      byte Index =
          Object->CodeReference->internalCode.instructions->at(++Object->PC);
      switch (Object->ClassPoolReference->pool.data.at(Index - 1).tag) {
      case ClassT: {
        break; // Not implemented
      };
      case MethodT: {
        break; // Not implemented
      };
      case FieldT: {
        break; // Not implemented
      };
      case DoubleT: {
        struct PrimAllocationInfo Info {};
        Info.primType = DoubleTA;
        Info.size = 8;
        Info.data =
            Object->ClassPoolReference->pool.data.at(Index - 1)
                .Double.highBytes |
            Object->ClassPoolReference->pool.data.at(Index - 1).Double.lowBytes;
        this->CurrentHeap.prAllocationBuffer = &Info;
        Object->OperandStack.push(this->CurrentHeap.Resolve(this));
        break;
      }
      case StringT: {
        ArrayAllocationInfo info{};
        info.arrayType = ByteArrayTA;
        info.ArrayCount = Object->ClassPoolReference->pool.data.at(Index - 1)
                              .String.info.size();
        info.ArraySizeofElement = sizeof(char);
        this->CurrentHeap.arrAllocationBuffer = &info;
        Object->OperandStack.push(this->CurrentHeap.Resolve(this));
        objRef *arr = (objRef *)Object->OperandStack.top()->point;
        for (size_t x = 0; x < (info.ArrayCount * 16); x += sizeof(valRef)) {
          *(char *)((valRef *)((char *)arr->pointers + x))->data =
              Object->ClassPoolReference->pool.data.at(Index - 1)
                  .String.info.at(x / 16);
        }
        break;
      };
      case LongT: {
        struct PrimAllocationInfo Info {};
        Info.primType = LongTA;
        Info.size = 8;
        Info.data =
            Object->ClassPoolReference->pool.data.at(Index - 1).Long.highBytes |
            Object->ClassPoolReference->pool.data.at(Index - 1).Long.lowBytes;
        this->CurrentHeap.prAllocationBuffer = &Info;
        Object->OperandStack.push(this->CurrentHeap.Resolve(this));
        break;
      };
      case FloatT: {
        struct PrimAllocationInfo Info {};
        Info.primType = FloatTA;
        Info.size = 4;
        Info.data =
            Object->ClassPoolReference->pool.data.at(Index - 1).Float.bytes;
        this->CurrentHeap.prAllocationBuffer = &Info;
        Object->OperandStack.push(this->CurrentHeap.Resolve(this));
        break;
      };
      case IntegerT: {
        struct PrimAllocationInfo Info {};
        Info.primType = IntTA;
        Info.size = 4;
        Info.data =
            Object->ClassPoolReference->pool.data.at(Index - 1).Integer.bytes;
        this->CurrentHeap.prAllocationBuffer = &Info;
        Object->OperandStack.push(this->CurrentHeap.Resolve(this));
        break;
      };
      case UnknownT:
        break;
      }
      break;
    };
    case op_pop: {
      Object->OperandStack.pop();
      break;
    };
    case op_nop: {
      break;
    };
    case op_getfield: {
      byte FirstByte =
          Object->CodeReference->internalCode.instructions->at(++Object->PC);
      byte SecondByte =
          Object->CodeReference->internalCode.instructions->at(++Object->PC);
      u2 CPIndex = FirstByte | SecondByte;
      FieldLink Link =
          Object->ClassPoolReference->pool.data.at(--CPIndex).Field;
      int FieldOffset = 0;
      for (auto &&fld : Link.data.Class.link->fields.fields) {
        if (fld.name == Link.Field->name &&
            fld.signature == Link.Field->signature) {
          break;
        } else {
          FieldOffset += 4;
        }
      }
      pointer *ObjectLink = Object->OperandStack.top();
      Object->OperandStack.pop();
      objRef *element = (objRef *)malloc(sizeof(objRef));
      objRef *lnk = (objRef *)ObjectLink->point;
      objRef *fld = (objRef *)((char *)lnk->pointers + FieldOffset);
      memcpy(element, fld, sizeof(objRef));
      pointer pnt;
      pnt.type = element->type;
      pnt.point = element;
      Object->OperandStack.push(&pnt);
      break;
    }
    case op_putfield: {
      byte FirstByte =
          Object->CodeReference->internalCode.instructions->at(++Object->PC);
      byte SecondByte =
          Object->CodeReference->internalCode.instructions->at(++Object->PC);
      u2 CPIndex = FirstByte | SecondByte;
      FieldLink Link =
          Object->ClassPoolReference->pool.data.at(--CPIndex).Field;
      int FieldOffset = 0;
      for (auto &&fld : Link.data.Class.link->fields.fields) {
        if (fld.name == Link.Field->name &&
            fld.signature == Link.Field->signature) {
          break;
        } else {
          FieldOffset += 4;
        }
      }
      pointer *Value = Object->OperandStack.top();
      Object->OperandStack.pop();
      pointer *ObjectLink = Object->OperandStack.top();
      Object->OperandStack.pop();
      objRef *ref = (objRef *)ObjectLink->point;
      memcpy((void *)((char *)ref->pointers + FieldOffset), Value->point,
             sizeof(objRef));
      break;
    };
    case op_astore_0: {
      pointer *Data{};
      Data = Object->OperandStack.top();
      Object->OperandStack.pop();
      Object->Locals.at(0) = Data;
      break;
    }
    case op_astore_1: {
      pointer *Data{};
      Data = Object->OperandStack.top();
      Object->OperandStack.pop();
      Object->Locals.at(1) = Data;
      break;
    }
    case op_astore_2: {
      pointer *Data{};
      Data = Object->OperandStack.top();
      Object->OperandStack.pop();
      Object->Locals.at(2) = Data;
      break;
    }
    case op_astore_3: {
      pointer *Data{};
      Data = Object->OperandStack.top();
      Object->OperandStack.pop();
      Object->Locals.at(3) = Data;
      break;
    }
    case op_iconst_0: {
      struct PrimAllocationInfo Info {};
      Info.primType = ByteTA;
      Info.size = 1;
      Info.data = 3;
      this->CurrentHeap.prAllocationBuffer = &Info;
      Object->OperandStack.push(this->CurrentHeap.Resolve(this));
      break;
    }
    case op_iconst_1: {
      struct PrimAllocationInfo Info {};
      Info.primType = ByteTA;
      Info.size = 1;
      Info.data = 4;
      this->CurrentHeap.prAllocationBuffer = &Info;
      Object->OperandStack.push(this->CurrentHeap.Resolve(this));
      break;
    }
    case op_iconst_2: {
      struct PrimAllocationInfo Info {};
      Info.primType = ByteTA;
      Info.size = 1;
      Info.data = 5;
      this->CurrentHeap.prAllocationBuffer = &Info;
      Object->OperandStack.push(this->CurrentHeap.Resolve(this));
      break;
    }
    case op_iconst_3: {
      struct PrimAllocationInfo Info {};
      Info.primType = ByteTA;
      Info.size = 1;
      Info.data = 6;
      this->CurrentHeap.prAllocationBuffer = &Info;
      Object->OperandStack.push(this->CurrentHeap.Resolve(this));
      break;
    }
    case op_iconst_4: {
      struct PrimAllocationInfo Info {};
      Info.primType = ByteTA;
      Info.size = 1;
      Info.data = 7;
      this->CurrentHeap.prAllocationBuffer = &Info;
      Object->OperandStack.push(this->CurrentHeap.Resolve(this));
      break;
    }
    case op_iconst_5: {
      struct PrimAllocationInfo Info {};
      Info.primType = ByteTA;
      Info.size = 1;
      Info.data = 8;
      this->CurrentHeap.prAllocationBuffer = &Info;
      Object->OperandStack.push(this->CurrentHeap.Resolve(this));
      break;
    }
    case op_return: {
      this->Frames.Frames.pop_back();
      return nullptr;
    };
    case op_invokestatic: {
      bool isNative{};
      std::vector<pointer> Objects{};
      while (true) {
        pointer Arg = *Object->OperandStack.top();
        Object->OperandStack.pop();
        Objects.push_back(Arg);
        if (Object->OperandStack.empty()) {
          break;
        }
      }
      Frame initFrame;
      for (auto &&arg : Objects) {
        initFrame.Locals.push_back(&arg);
      }
      byte FirstByte =
          Object->CodeReference->internalCode.instructions->at(++Object->PC);
      byte SecondByte =
          Object->CodeReference->internalCode.instructions->at(++Object->PC);
      u2 CPIndex = FirstByte | SecondByte;
      MethodLink Link =
          Object->ClassPoolReference->pool.data.at(--CPIndex).Method;
      for (auto &&flag : Link.Method->methodFlags.flags) {
        if (flag == accessFlags::ACC_NATIVE) {
          if (Link.Method->name == "LoadNativeLibrary") {
            NativeLibrary lib;
            std::string name;
            objRef *reference = (objRef *)Objects.at(0).point;
            for (size_t i = 0; i < reference->count * 16; i += sizeof(valRef)) {
              valRef *value = (valRef *)((char *)reference->pointers + i);
              name += *(char *)value->data;
            }
            lib.libraryName = name;
            this->NBus.Resolve(&lib);
            Object->OperandStack.push(new pointer());
            isNative = true;
            break;
          } else {
            std::string funName =
                "Java_" + Link.data.Class.className + "_" + Link.Method->name;
            ParameterBundle bundle;
            bundle.returnValue.stackTop = nullptr;
            bundle.args.Objects = Objects.at(0).point;
            this->NBus.Resolve(&funName);
            if (NBus.buffer.BufferedCall) {

              NBus.buffer.BufferedCall(bundle);
              isNative = true;
            }
            this->Frames.Frames.push_back(&initFrame);
          }
        }
      }
      if (!isNative) {
        if (Link.Method->signature.back() == 'V') {
          initFrame.ClassPoolReference = Link.data.Class.link;
          initFrame.MethodReference = Link.Method;
          initFrame.CodeReference =
              &Link.Method->attributes.attributes.at(0).code;
          this->Frames.Frames.push_back(&initFrame);
          Resolve(&initFrame);
        } else {
          pointer ret{};
          Object->OperandStack.push(&ret);
          initFrame.ReturnValue = Object->OperandStack.top();
          initFrame.ClassPoolReference = Link.data.Class.link;
          initFrame.MethodReference = Link.Method;
          initFrame.CodeReference =
              &Link.Method->attributes.attributes.at(0).code;
          this->Frames.Frames.push_back(&initFrame);
          Resolve(&initFrame);
        }
        break;
      } else {
        break;
      }
      break;
    }
    case op_invokevirtual: {
      bool isNative{};
      std::vector<pointer> Objects{};
      while (true) {
        pointer Arg = *Object->OperandStack.top();
        Object->OperandStack.pop();
        Objects.push_back(Arg);
        if (Arg.type == ObjectTA) {
          break;
        }
      }
      Frame initFrame;
      for (auto &&arg : Objects) {
        initFrame.Locals.push_back(&arg);
      }
      byte FirstByte =
          Object->CodeReference->internalCode.instructions->at(++Object->PC);
      byte SecondByte =
          Object->CodeReference->internalCode.instructions->at(++Object->PC);
      u2 CPIndex = FirstByte | SecondByte;
      MethodLink Link =
          Object->ClassPoolReference->pool.data.at(--CPIndex).Method;
      for (auto &&flag : Link.Method->methodFlags.flags) {
        if (flag == accessFlags::ACC_NATIVE) {
          if (Link.Method->name == "LoadNativeLibrary") {
            NativeLibrary lib;
            std::string name;
            objRef *reference = (objRef *)Objects.at(0).point;
            for (size_t i = 0; i < reference->count * 16; i += sizeof(valRef)) {
              valRef *value = (valRef *)((char *)reference->pointers + i);
              name += *(char *)value->data;
            }
            lib.libraryName = name;
            this->NBus.Resolve(&lib);
            Object->OperandStack.push(new pointer());
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
            this->Frames.Frames.push_back(&initFrame);
          }
        }
      }
      if (!isNative) {
        if (Link.Method->signature.back() == 'V') {
          initFrame.ClassPoolReference = Link.data.Class.link;
          initFrame.MethodReference = Link.Method;
          initFrame.CodeReference =
              &Link.Method->attributes.attributes.at(0).code;
          this->Frames.Frames.push_back(&initFrame);
          Resolve(&initFrame);
        } else {
          pointer ret{};
          Object->OperandStack.push(&ret);
          initFrame.ReturnValue = Object->OperandStack.top();
          initFrame.ClassPoolReference = Link.data.Class.link;
          initFrame.MethodReference = Link.Method;
          initFrame.CodeReference =
              &Link.Method->attributes.attributes.at(0).code;
          this->Frames.Frames.push_back(&initFrame);
          Resolve(&initFrame);
        }
        break;
      } else {
        break;
      }
      break;
    };
    case op_invokespecial: {
      if (Object->ClassPoolReference->mainInfo.superClass.className == "NOPE") {
        this->Frames.Frames.pop_back();
        return nullptr;
      }
      std::vector<pointer> Objects;
      bool reading = true;
      while (reading) {
        pointer Arg = *Object->OperandStack.top();
        Object->OperandStack.pop();
        Objects.push_back(Arg);
        if (Arg.type == ObjectTA) {
          reading = false;
          break;
        }
      }
      Frame initFrame;
      for (auto &&arg : Objects) {
        initFrame.Locals.push_front(&arg);
      }
      byte FirstByte =
          Object->CodeReference->internalCode.instructions->at(++Object->PC);
      byte SecondByte =
          Object->CodeReference->internalCode.instructions->at(++Object->PC);
      u2 CPIndex = FirstByte | SecondByte;
      MethodLink Link =
          Object->ClassPoolReference->pool.data.at(--CPIndex).Method;
      if (Link.Method->signature.back() == 'V') {
        initFrame.ClassPoolReference = Link.data.Class.link;
        initFrame.MethodReference = Link.Method;
        initFrame.CodeReference =
            &Link.Method->attributes.attributes.at(0).code;
        this->Frames.Frames.push_back(&initFrame);
        Resolve(&initFrame);
      } else {
        pointer ret{};
        Object->OperandStack.push(&ret);
        initFrame.ReturnValue = Object->OperandStack.top();
        initFrame.ClassPoolReference = Link.data.Class.link;
        initFrame.MethodReference = Link.Method;
        initFrame.CodeReference =
            &Link.Method->attributes.attributes.at(0).code;
        this->Frames.Frames.push_back(&initFrame);
        Resolve(&initFrame);
      }
      break;
    };
    case op_castore: {
      pointer Value = *Object->OperandStack.top();
      Object->OperandStack.pop();
      pointer Index = *Object->OperandStack.top();
      Object->OperandStack.pop();
      pointer Array = *Object->OperandStack.top();
      Object->OperandStack.pop();
      u1 index = --*(u1 *)((valRef *)Index.point)->data;
      char value = *(char *)((valRef *)Value.point)->data;
      objRef *array = (objRef *)Array.point;
      *((char *)((valRef *)((char *)array->pointers + (index * sizeof(valRef))))
            ->data) = value;
      break;
    };
    case op_sipush: {
      struct PrimAllocationInfo Info {};
      Info.primType = ShortTA;
      Info.size = 1;
      Info.data =
          Object->CodeReference->internalCode.instructions->at(++Object->PC) |
          Object->CodeReference->internalCode.instructions->at(++Object->PC);
      this->CurrentHeap.prAllocationBuffer = &Info;
      Object->OperandStack.push(this->CurrentHeap.Resolve(this));
      break;
    }
    case op_aastore: {
      /*
              struct Object Value = *Object->OperandStack.top();
        Object->OperandStack.pop();
        struct Object Index = *Object->OperandStack.top();
        Object->OperandStack.pop();
        struct Object Array = *Object->OperandStack.top();
        Object->OperandStack.pop();
        u4 *UIndex = (u4 *)Index.Header.Data;
        memcpy((void *)&Array.Header.Data[*UIndex * sizeof(struct Object)],
              (void *)&Value, sizeof(struct Object));
      */
      break;
    }
    case op_aconst_null: {
      Object->OperandStack.push(this->CurrentHeap.Resolve(this));
      break;
    }
    case op_aload: {
      pointer *Value;
      Value = Object->Locals.at(++Object->PC);
      Object->OperandStack.push(Value);
      break;
    };
    case op_aload_0: {
      pointer *Value;
      Value = Object->Locals.at(0);
      Object->OperandStack.push(Value);
      break;
    }
    case op_aload_1: {
      pointer *Value;
      Value = Object->Locals.at(1);
      Object->OperandStack.push(Value);
      break;
    }
    case op_aload_2: {
      pointer *Value;
      Value = Object->Locals.at(2);
      Object->OperandStack.push(Value);
      break;
    }
    case op_aload_3: {
      pointer *Value;
      Value = Object->Locals.at(3);
      Object->OperandStack.push(Value);
      break;
    }
    case op_new: {
      byte FirstByte =
          Object->CodeReference->internalCode.instructions->at(++Object->PC);
      byte SecondByte =
          Object->CodeReference->internalCode.instructions->at(++Object->PC);
      u2 CPIndex = FirstByte | SecondByte;
      ClassLink *Link =
          &Object->ClassPoolReference->pool.data.at(--CPIndex).Class;
      this->CurrentHeap.objAllocationBuffer = Link;
      Object->OperandStack.push(this->CurrentHeap.Resolve(this));
      break;
    }
    case op_newarray: {
      pointer *Index = Object->OperandStack.top();
      u1 index = *(u1 *)((valRef *)Index->point)->data;
      Object->OperandStack.pop();
      byte FirstByte =
          Object->CodeReference->internalCode.instructions->at(++Object->PC);
      switch (FirstByte) {
      case 4: {
        ArrayAllocationInfo info{};
        info.arrayType = BooleanArrayTA;
        info.ArrayCount = index;
        info.ArraySizeofElement = sizeof(valRef);
        this->CurrentHeap.arrAllocationBuffer = &info;
        Object->OperandStack.push(this->CurrentHeap.Resolve(this));
        break;
      }
      case 5: {
        ArrayAllocationInfo info{};
        info.arrayType = CharArrayTA;
        info.ArrayCount = index;
        info.ArraySizeofElement = sizeof(valRef);
        this->CurrentHeap.arrAllocationBuffer = &info;
        Object->OperandStack.push(this->CurrentHeap.Resolve(this));
        break;
      }
      case 6: {
        ArrayAllocationInfo info{};
        info.arrayType = FloatArrayTA;
        info.ArrayCount = index;
        info.ArraySizeofElement = sizeof(valRef);
        this->CurrentHeap.arrAllocationBuffer = &info;
        Object->OperandStack.push(this->CurrentHeap.Resolve(this));
        break;
      }
      case 7: {
        ArrayAllocationInfo info{};
        info.arrayType = DoubleArrayTA;
        info.ArrayCount = index;
        info.ArraySizeofElement = sizeof(valRef);
        this->CurrentHeap.arrAllocationBuffer = &info;
        Object->OperandStack.push(this->CurrentHeap.Resolve(this));
        break;
      }
      case 8: {
        ArrayAllocationInfo info{};
        info.arrayType = ByteArrayTA;
        info.ArrayCount = index;
        info.ArraySizeofElement = sizeof(valRef);
        this->CurrentHeap.arrAllocationBuffer = &info;
        Object->OperandStack.push(this->CurrentHeap.Resolve(this));
        break;
      }
      case 9: {
        ArrayAllocationInfo info{};
        info.arrayType = ShortArrayTA;
        info.ArrayCount = index;
        info.ArraySizeofElement = sizeof(valRef);
        this->CurrentHeap.arrAllocationBuffer = &info;
        Object->OperandStack.push(this->CurrentHeap.Resolve(this));
        break;
      }
      case 10: {
        ArrayAllocationInfo info{};
        info.arrayType = IntArrayTA;
        info.ArrayCount = index;
        info.ArraySizeofElement = sizeof(valRef);
        this->CurrentHeap.arrAllocationBuffer = &info;
        Object->OperandStack.push(this->CurrentHeap.Resolve(this));
        break;
      }
      case 11: {
        ArrayAllocationInfo info{};
        info.arrayType = LongArrayTA;
        info.ArrayCount = index;
        info.ArraySizeofElement = sizeof(valRef);
        this->CurrentHeap.arrAllocationBuffer = &info;
        Object->OperandStack.push(this->CurrentHeap.Resolve(this));
        break;
      }
      }
      break;
    };
    /*
          pointer *Index = Object->OperandStack.top();
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
    */
    case op_anewarray: {
      break;
    };
    case op_aaload: {
      /*
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
      */

    } break;
    }
  }
  return nullptr;
}

pointer *Heap::Resolve(Engine *Object) {
  if (this->arrAllocationBuffer) {
    pointer *pnt = new pointer();
    objRef *ref = new objRef();
    ref->type = arrAllocationBuffer->arrayType;
    ref->count = arrAllocationBuffer->ArrayCount;
    ref->pointers =
        malloc(ref->count * sizeof(arrAllocationBuffer->ArraySizeofElement));
    switch (ref->type) {
    case ObjectTA: {
      break;
    };
    case StringTA: {
      break;
    }
    case WeakTA: {
      break;
    }
    case ThrowableTA: {
      break;
    }
    case ClassTA: {
      break;
    }
    case MethodIDTA: {
      break;
    }
    case FieldIDTA: {
      break;
    }
    case ArrayTA: {
      break;
    }
    case ObjectArrayTA: {
      for (size_t x = 0; x < ref->count * sizeof(valRef); x += sizeof(valRef)) {
        objRef *pointer = (objRef *)((char *)ref->pointers + x);
        pointer->type = ObjectTA;
      }
      break;
    }
    case BooleanArrayTA: {
      for (size_t x = 0; x < ref->count * sizeof(valRef); x += sizeof(valRef)) {
        valRef *pointer = (valRef *)((char *)ref->pointers + x);
        pointer->type = BooleanTA;
        pointer->size = 1;
        pointer->data = malloc(arrAllocationBuffer->ArraySizeofElement);
        *(byte *)pointer->data = 0;
      }
      break;
    }
    case ByteArrayTA: {
      for (size_t x = 0; x < ref->count * sizeof(valRef); x += sizeof(valRef)) {
        valRef *pointer = (valRef *)((char *)ref->pointers + x);
        pointer->type = ByteTA;
        pointer->size = 1;
        pointer->data = malloc(arrAllocationBuffer->ArraySizeofElement);
        *(byte *)pointer->data = 0;
      }
      break;
    }
    case CharArrayTA: {
      for (size_t x = 0; x < ref->count * sizeof(valRef); x += sizeof(valRef)) {
        valRef *pointer = (valRef *)((char *)ref->pointers + x);
        pointer->type = CharTA;
        pointer->size = 2;
        pointer->data = malloc(arrAllocationBuffer->ArraySizeofElement);
        *(short *)pointer->data = 0;
      }
      break;
    }
    case ShortArrayTA: {
      for (size_t x = 0; x < ref->count * sizeof(valRef); x += sizeof(valRef)) {
        valRef *pointer = (valRef *)((char *)ref->pointers + x);
        pointer->type = ShortTA;
        pointer->size = 2;
        pointer->data = malloc(arrAllocationBuffer->ArraySizeofElement);
        *(short *)pointer->data = 0;
      }
      break;
    }
    case IntArrayTA: {
      for (size_t x = 0; x < ref->count * sizeof(valRef); x += sizeof(valRef)) {
        valRef *pointer = (valRef *)((char *)ref->pointers + x);
        pointer->type = IntTA;
        pointer->size = 4;
        pointer->data = malloc(arrAllocationBuffer->ArraySizeofElement);
        *(int *)pointer->data = 0;
      }
      break;
    }
    case LongArrayTA: {
      for (size_t x = 0; x < ref->count * sizeof(valRef); x += sizeof(valRef)) {
        valRef *pointer = (valRef *)((char *)ref->pointers + x);
        pointer->type = LongTA;
        pointer->size = 8;
        pointer->data = malloc(arrAllocationBuffer->ArraySizeofElement);
        *(long *)pointer->data = 0;
      }
      break;
    }
    case FloatArrayTA: {
      for (size_t x = 0; x < ref->count * sizeof(valRef); x += sizeof(valRef)) {
        valRef *pointer = (valRef *)((char *)ref->pointers + x);
        pointer->type = FloatTA;
        pointer->size = 4;
        pointer->data = malloc(arrAllocationBuffer->ArraySizeofElement);
        *(float *)pointer->data = 0;
      }
      break;
    }
    case DoubleArrayTA: {
      for (size_t x = 0; x < ref->count * sizeof(valRef); x += sizeof(valRef)) {
        valRef *pointer = (valRef *)((char *)ref->pointers + x);
        pointer->type = DoubleTA;
        pointer->size = 8;
        pointer->data = malloc(arrAllocationBuffer->ArraySizeofElement);
        *(double *)pointer->data = 0;
      }
      break;
    }
    case BooleanTA: {
      break;
    }
    case ByteTA: {
      break;
    }
    case CharTA: {
      break;
    }
    case ShortTA: {
      break;
    }
    case IntTA: {
      break;
    }
    case LongTA: {
      break;
    }
    case FloatTA: {
      break;
    }
    case DoubleTA: {
      break;
    }
    case SizeTA: {
      break;
    }
    case ObjReference: {
      break;
    }
    case ValReference: {
      break;
    }
    case NULLTA:
      throw "WRONG MOVE";
      break;
    }
    this->objHeap.push_back(ref);
    pnt->type = ref->type;
    pnt->point = this->objHeap.back();
    this->objAllocationBuffer = nullptr;
    this->arrAllocationBuffer = nullptr;
    this->prAllocationBuffer = nullptr;
    return pnt;
  } else if (this->objAllocationBuffer) {
    ClassLink *link = this->objAllocationBuffer;
    if (link == nullptr) {
      pointer *pnt = new pointer();
      objRef *ref = new objRef();
      ref->type = NULLTA;
      ref->count = 0;
      ref->pointers = malloc(ref->count * sizeof(objRef));
      this->objHeap.push_back(ref);
      pnt->type = NULLTA;
      pnt->point = this->objHeap.back();
      this->objAllocationBuffer = nullptr;
      this->arrAllocationBuffer = nullptr;
      this->prAllocationBuffer = nullptr;
      return pnt;
    }
    pointer *pnt = new pointer();
    objRef *ref = new objRef();
    ref->type = ObjectTA;
    ref->count = link->link->fields.fields.size();
    ref->pointers = malloc(ref->count * sizeof(objRef));
    this->objHeap.push_back(ref);
    pnt->type = ObjectTA;
    pnt->point = this->objHeap.back();
    return pnt;
  } else if (this->prAllocationBuffer) {
    pointer *pnt = new pointer();
    valRef *ref = new valRef();
    ref->type = prAllocationBuffer->primType;
    ref->size = prAllocationBuffer->size;
    ref->data = malloc(ref->size);
    *(u1 *)ref->data = prAllocationBuffer->data;
    pnt->type = ref->type;
    pnt->point = this->objHeap.back();
    this->objAllocationBuffer = nullptr;
    this->arrAllocationBuffer = nullptr;
    this->prAllocationBuffer = nullptr;
    return pnt;
  } else {
    throw "[HEAP CONTROLLER] NO ALLOCATION INFO";
  }
  this->objAllocationBuffer = nullptr;
  this->arrAllocationBuffer = nullptr;
  this->prAllocationBuffer = nullptr;
  return nullptr;
}
