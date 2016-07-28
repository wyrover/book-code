#ifndef _HELLO_WORLD_H_
#define _HELLO_WORLD_H_

#include <iostream>


class HelloWorld {
public :
  void sayHello () {
   std::cout << "hello world!" << std::endl;
  }
};

typedef HelloWorld * (*create_t)();
typedef void (*destroy_t)(HelloWorld *hw);

#endif
