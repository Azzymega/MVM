#pragma once
#include "definitions.hpp"

struct constantClass
{
    u2 nameIndex;
};

struct constantDouble
{
    u4 highBytes;
    u4 lowBytes;
};

struct constantFieldRef
{
    u2 classIndex;
    u2 nameAndTypeIndex;
};

struct constantMethodRef
{
    u2 classIndex;
    u2 nameAndTypeIndex;
};

struct constantString
{
    u2 stringIndex;
};

struct constantNameAndType
{
    u2 nameIndex;
    u2 descriptorIndex;
};

struct constantMethodType // ???
{
    u2 descriptorIndex;
};

struct constantFloat
{
    u4 bytes;
};

struct constantInteger
{
    u4 bytes;
};

struct constantInterfaceMethRef
{
    u2 classIndex;
    u2 nameAndTypeIndex;
};

struct constantLong
{
    u4 highBytes;
    u4 lowBytes;
};

struct constantUTF8
{
    u2 length;
    std::string* utf8;
};

struct constantPoolType
{
    u1 tag;
    union
    {
        constantClass constantClass;
        constantDouble constantDouble;
        constantFieldRef constantFieldRef;
        constantFloat constantFloat;
        constantInteger constantInteger;
        constantInterfaceMethRef constantInterfaceMethRef;
        constantLong constantLong;
        constantMethodRef constantMethodRef;
        constantMethodType constantMethodType;
        constantNameAndType constantNameAndType;
        constantString constantString;
        constantUTF8 constantUTF8;
    };
};