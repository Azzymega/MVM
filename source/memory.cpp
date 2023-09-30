#include "memory.hpp"
#include <vector>

void Memory::Interact(Linker *Object) { Object->Resolve(this); }

Memory *Memory::Resolve(classFile *Object) {
  this->buffer.buffer = Object;
  this->linker.buffer = this;
  Class *Klass = new Class();
  this->buffer.klassBuffer = Klass;
  Resolve(&Klass->pool);
  Resolve(&Klass->mainInfo);
  Resolve(&Klass->interfaces);
  Resolve(&Klass->fields);
  Resolve(&Klass->methods);
  this->buffer.attrBuffer = &this->buffer.buffer->attr;
  Resolve(&Klass->attr);
  this->info.classes.push_back(Klass);
  linker.Resolve(&info);
  return nullptr;
}

Memory *Memory::Resolve(ClassHeader *Object) {
  Object->minVersion = this->buffer.buffer->data.minVersion;
  Object->maxVersion = this->buffer.buffer->data.maxVersion;
  Object->flags = Resolve(&this->buffer.buffer->data.accessFlags);
  Object->Class.className = this->buffer.buffer->pool.Resolve(
      this->buffer.buffer->pool
          .elements[this->buffer.buffer->data.thisClass - 1]
          .constantClass.nameIndex -
      1);
  Object->superClass.className = this->buffer.buffer->pool.Resolve(
      this->buffer.buffer->pool
          .elements[this->buffer.buffer->data.superClass - 1]
          .constantClass.nameIndex -
      1);
  return nullptr;
}

AccessFlags Memory::Resolve(unsigned short *Object) {
  AccessFlags flags;
  if ((*Object & accessFlags::ACC_FINAL) != 0) {
    flags.flags.push_back(ACC_PUBLIC);
  }
  if ((*Object & accessFlags::ACC_ABSTRACT) != 0) {
    flags.flags.push_back(ACC_PUBLIC);
  }
  if ((*Object & accessFlags::ACC_PRIVATE) != 0) {
    flags.flags.push_back(ACC_PUBLIC);
  }
  if ((*Object & accessFlags::ACC_PROTECTED) != 0) {
    flags.flags.push_back(ACC_PUBLIC);
  }
  if ((*Object & accessFlags::ACC_PUBLIC) != 0) {
    flags.flags.push_back(ACC_PUBLIC);
  }
  if ((*Object & accessFlags::ACC_STATIC) != 0) {
    flags.flags.push_back(ACC_PUBLIC);
  }
  if ((*Object & accessFlags::ACC_SYNTHETIC) != 0) {
    flags.flags.push_back(ACC_PUBLIC);
  }
  if ((*Object & accessFlags::ACC_ANNOTATION) != 0) {
    flags.flags.push_back(ACC_PUBLIC);
  }
  if ((*Object & accessFlags::ACC_CENUM) != 0) {
    flags.flags.push_back(ACC_PUBLIC);
  }
  if ((*Object & accessFlags::ACC_INTERFACE) != 0) {
    flags.flags.push_back(ACC_PUBLIC);
  }
  if ((*Object & accessFlags::ACC_SUPER) != 0) {
    flags.flags.push_back(ACC_PUBLIC);
  }
  return flags;
}

