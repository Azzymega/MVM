#pragma once
#include "attributes.hpp"
#include "constantPoolTypes.hpp"
#include "definitions.hpp"
#include <string>


struct loader;

struct classFileMetadata : public ILoadInteraction<loader> {
  std::string diskLocation;
  u4 magic;
  u2 minVersion;
  u2 maxVersion;
  u2 accessFlags;
  u2 thisClass;
  u2 superClass;
  void Interact(loader *Object) override;
};

struct field {
  u2 access_flags;
  u2 name_index;
  u2 signature_index;
  attributes attributes;
};

struct method {
  u2 access_flags;
  u2 name_index;
  u2 signature_index;
  attributes attributes;
};

struct methods : public ILoadInteraction<loader> {
  u2 methodsCount;
  std::vector<method> elements;
  void Interact(loader *Object) override;
};

struct fields : public ILoadInteraction<loader> {
  u2 fieldsCount;
  std::vector<field> elements;
  void Interact(loader *Object) override;
};

struct interfaces : public ILoadInteraction<loader> {
  u2 interfacesCount;
  std::vector<u2> interfaces;
  void Interact(loader *Object) override;
};

struct constantPool : public ILoadInteraction<loader>,
                      ILoadConflict<std::string, u2> {
  u2 constantPoolCount;
  std::vector<constantPoolType> elements;
  void Interact(loader *Object) override;
  std::string Resolve(unsigned short Object) override;
};

struct classFile : public ILoadInteraction<loader> {
  classFileMetadata data;
  constantPool pool;
  interfaces intrf;
  fields flds;
  methods mthds;
  attributes attr;
  void Interact(loader *object);
};