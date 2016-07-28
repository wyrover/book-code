#include <string>
#include <iostream>
#include "HelloWorld.h"

using namespace std;

extern "C" HelloWorld* create() {
  return new HelloWorld;
}

extern "C" void destroy(HelloWorld *hw) {
  delete hw;
}
