#include "memory.hpp"

void Memory::Interact(Linker *Object) { Object->Resolve(this); }

Memory *Memory::Resolve(classFile *Object) {
  this->buffer.buffer = Object;
  Class Klass = Class();
  Resolve(&Klass.pool);
  Resolve(&Klass.mainInfo);
  Resolve(&Klass.interfaces);
  Resolve(&Klass.fields);
  Resolve(&Klass.methods);
  Klass.attr = this->buffer.buffer->attr;
  this->info.classes.emplace_back(Klass);
  return nullptr;
}

Memory *Memory::Resolve(ClassHeader *Object) {
  Object->minVersion = this->buffer.buffer->data.minVersion;
  Object->maxVersion = this->buffer.buffer->data.maxVersion;
  Object->flags = Resolve(&this->buffer.buffer->data.accessFlags);
  Object->Class.className = this->buffer.buffer->pool.Resolve(
      this->buffer.buffer->pool.elements[this->buffer.buffer->data.thisClass-1]
          .constantClass.nameIndex-1);
  Object->superClass.className = this->buffer.buffer->pool.Resolve(
      this->buffer.buffer->pool.elements[this->buffer.buffer->data.superClass-1]
          .constantClass.nameIndex-1);
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
    Object->fields.emplace_back(Field{
        Resolve(&fld.access_flags),
        std::string(*this->buffer.buffer->pool.elements.at(fld.name_index - 1)
                         .constantUTF8.utf8),
        std::string(
            *this->buffer.buffer->pool.elements.at(fld.signature_index - 1)
                 .constantUTF8.utf8),
        fld.attributes});
  }
  return nullptr;
}

Memory *Memory::Resolve(Methods *Object) {
  for (method mth : this->buffer.buffer->mthds.elements) {
    Object->methods.emplace_back(Method{
        Resolve(&mth.access_flags),
        std::string(*this->buffer.buffer->pool.elements.at(mth.name_index - 1)
                         .constantUTF8.utf8),
        std::string(
            *this->buffer.buffer->pool.elements.at(mth.signature_index - 1)
                 .constantUTF8.utf8),
        mth.attributes});
  }
  return nullptr;
}

Linker *Linker::Resolve(Memory *Object) { return nullptr; }
