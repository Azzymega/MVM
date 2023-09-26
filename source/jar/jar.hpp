#pragma once
#include "../definitions.hpp"
#include "../loader.hpp"

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
  u4 signature = 0x04034b50;
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
  u4 signature = 0x02014b50;
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
  u4 signature = 0x06054b50;
  u2 diskNumber;
  u2 startDiskNumber;
  u2 numberCentralDirectoryRecord;
  u2 totalCentralDirectoryRecord;
  u4 sizeOfCentralDirectory;
  u4 centralDirectoryOffset;
  u2 commentLength;
  CommentData commentData;
};

struct JavaArhive {
  std::vector<byteBuffer> classFiles;
};