#include "../source/loader.hpp"
#include "../source/memory.hpp"
#include <iostream>
#include "../source/jar/jar.hpp"
#include <vector>
// JAR TEST
int main()
{
    std::cout << "JAR TEST STARTED!" << std::endl << std::endl;
    loader ld = loader();
    Jar jarFile;
    std::vector<classFile> files;
    jarFile.diskLocation = "test.jar";
    JarLoader jld;
    jarFile.Interact(&jld);
    JavaArhive arch;
    arch.Interact(&jld);
    return 0;
}