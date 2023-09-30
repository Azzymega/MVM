#include "../source/loader.hpp"
#include "../source/memory.hpp"
#include <iostream>
// TESTS
int main()
{
    std::cout << "TEST STARTED!" << std::endl << std::endl;
    loader ld = loader();
    classFile file;
    file.data.diskLocation = "test.class";
    ld.Resolve(&file);
    if (file.attr.attr->at(0).sourceFile.index == 22)
    {
        std::cout << "Class file parser test passed" << std::endl;
    }
    else
    {
        std::cout << "CLASS FILE PARSER TEST FAILED!!!" << std::endl;
    }
    ld = loader();
    file = classFile();
    file.data.diskLocation = "test2.class";
    ld.Resolve(&file);
    if (file.attr.attr->at(0).sourceFile.index == 16)
    {
        std::cout << "Class file parser test passed" << std::endl;
    }
    else
    {
        std::cout << "CLASS FILE PARSER TEST FAILED!!!" << std::endl;
    }
    Memory mem;
    mem.Resolve(&file);
    if (mem.info.classes.at(0)->attr.attributes.at(0).sourceFile.name->String.info == "System.Machine.java") {
        std::cout << "Class file digitalization test passed" << std::endl;
    }
    else {
        std::cout << "CLASS FILE DIGITALIZATION TEST FAILED!!!" << std::endl;
    }
    return 0;
}