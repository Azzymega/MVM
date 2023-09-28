#include "jar.hpp"
#include <cstdlib>

loaderComponent *loaderComponent::Resolve(std::string *Object) {
  this->loadingStream.open(*Object);
  if (!loadingStream.is_open() && *Object != "null") {
    throw "[LOADER] FAIL LOADING FAILS!!!";
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
  if (this->loader.buffer.Resolve(u4()) !=
      LFHsignature) { // убрать сдвиг с byteBuffer!
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
    for (int i = 0; i < hd.uncompressedSize; i++) {
      dt.archiveData.push_back(this->loader.buffer.Resolve(u1()));
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
  Object->mainClassName =
      std::string(this->buffer->locals.locals[0].data.archiveData.begin(),
                  this->buffer->locals.locals[0].data.archiveData.end());
  byteBuffer *buffer = new byteBuffer();
  for (int i = 1; i < this->buffer->locals.locals.size(); i++) {
    buffer->byteBufferCounter = 0;
    buffer->buffer = this->buffer->locals.locals[i].data.archiveData;
    Object->files.classFiles.emplace_back(buffer);
  }
  return nullptr;
}

void Jar::Interact(JarLoader *Object) { Object->Resolve(this); }

void JavaArhive::Interact(JarLoader *Object) { Object->Resolve(this); }
