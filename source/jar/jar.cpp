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
  if (this->loader.buffer.Resolve(u4()) != LFHsignature) { // убрать сдвиг с byteBuffer!
    this->loader.buffer.byteBufferCounter -= 4;
    return nullptr;
  } else {
    LocalFileHeader hd = LocalFileHeader();
    hd.versionToExtract = this->loader.buffer.Resolve(u2());
    hd.gpFlag = this->loader.buffer.Resolve(u2());
    hd.compressionMethod = this->loader.buffer.Resolve(u2());
    if (hd.compressionMethod != 0) {
      throw "[JARLOADER] JAR IS COMPRESSED!";
      abort();
    }
    hd.modificationTime = this->loader.buffer.Resolve(u2());
    hd.modificationDate = this->loader.buffer.Resolve(u2());
    hd.crc32 = this->loader.buffer.Resolve(u4());
    hd.compressedSize = this->loader.buffer.Resolve(u4());
    if (hd.compressedSize != 0) {
      throw "[JARLOADER] JAR IS COMPRESSED!";
      abort();
    }
    hd.uncompressedSize = this->loader.buffer.Resolve(u4());
    hd.fileNameLength = this->loader.buffer.Resolve(u2());
    hd.extraFieldLength = this->loader.buffer.Resolve(u2());
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
    hd.versionMadeBy = this->loader.buffer.Resolve(u2());
    hd.versionToExtract = this->loader.buffer.Resolve(u2());
    hd.gpFlag = this->loader.buffer.Resolve(u2());
    hd.compressionMethod = this->loader.buffer.Resolve(u2());
    if (hd.compressionMethod != 0) {
      throw "[JARLOADER] JAR IS COMPRESSED!";
      abort();
    }
    hd.modificationTime = this->loader.buffer.Resolve(u2());
    hd.modificationDate = this->loader.buffer.Resolve(u2());
    hd.crc32 = this->loader.buffer.Resolve(u4());
    hd.compressedSize = this->loader.buffer.Resolve(u4());
    if (hd.compressedSize != 0) {
      throw "[JARLOADER] JAR IS COMPRESSED!";
      abort();
    }
    hd.uncompressedSize = this->loader.buffer.Resolve(u4());
    hd.fileNameLength = this->loader.buffer.Resolve(u2());
    hd.extraFieldLength = this->loader.buffer.Resolve(u2());
    hd.fileCommentLength = this->loader.buffer.Resolve(u2());
    hd.diskNumber = this->loader.buffer.Resolve(u2());
    hd.internalFileAttributes = this->loader.buffer.Resolve(u4());
    hd.externalFileAttributes = this->loader.buffer.Resolve(u4());
    hd.localFileHeaderOffset = this->loader.buffer.Resolve(u4());
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
  Object->signature = this->loader.buffer.Resolve(u4());
  Object->diskNumber = this->loader.buffer.Resolve(u2());
  Object->startDiskNumber = this->loader.buffer.Resolve(u2());
  Object->numberCentralDirectoryRecord = this->loader.buffer.Resolve(u2());
  Object->totalCentralDirectoryRecord = this->loader.buffer.Resolve(u2());
  Object->sizeOfCentralDirectory = this->loader.buffer.Resolve(u4());
  Object->centralDirectoryOffset = this->loader.buffer.Resolve(u4());
  Object->commentLength = this->loader.buffer.Resolve(u2());
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