Memory *Memory::Resolve(DataPool *Object) {
  DataPoolType poolElement;
  for (constantPoolType type : this->buffer.buffer->pool.elements) {
    switch (type.tag) {
    case cpType::CONSTANT_Class:
      poolElement.tag = ClassT;
      poolElement.Class = ClassLink{*this->buffer.buffer->pool.elements
                                         .at(type.constantClass.nameIndex - 1)
                                         .constantUTF8.utf8,
                                    nullptr};
      Object->data.emplace_back(poolElement);
      break;
    case cpType::CONSTANT_Double:
      poolElement.tag = DoubleT;
      poolElement.Double = type.constantDouble;
      Object->data.emplace_back(poolElement);
      break;
    case cpType::CONSTANT_Fieldref:
      poolElement.tag = DataPoolTypes::FieldT;
      poolElement.Field = FieldLink{
          TypeMetadata{
              ClassLink{*this->buffer.buffer->pool.elements
                             .at(this->buffer.buffer->pool.elements
                                     .at(type.constantFieldRef.classIndex)
                                     .constantClass.nameIndex)
                             .constantUTF8.utf8,
                        nullptr},
              NameAndType{
                  *this->buffer.buffer->pool.elements
                       .at(this->buffer.buffer->pool.elements
                               .at(type.constantFieldRef.nameAndTypeIndex)
                               .constantNameAndType.nameIndex)
                       .constantUTF8.utf8,
                  *this->buffer.buffer->pool.elements
                       .at(this->buffer.buffer->pool.elements
                               .at(type.constantFieldRef.nameAndTypeIndex)
                               .constantNameAndType.nameIndex)
                       .constantUTF8.utf8}},
          nullptr};
      Object->data.emplace_back(poolElement);
      break;
    case cpType::CONSTANT_Float:
      poolElement.tag = FloatT;
      poolElement.Float = type.constantFloat;
      Object->data.emplace_back(poolElement);
      break;
    case cpType::CONSTANT_Integer:
      poolElement.tag = IntegerT;
      poolElement.Integer = type.constantInteger;
      Object->data.emplace_back(poolElement);
      break;
    case cpType::CONSTANT_InterfaceMethodref:
      poolElement.tag = DataPoolTypes::MethodT;
      poolElement.Method = MethodLink{
          TypeMetadata{
              ClassLink{std::string(
                            *this->buffer.buffer->pool.elements
                                 .at(this->buffer.buffer->pool.elements
                                         .at(type.constantMethodRef.classIndex)
                                         .constantClass.nameIndex)
                                 .constantUTF8.utf8),
                        nullptr},
              NameAndType{
                  std::string(
                      *this->buffer.buffer->pool.elements
                           .at(this->buffer.buffer->pool.elements
                                   .at(type.constantMethodRef.nameAndTypeIndex)
                                   .constantNameAndType.nameIndex)
                           .constantUTF8.utf8),
                  std::string(
                      *this->buffer.buffer->pool.elements
                           .at(this->buffer.buffer->pool.elements
                                   .at(type.constantMethodRef.nameAndTypeIndex)
                                   .constantNameAndType.descriptorIndex)
                           .constantUTF8.utf8)}},
          nullptr};
      Object->data.emplace_back(poolElement);
      break;
    case cpType::CONSTANT_Long:
      poolElement.tag = LongT;
      poolElement.Long = type.constantLong;
      Object->data.emplace_back(poolElement);
      break;
    case cpType::CONSTANT_Methodref:
      poolElement.tag = DataPoolTypes::MethodT;
      poolElement.Method = MethodLink{
          TypeMetadata{
              ClassLink{*this->buffer.buffer->pool.elements
                             .at(this->buffer.buffer->pool.elements
                                     .at(type.constantMethodRef.classIndex - 1)
                                     .constantClass.nameIndex -
                                 1)
                             .constantUTF8.utf8,
                        nullptr},
              NameAndType{
                  *this->buffer.buffer->pool.elements
                       .at(this->buffer.buffer->pool.elements
                               .at(type.constantMethodRef.classIndex - 1)
                               .constantNameAndType.nameIndex)
                       .constantUTF8.utf8,
                  *this->buffer.buffer->pool.elements
                       .at(this->buffer.buffer->pool.elements
                               .at(type.constantMethodRef.nameAndTypeIndex - 1)
                               .constantNameAndType.descriptorIndex -
                           1)
                       .constantUTF8.utf8}},
          nullptr};
      Object->data.emplace_back(poolElement);
      break;
    case cpType::CONSTANT_NameAndType:
      poolElement.tag = UnknownT;
      poolElement.Boiler.magic = 1;
      Object->data.emplace_back(poolElement);
      break;
    case cpType::CONSTANT_String:
      poolElement.tag = StringT;
      poolElement.String.info = *this->buffer.buffer->pool.elements
                                     .at(type.constantString.stringIndex - 1)
                                     .constantUTF8.utf8;
      Object->data.emplace_back(poolElement);
      break;
    case cpType::CONSTANT_Utf8:
      poolElement.tag = StringT;
      poolElement.String.info = *type.constantUTF8.utf8;
      Object->data.emplace_back(poolElement);
      break;
    }
    poolElement = DataPoolType();
  }
  return nullptr;
}

