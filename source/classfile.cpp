/*
 *
 *  * PROJECT:     MVM
 *  * LICENSE:     GPL - See COPYING in the top level directory
 *  * PROGRAMMER:  Maltsev Daniil <brickexberiment@lenta.ru>
 * 
 */

#include "classfile.hpp"
#include "loader.hpp"

void classFile::Interact(loader *Object)
{
    Object->Resolve(this);
}

void attributes::Interact(loader *Object) 
{
    Object->Resolve(this);
}

void constantPool::Interact(loader *Object) 
{
    Object->Resolve(this);
}

std::string constantPool::Resolve(unsigned short Object) {
    return *this->elements.at(Object).constantUTF8.utf8;
}

void interfaces::Interact(loader *Object) 
{
    Object->Resolve(this);
}

void fields::Interact(loader *Object) 
{
    Object->Resolve(this);
}

void methods::Interact(loader *Object) 
{
    Object->Resolve(this);
}

void classFileMetadata::Interact(loader *Object) 
{
    Object->Resolve(this);
}
