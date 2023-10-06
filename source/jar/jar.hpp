/*
 *
 *  * PROJECT:     MVM - Jar/zip library
 *  * LICENSE:     GPL - See COPYING in the top level directory
 *  * PROGRAMMER:  Maltsev Daniil <brickexberiment@lenta.ru>
 * 
 */

#pragma once
#include "../definitions.hpp"
#include "../loader.hpp"
#include <string>
#define LFHsignature 4
#define CDFHsignature 73
#define EOCDRsiganture 0x504b0506

struct ExtraData {
  std::vector<u1> extraData;
};

struct CommentData {
  std::vector<u1> commentData;
};

struct Data {
  std::vector<u1> archiveData;
};

struct LocalFileHeader {
  u4 signature = 0x504b0304;
  u2 versionToExtract;
  u2 gpFlag;
  u2 compressionMethod; // only 0 avaliable
  u2 modificationTime;
  u2 modificationDate;
  u4 crc32;
  u4 compressedSize;
  u4 uncompressedSize;
  u2 fileNameLength;
  u2 extraFieldLength;
  std::string filename;
  ExtraData extraData;
  Data data;
};

struct CentralDirectoryFileHeader {
  u4 signature = 0x504b0201;
  u2 versionMadeBy;
  u2 versionToExtract;
  u2 gpFlag;
  u2 compressionMethod; // only 0 avaliable
  u2 modificationTime;
  u2 modificationDate;
  u4 crc32;
  u4 compressedSize;
  u4 uncompressedSize;
  u2 fileNameLength;
  u2 extraFieldLength;
  u2 fileCommentLength;
  u2 diskNumber;
  u2 internalFileAttributes;
  u4 externalFileAttributes;
  u4 localFileHeaderOffset;
  std::string filename;
  ExtraData extraData;
  CommentData commentData;
};

struct EndOfCentralDirectoryRecord {
  u4 signature = 0x504b0506;
  u2 diskNumber;
  u2 startDiskNumber;
  u2 numberCentralDirectoryRecord;
  u2 totalCentralDirectoryRecord;
  u4 sizeOfCentralDirectory;
  u4 centralDirectoryOffset;
  u2 commentLength;
  CommentData commentData;
};

struct LocalFileHeaders {
  std::vector<LocalFileHeader> locals;
};

struct CentralDirectoryFileHeaders {
  std::vector<CentralDirectoryFileHeader> centrals;
};

struct JarLoader;

struct Jar : public ILoadInteraction<JarLoader> {
  std::string diskLocation;
  LocalFileHeaders locals;
  CentralDirectoryFileHeaders centrals;
  EndOfCentralDirectoryRecord end;
  void Interact(JarLoader *Object) override;
};

struct BufferedClassFiles {
  std::vector<byteBuffer*> classFiles;
};

struct JavaArhive : ILoadInteraction<JarLoader> {
  std::string mainClassName;
  BufferedClassFiles files;
  void Interact(JarLoader *Object) override;
};

struct loaderComponent : public ILoadConflict<loaderComponent *, std::string *>,
                         ILoadInteraction<byteBuffer> {
  std::ifstream loadingStream;
  byteBuffer buffer;
  loaderComponent *Resolve(std::string *Object) override;
  void Interact(byteBuffer *Object) override;
};

struct JarLoader : public ILoadInteraction<loaderComponent>,
                   ILoadConflict<JarLoader *, Jar *>,
                   ILoadConflict<JarLoader *, LocalFileHeaders *>,
                   ILoadConflict<JarLoader *, CentralDirectoryFileHeaders *>,
                   ILoadConflict<JarLoader *, EndOfCentralDirectoryRecord *>,
                   ILoadConflict<JarLoader *, JavaArhive *> {
  Jar *buffer;
  loaderComponent loader;
  void Interact(loaderComponent *Object) override;
  JarLoader *Resolve(Jar *Object) override;
  JarLoader *Resolve(LocalFileHeaders *Object) override;
  JarLoader *Resolve(CentralDirectoryFileHeaders *Object) override;
  JarLoader *Resolve(EndOfCentralDirectoryRecord *Object) override;
  JarLoader *Resolve(JavaArhive *Object) override;
};