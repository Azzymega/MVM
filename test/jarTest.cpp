#include "../source/jar/jar.hpp"
#include "../source/loader.hpp"
#include "../source/memory.hpp"
#include <iostream>
#include <ostream>
#include <vector>

// JAR TEST
int main() {
  std::cout << "JAR TEST STARTED!" << std::endl << std::endl;
  loader ld = loader();
  Jar jarFile;
  std::vector<classFile> files;
  jarFile.diskLocation = "test.jar";
  JarLoader jld;
  jarFile.Interact(&jld);
  JavaArhive arch;
  arch.Interact(&jld);
  if (arch.mainClassName == "test.class") {
    std::cout << "Jar loading and init test success" << std::endl;
  } else {
    std::cout << "JAR LOADING AND INIT TEST FAILED!!!" << std::endl;
  }
  return 0;
}