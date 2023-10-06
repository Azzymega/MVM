/*
 *
 *  * PROJECT:     MVM - Jar/zip library
 *  * LICENSE:     GPL - See COPYING in the top level directory
 *  * PROGRAMMER:  Maltsev Daniil <brickexberiment@lenta.ru>
 * 
 */

#include "jar.hpp"

loaderComponent *loaderComponent::Resolve(std::string *Object) {
  this->loadingStream.open(*Object, std::ios::binary);
  if (!loadingStream.is_open() && *Object != "null") {
    throw "[LOADER] FILE LOADING FAILS!!!";
  }
  return nullptr;
}

void loaderComponent::Interact(byteBuffer *Object) {
  Object->Resolve(&this->loadingStream);
}

void JarLoader::Interact(loaderComponent *Object) {
  Object->Resolve(&this->buffer->diskLocation);
}

JarLoader *JarLoader::Resolve(Jar *Object) {
  this->buffer = Object;
  Interact(&this->loader);
  this->loader.Interact(&this->loader.buffer);
  Resolve(&Object->locals);
  Resolve(&Object->centrals);
  Resolve(&Object->end);
  return nullptr;
}

JarLoader *JarLoader::Resolve(LocalFileHeaders *Object) {
  if (this->loader.buffer.Resolve(u4()) != LFHsignature) {
    this->loader.buffer.byteBufferCounter -= 4;
    return nullptr;
  } else {
    LocalFileHeader hd = LocalFileHeader();
    this->loader.buffer.Resolve(&hd.versionToExtract);
    this->loader.buffer.Resolve(&hd.gpFlag);
    this->loader.buffer.Resolve(&hd.compressionMethod);
    if (hd.compressionMethod != 0) {
      throw "[JARLOADER] JAR IS COMPRESSED!";
      abort();
    }
    this->loader.buffer.Resolve(&hd.modificationTime);
    this->loader.buffer.Resolve(&hd.modificationDate);
    this->loader.buffer.Resolve(&hd.crc32);
    this->loader.buffer.Resolve(&hd.compressedSize);
    this->loader.buffer.Resolve(&hd.uncompressedSize);
    this->loader.buffer.Resolve(&hd.fileNameLength);
    this->loader.buffer.Resolve(&hd.extraFieldLength);
    std::string filename;
    for (int i = 0; i < hd.fileNameLength; i++) {
      filename.push_back(this->loader.buffer.Resolve(u1()));
    }
    ExtraData data;
    for (int i = 0; i < hd.extraFieldLength; i++) {
      data.extraData.push_back(this->loader.buffer.Resolve(u1()));
    }
    Data dt;
    u1 q = 0; // hackfix
    u1 w = 0; // hackfix
    u1 e = 0; // hackfix
    u1 r = 0; // hackfix
    while (!(q == 'P' && w == 'K' && e == 0x3 && r == 0x4) && !(q == 'P' && w == 'K' && e == 0x1 && r == 0x2)) { // hackfix
      this->loader.buffer.Resolve(&q); // hackfix
      this->loader.buffer.Resolve(&w); // hackfix
      this->loader.buffer.Resolve(&e); // hackfix
      this->loader.buffer.Resolve(&r); // hackfix
      loader.buffer.byteBufferCounter -= 4; // hackfix!!!
      if ((q == 'P' && w == 'K' && e == 0x3 && r == 0x4) || (q == 'P' && w == 'K' && e == 0x1 && r == 0x2)) { // hackfix
        break;
      }
      dt.archiveData.push_back(this->loader.buffer.Resolve(
          u1())); // hackfix!!! 7zip can't count file size! дописать
    }
    hd.filename = filename;
    hd.extraData = data;
    hd.data = dt;
    Object->locals.emplace_back(hd);
    Resolve(Object);
  }
  return nullptr;
}

JarLoader *JarLoader::Resolve(CentralDirectoryFileHeaders *Object) {
  if (this->loader.buffer.Resolve(u4()) != CDFHsignature) {
    this->loader.buffer.byteBufferCounter -= 4;
    return nullptr;
  } else {
    CentralDirectoryFileHeader hd;
    this->loader.buffer.Resolve(&hd.versionMadeBy);
    this->loader.buffer.Resolve(&hd.versionToExtract);
    this->loader.buffer.Resolve(&hd.gpFlag);
    this->loader.buffer.Resolve(&hd.compressionMethod);
    if (hd.compressionMethod != 0) {
      throw "[JARLOADER] JAR IS COMPRESSED!";
      abort();
    }
    this->loader.buffer.Resolve(&hd.modificationTime);
    this->loader.buffer.Resolve(&hd.modificationDate);
    this->loader.buffer.Resolve(&hd.crc32);
    this->loader.buffer.Resolve(&hd.compressedSize);
    this->loader.buffer.Resolve(&hd.uncompressedSize);
    this->loader.buffer.Resolve(&hd.fileNameLength);
    this->loader.buffer.Resolve(&hd.extraFieldLength);
    this->loader.buffer.Resolve(&hd.fileCommentLength);
    this->loader.buffer.Resolve(&hd.diskNumber);
    this->loader.buffer.Resolve(&hd.internalFileAttributes);
    this->loader.buffer.Resolve(&hd.externalFileAttributes);
    this->loader.buffer.Resolve(&hd.localFileHeaderOffset);
    std::string filename;
    for (int i = 0; i < hd.fileNameLength; i++) {
      filename.push_back(this->loader.buffer.Resolve(u1()));
    }
    ExtraData data;
    for (int i = 0; i < hd.extraFieldLength; i++) {
      data.extraData.push_back(this->loader.buffer.Resolve(u1()));
    }
    CommentData commentData;
    for (int i = 0; i < hd.fileCommentLength; i++) {
      commentData.commentData.push_back(this->loader.buffer.Resolve(u1()));
    }
    hd.filename = filename;
    hd.extraData = data;
    hd.commentData = commentData;
    Object->centrals.emplace_back(hd);
    Resolve(Object);
  }
  return nullptr;
}

JarLoader *JarLoader::Resolve(EndOfCentralDirectoryRecord *Object) {
  this->loader.buffer.Resolve(&Object->signature);
  this->loader.buffer.Resolve(&Object->diskNumber);
  this->loader.buffer.Resolve(&Object->startDiskNumber);
  this->loader.buffer.Resolve(&Object->numberCentralDirectoryRecord);
  this->loader.buffer.Resolve(&Object->totalCentralDirectoryRecord);
  this->loader.buffer.Resolve(&Object->sizeOfCentralDirectory);
  this->loader.buffer.Resolve(&Object->centralDirectoryOffset);
  this->loader.buffer.Resolve(&Object->commentLength);
  CommentData commentData;
  for (int i = 0; i < Object->commentLength; i++) {
    commentData.commentData.push_back(this->loader.buffer.Resolve(u1()));
  }
  Object->commentData = commentData;
  return nullptr;
}

JarLoader *JarLoader::Resolve(JavaArhive *Object) {
  byteBuffer *buffer;
  for (auto &&buf : this->buffer->locals.locals) {
    if (buf.filename == "MANIFEST.MF") {
      Object->mainClassName =
          std::string(buf.data.archiveData.begin(), buf.data.archiveData.end());
    } else {
      buffer = new byteBuffer();
      buffer->byteBufferCounter = 0;
      buffer->buffer = buf.data.archiveData;
      Object->files.classFiles.emplace_back(buffer);
    }
  }
  return nullptr;
}

void Jar::Interact(JarLoader *Object) { Object->Resolve(this); }

void JavaArhive::Interact(JarLoader *Object) { Object->Resolve(this); }