Memory *Memory::Resolve(Interfaces *Object) {
  for (u2 interfaceIndex : this->buffer.buffer->intrf.interfaces) {
    Object->interfaces.emplace_back(Interface{
        std::string(*this->buffer.buffer->pool.elements.at(interfaceIndex)
                         .constantUTF8.utf8),
        nullptr});
  }
  return nullptr;
}

Memory *Memory::Resolve(Fields *Object) {
  for (field fld : this->buffer.buffer->flds.elements) {
    NAttributes attrs;
    this->buffer.attrBuffer = &fld.attributes;
    Resolve(&attrs);
    Object->fields.emplace_back(Field{
        Resolve(&fld.access_flags),
        std::string(*this->buffer.buffer->pool.elements.at(fld.name_index - 1)
                         .constantUTF8.utf8),
        std::string(
            *this->buffer.buffer->pool.elements.at(fld.signature_index - 1)
                 .constantUTF8.utf8),
        attrs});
  }
  return nullptr;
}

Memory *Memory::Resolve(Methods *Object) {
  for (method mth : this->buffer.buffer->mthds.elements) {
    NAttributes attrs = NAttributes();
    this->buffer.attrBuffer = &mth.attributes;
    Resolve(&attrs);
    Object->methods.emplace_back(Method{
        Resolve(&mth.access_flags),
        std::string(*this->buffer.buffer->pool.elements.at(mth.name_index - 1)
                         .constantUTF8.utf8),
        std::string(
            *this->buffer.buffer->pool.elements.at(mth.signature_index - 1)
                 .constantUTF8.utf8),
        attrs});
  }
  return nullptr;
}

