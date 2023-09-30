#include "loader.hpp"
#include <vector>

void loader::Interact(byteBuffer *Object) {
  Object->Resolve(&this->readStream);
}

loader *loader::Resolve(classFile *Object) {
  this->file = Object;
  Resolve(&Object->data.diskLocation);
  Interact(&this->buffer);
  Resolve(&Object->data);
  Resolve(&Object->pool);
  Object->data.accessFlags = buffer.Resolve(u2());
  Object->data.thisClass = buffer.Resolve(u2());
  Object->data.superClass = buffer.Resolve(u2());
  Resolve(&Object->intrf);
  Resolve(&Object->flds);
  Resolve(&Object->mthds);
  Resolve(&Object->attr);
  return nullptr;
}



loader *loader::Resolve(attributes *Object) {
  Object->attributes_count = buffer.Resolve(u2());
  Object->attr = new std::vector<attribute>(Object->attributes_count);
  for (auto &&attr : *Object->attr) {
    attr.name = buffer.Resolve(u2());
    attr.length = buffer.Resolve(u4());
    if (cpoolget(attr.name) == "SourceFile") {
      attr.sourceFile.index = buffer.Resolve(u2());
    } else if (cpoolget(attr.name) == "ConstantValue") {
      attr.constantValue.index = buffer.Resolve(u2());
    } else if (cpoolget(attr.name) == "Exceptions") {
      Resolve(&attr.exceptions);
    } else if (cpoolget(attr.name) == "LineNumberTable") {
      Resolve(&attr.lineNumberTable);
    } else if (cpoolget(attr.name) == "LocalVariableTable") {
      Resolve(&attr.localVariableTable);
    } else if (cpoolget(attr.name) == "Code") {
      attr.code.maxStack = buffer.Resolve(u2());
      attr.code.maxLocals = buffer.Resolve(u2());
      Resolve(&attr.code.internalCode);
      Resolve(&attr.code.tables);
      Resolve(&attr.code.attr);
    }
  }
  return nullptr;
}

loader *loader::Resolve(fields *Object) {
  Object->fieldsCount = buffer.Resolve(u2());
  Object->elements = std::vector<field>(Object->fieldsCount);
  for (auto &&field : Object->elements) {
    field.access_flags = buffer.Resolve(u2());
    field.name_index = buffer.Resolve(u2());
    field.signature_index = buffer.Resolve(u2());
    Resolve(&field.attributes);
  }
  return nullptr;
}

loader *loader::Resolve(interfaces *Object) {
  Object->interfacesCount = buffer.Resolve(u2());
  Object->interfaces = std::vector<u2>(Object->interfacesCount);
  for (auto &&interface : Object->interfaces) {
    interface = buffer.Resolve(u2());
  }
  return nullptr;
}

loader *loader::Resolve(methods *Object) {
  Object->methodsCount = buffer.Resolve(u2());
  Object->elements = std::vector<method>(Object->methodsCount);
  for (auto &&method : Object->elements) {
    method.access_flags = buffer.Resolve(u2());
    method.name_index = buffer.Resolve(u2());
    method.signature_index = buffer.Resolve(u2());
    Resolve(&method.attributes);
  }
  return nullptr;
}

loader *loader::Resolve(constantPool *Object) {
  Object->constantPoolCount = buffer.Resolve(u2());
  Object->elements = std::vector<constantPoolType>(Object->constantPoolCount);
  for (auto &&cpItem : Object->elements) {
    cpItem.tag = buffer.Resolve(u1());
    switch (cpItem.tag) {
    case cpType::CONSTANT_Class:
      cpItem.constantClass.nameIndex = buffer.Resolve(u2());
      break;
    case cpType::CONSTANT_Double:
      cpItem.constantDouble.highBytes = buffer.Resolve(u4());
      cpItem.constantDouble.lowBytes = buffer.Resolve(u4());
      break;
    case cpType::CONSTANT_Fieldref:
      cpItem.constantFieldRef.classIndex = buffer.Resolve(u2());
      cpItem.constantFieldRef.nameAndTypeIndex = buffer.Resolve(u2());
      break;
    case cpType::CONSTANT_Float:
      cpItem.constantFloat.bytes = buffer.Resolve(u4());
      break;
    case cpType::CONSTANT_Integer:
      cpItem.constantInteger.bytes = buffer.Resolve(u4());
      break;
    case cpType::CONSTANT_InterfaceMethodref:
      cpItem.constantInterfaceMethRef.classIndex = buffer.Resolve(u2());
      cpItem.constantInterfaceMethRef.nameAndTypeIndex = buffer.Resolve(u2());
      break;
    case cpType::CONSTANT_Long:
      cpItem.constantLong.highBytes = buffer.Resolve(u4());
      cpItem.constantLong.lowBytes = buffer.Resolve(u4());
      break;
    case cpType::CONSTANT_Methodref:
      cpItem.constantMethodRef.classIndex = buffer.Resolve(u2());
      cpItem.constantMethodRef.nameAndTypeIndex = buffer.Resolve(u2());
      break;
    case cpType::CONSTANT_NameAndType:
      cpItem.constantNameAndType.nameIndex = buffer.Resolve(u2());
      cpItem.constantNameAndType.descriptorIndex = buffer.Resolve(u2());
      break;
    case cpType::CONSTANT_String:
      cpItem.constantString.stringIndex = buffer.Resolve(u2());
      break;
    case cpType::CONSTANT_Utf8:
      cpItem.constantUTF8.length = buffer.Resolve(u2());
      cpItem.constantUTF8.utf8 = new std::string();
      for (size_t i = 0; i < cpItem.constantUTF8.length; i++) {
        cpItem.constantUTF8.utf8->push_back(buffer.Resolve(u1()));
      }
      break;
    default:
      this->buffer.byteBufferCounter--;
      break;
    }
  }
  return nullptr;
}

