#include "../../source/jar/jar.hpp"
#include "../../source/loader.hpp"
#include "../../source/memory.hpp"
#include <iostream>
#include <ostream>
#include <vector>

// RUNTIME LOAD TEST
int main() {
  std::cout << "RUNTIME LOAD TEST STARTED!" << std::endl << std::endl;
  loader ld = loader();
  Jar jarFile;
  std::vector<classFile *> files;
  jarFile.diskLocation = "Runtime.jar";
  JarLoader jld;
  jarFile.Interact(&jld);
  JavaArhive arch;
  arch.Interact(&jld);
  if (arch.mainClassName == "System.class\r\n") {
    std::cout << "Jar loading and init test passed" << std::endl;
  } else {
    std::cout << "JAR LOADING AND INIT TEST FAILED!!!" << std::endl;
  }
  for (byteBuffer *buf : arch.files.classFiles) {
    files.push_back(ld.Resolve(buf)); // поправить баг!!!
  }
  Memory mem;
  for (auto &&fil : files) {
    mem.Resolve(fil);
  }
  if (mem.info.classes.at(0)
          ->attr.attributes.at(0)
          .sourceFile.name->String.info == "AccessFlag.java") {
    std::cout << "Class file digitalization test passed" << std::endl;
  } else {
    std::cout << "CLASS FILE DIGITALIZATION TEST FAILED!!!" << std::endl;
  }
  if (mem.info.classes.at(0)->mainInfo.Class.link == mem.info.classes.at(0)) {
    std::cout << "Linker test passed" << std::endl;
  } else {
    std::cout << "LINKER TEST FAILED!!!" << std::endl;
  }
  return 0;
}