Memory *Memory::Resolve(NAttributes *Object) {
  NAttribute Nattr = NAttribute();
  for (auto &&attr : *this->buffer.attrBuffer->attr) {
    if (this->buffer.buffer->pool.Resolve(attr.name - 1) == "SourceFile") {
      Nattr.name = &this->buffer.klassBuffer->pool.data.at(attr.name - 1);
      Nattr.type = SmartAttributesType::NTSourceFile;
      Nattr.length = attr.length;
      Nattr.sourceFile.name =
          &this->buffer.klassBuffer->pool.data.at(attr.sourceFile.index - 1);
      Object->attributes.push_back(Nattr);
    } else if (this->buffer.buffer->pool.Resolve(attr.name - 1) ==
               "ConstantValue") {
      Nattr.name = &this->buffer.klassBuffer->pool.data.at(attr.name - 1);
      Nattr.type = SmartAttributesType::NTConstantValue;
      Nattr.length = attr.length;
      Nattr.constantValue.constant =
          &this->buffer.klassBuffer->pool.data.at(attr.constantValue.index - 1);
      Object->attributes.push_back(Nattr);
    } else if (this->buffer.buffer->pool.Resolve(attr.name - 1) ==
               "Exceptions") {
      Nattr.name = &this->buffer.klassBuffer->pool.data.at(attr.name - 1);
      Nattr.type = SmartAttributesType::NTExceptions;
      Nattr.length = attr.length;
      for (auto &&exc : *attr.exceptions.tableExceptions) {
        Nattr.exceptions.exceptions.push_back(
            this->buffer.klassBuffer->pool.data.at(exc));
      }
      Object->attributes.push_back(Nattr);
    } else if (this->buffer.buffer->pool.Resolve(attr.name - 1) ==
               "LineNumberTable") {
      Nattr.name = &this->buffer.klassBuffer->pool.data.at(attr.name - 1);
      Nattr.type = SmartAttributesType::NTLineNumberTable;
      Nattr.length = attr.length;
      for (auto &&exc : *attr.lineNumberTable.table) {
        Nattr.liTable.table.push_back(NLineNumber{exc.startPc, exc.lineNumber});
      }
      Object->attributes.push_back(Nattr);
    } else if (this->buffer.buffer->pool.Resolve(attr.name - 1) ==
               "LocalVariableTable") {
      Nattr.name = &this->buffer.klassBuffer->pool.data.at(attr.name - 1);
      Nattr.type = SmartAttributesType::NTLocalVariableTable;
      Nattr.length = attr.length;
      for (auto &&exc : *attr.localVariableTable.table) {
        Nattr.lTable.table.push_back(NLocalVariable{
            exc.start_pc, exc.length,
            &this->buffer.klassBuffer->pool.data.at(exc.name_index),
            &this->buffer.klassBuffer->pool.data.at(exc.signature_index),
            exc.slot});
      }
      Object->attributes.push_back(Nattr);
    } else if (this->buffer.buffer->pool.Resolve(attr.name - 1) == "Code") {
      Nattr.name = &this->buffer.klassBuffer->pool.data.at(attr.name - 1);
      Nattr.type = SmartAttributesType::NTCode;
      Nattr.length = attr.length;
      Nattr.code.maxStack = attr.code.maxStack;
      Nattr.code.maxLocals = attr.code.maxLocals;
      Nattr.code.internalCode = attr.code.internalCode;
      Nattr.code.tables = attr.code.tables;
      this->buffer.attrBuffer = &attr.code.attr;
      Resolve(&Nattr.code.attr);
    }
  }
  return nullptr;
}

Linker *Linker::Resolve(Memory *Object) {
  this->buffer = Object;
  return nullptr;
}

Linker *Linker::Resolve(DataPool *Object) {
  for (auto &&poolElement : Object->data) {
    switch (poolElement.tag) {
    case ClassT:
      for (auto &&klazz : this->buffer->info.classes) {
        if (klazz->mainInfo.Class.className == poolElement.Class.className) {
          poolElement.Class.link = klazz;
        }
      }
      break;
    case MethodT:
      for (auto &&klazz : this->buffer->info.classes) {
        if (klazz->mainInfo.Class.className ==
            poolElement.Method.data.Class.className) {
          poolElement.Method.data.Class.link = klazz;
          for (auto &&method : klazz->methods.methods) {
            if (method.name == poolElement.Method.data.info.name) {
              poolElement.Method.Method = &method;
            }
          }
        }
      }
      break;
    case FieldT:
      for (auto &&klazz : this->buffer->info.classes) {
        if (klazz->mainInfo.Class.className ==
            poolElement.Field.data.Class.className) {
          poolElement.Field.data.Class.link = klazz;
          for (auto &&field : klazz->fields.fields) {
            if (field.name == poolElement.Field.data.info.name) {
              poolElement.Field.Field = &field;
            }
          }
        }
      }
      break;
    case DoubleT:
      break;
    case StringT:
      break;
    case LongT:
      break;
    case FloatT:
      break;
    case IntegerT:
      break;
    case UnknownT:
      break;
    }
  }
  return nullptr;
}

Linker * Linker::Resolve(Class *Object) {
  Resolve(&Object->mainInfo.Class);
  Resolve(&Object->mainInfo.superClass);
  Resolve(&Object->pool);
  return nullptr;
}

Linker *Linker::Resolve(Classes *Object) {
  for (auto &&klazz : Object->classes) {
    Resolve(klazz);
  }
  return nullptr;
}

Linker *Linker::Resolve(ClassLink *Object) {
  for (auto &&klazz : this->buffer->info.classes) {
    if (klazz->mainInfo.Class.className == Object->className) {
      Object->link = klazz;
    }
  }
  return nullptr;
}