loader *loader::Resolve(classFileMetadata *Object) {
  Object->magic = buffer.Resolve(u4());
  Object->minVersion = buffer.Resolve(u2());
  Object->maxVersion = buffer.Resolve(u2());
  return nullptr;
}

loader *loader::Resolve(std::string *Object) {
  readStream.open(*Object);
  if (!readStream.is_open() && *Object!="null") {
    throw "FAIL";
  }
  return nullptr;
}

loader *loader::Resolve(exceptions *Object) {
  Object->exceptionCount = buffer.Resolve(u2());
  Object->tableExceptions = new std::vector<u2>(Object->exceptionCount);
  for (auto &&data : *Object->tableExceptions) {
    data = buffer.Resolve(u2());
  }
  return nullptr;
}

loader *loader::Resolve(localVariableTable *Object) {
  Object->length = buffer.Resolve(u2());
  Object->table = new std::vector<localVariable>(Object->length);
  for (auto &&local : *Object->table) {
    local.start_pc = buffer.Resolve(u2());
    local.length = buffer.Resolve(u2());
    local.name_index = buffer.Resolve(u2());
    local.signature_index = buffer.Resolve(u2());
    local.slot = buffer.Resolve(u2());
  }
  return nullptr;
}

loader *loader::Resolve(exceptionsTables *Object) {
  Object->exceptionTableLength = buffer.Resolve(u2());
  Object->table = new std::vector<exceptionTable>(Object->exceptionTableLength);
  for (auto &&exception : *Object->table) {
    exception.startPc = buffer.Resolve(u2());
    exception.endPc = buffer.Resolve(u2());
    exception.handlerPc = buffer.Resolve(u2());
    exception.catchType = buffer.Resolve(u2());
  }
  return nullptr;
}

loader *loader::Resolve(lineNumberTable *Object) {
  Object->length = buffer.Resolve(u2());
  Object->table = new std::vector<lineNumber>(Object->length);
  for (auto &&line : *Object->table) {
    line.startPc = buffer.Resolve(u2());
    line.lineNumber = buffer.Resolve(u2());
  }
  return nullptr;
}

loader *loader::Resolve(bytecode *Object) {
  Object->length = buffer.Resolve(u4());
  Object->instructions = new std::vector<u1>(Object->length);
  for (auto &&line : *Object->instructions) {
    line = buffer.Resolve(u1());
  }
  return nullptr;
}


classFile *loader::Resolve(byteBuffer *Object) {
  classFile *fl = new classFile();
  this->file = fl;
  this->buffer = *Object;
  Resolve(&fl->data);
  Resolve(&fl->pool);
  fl->data.accessFlags = buffer.Resolve(u2());
  fl->data.thisClass = buffer.Resolve(u2());
  fl->data.superClass = buffer.Resolve(u2());
  Resolve(&fl->intrf);
  Resolve(&fl->flds);
  Resolve(&fl->mthds);
  Resolve(&fl->attr);
  return fl;
}

std::string loader::cpoolget(u2 data) {
  return *this->file->pool.elements[data-1].constantUTF8.utf8;
}

byteBuffer *byteBuffer::Resolve(std::ifstream *Object) {
  byteBufferCounter = 0;
  while (!Object->eof()) {
    uint8_t data;
    *Object >> data >> std::noskipws;
    buffer.push_back(data);
  }
  return nullptr;
}

unsigned char byteBuffer::Resolve(unsigned char Object) {
  Object = 1;
  u1 temp[1];
  temp[0] = buffer[byteBufferCounter];
  byteBufferCounter += sizeof(u1);
  return temp[0];
}

unsigned int byteBuffer::Resolve(unsigned int Object) {
  Object = 2;
  u2 temp[4];
  temp[0] = buffer[byteBufferCounter];
  temp[1] = buffer[byteBufferCounter + 1];
  temp[2] = buffer[byteBufferCounter + 2];
  temp[3] = buffer[byteBufferCounter + 3];
  byteBufferCounter += sizeof(u4);
  return temp[0] >> 24 | temp[1] >> 16 | temp[2] >> 8 | temp[3];
}

unsigned short byteBuffer::Resolve(unsigned short Object) {
  Object = 4;
  u2 temp[2];
  temp[0] = buffer[byteBufferCounter];
  temp[1] = buffer[byteBufferCounter + 1];
  byteBufferCounter += sizeof(u2);
  return temp[0] >> 8 | temp[1];
}

void byteBuffer::Resolve(unsigned char *Object) {
  u1 temp[1];
  temp[0] = buffer[byteBufferCounter];
  byteBufferCounter += sizeof(u1);
  *Object = temp[0];
}

void byteBuffer::Resolve(unsigned int *Object) {
  u1 temp[4];
  temp[0] = buffer[byteBufferCounter];
  temp[1] = buffer[byteBufferCounter + 1];
  temp[2] = buffer[byteBufferCounter + 2];
  temp[3] = buffer[byteBufferCounter + 3];
  byteBufferCounter += sizeof(u4);
  *Object = temp[0] | temp[1] | temp[2] | temp[3];
}

void byteBuffer::Resolve(unsigned short *Object) {
  u1 temp[2];
  temp[0] = buffer[byteBufferCounter];
  temp[1] = buffer[byteBufferCounter + 1];
  byteBufferCounter += sizeof(u2);
  *Object = temp[0] | temp[1];
}
