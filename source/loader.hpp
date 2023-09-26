#pragma once
#include "classfile.hpp"
#include "definitions.hpp"
#include <fstream>

struct byteBuffer : public ILoadConflict<byteBuffer *, std::ifstream *>,
                    ILoadConflict<u1, u1>,
                    ILoadConflict<u2, u2>,
                    ILoadConflict<u4, u4> {
  byteBuffer *Resolve(std::ifstream *Object) override;
  unsigned char Resolve(unsigned char Object) override;
  unsigned int Resolve(unsigned int Object) override;
  unsigned short Resolve(unsigned short Object) override;
  std::vector<u1> buffer;
  u4 byteBufferCounter;
};

struct loader : public ILoadConflict<loader *, classFile *>,
                ILoadConflict<loader *, constantPool *>,
                ILoadConflict<loader *, interfaces *>,
                ILoadConflict<loader *, fields *>,
                ILoadConflict<loader *, methods *>,
                ILoadConflict<loader *, attributes *>,
                ILoadConflict<loader *, classFileMetadata *>,
                ILoadConflict<loader *, std::string *>,
                ILoadConflict<loader *, localVariableTable *>,
                ILoadConflict<loader *, exceptions *>,
                ILoadConflict<loader *, exceptionsTables *>,
                ILoadConflict<loader *, lineNumberTable *>,
                ILoadConflict<loader *, bytecode *>,
                ILoadInteraction<byteBuffer> {

  byteBuffer buffer;
  std::ifstream readStream;
  classFile *file; // hackfix
  void Interact(byteBuffer *Object) override;
  loader *Resolve(classFile *Object) override;
  loader *Resolve(attributes *Object) override;
  loader *Resolve(fields *Object) override;
  loader *Resolve(interfaces *Object) override;
  loader *Resolve(methods *Object) override;
  loader *Resolve(constantPool *Object) override;
  loader *Resolve(classFileMetadata *Object) override;
  loader *Resolve(std::string *Object) override;
  loader *Resolve(exceptions *Object) override;
  loader *Resolve(localVariableTable *Object) override;
  loader *Resolve(exceptionsTables *Object) override;
  loader *Resolve(lineNumberTable *Object) override;
  loader *Resolve(bytecode *Object) override;

private: // hackfix
  std::string cpoolget(u2 data);
};