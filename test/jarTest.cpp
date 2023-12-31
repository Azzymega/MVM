/*
 *
 *  * PROJECT:     MVM
 *  * LICENSE:     GPL - See COPYING in the top level directory
 *  * PROGRAMMER:  Maltsev Daniil <brickexberiment@lenta.ru>
 * 
 */

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
  std::vector<classFile *> files;
  jarFile.diskLocation = "test2.jar";
  JarLoader jld;
  jarFile.Interact(&jld);
  JavaArhive arch;
  arch.Interact(&jld);
  if (arch.mainClassName == "test.class") {
    std::cout << "Jar loading and init test passed" << std::endl;
  } else {
    std::cout << "JAR LOADING AND INIT TEST FAILED!!!" << std::endl;
  }
  for (byteBuffer *buf : arch.files.classFiles) {
    files.push_back(ld.Resolve(buf));
  }
  if (files[0]->attr.attr->at(0).sourceFile.index == 16) {
    std::cout << "Class file integrity test passed" << std::endl;
  } else {
    std::cout << "CLASS FILE INTEGRITY TEST FAILED!!!" << std::endl;
  }
  return 0;
}