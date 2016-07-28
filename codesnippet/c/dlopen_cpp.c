#include <iostream>
#include <dlfcn.h>

#include "HelloWorld.h"

using namespace std;

int main(int argc,char *argv[])
{

  cout << "C++ dlopen demo\n" << endl;
  cout << "Opening hello.so..." << endl;
  void* handle = dlopen("./helloworld_cpp.so", RTLD_LAZY);

  if (!handle) {
      cout << "Cannot open library: " << dlerror() << endl;
      return 1;
  }

  cout << "Loading symbol create and destroy..." << endl;

  dlerror();
  create_t create = (create_t) dlsym(handle, "create");
  const char *dlsym_error = dlerror();
  if (dlsym_error) {
    cout << "create : " <<  dlerror() << endl;
    dlclose(handle);
    return 1;
  }
  destroy_t destroy = (destroy_t)dlsym(handle, "destroy");
  dlsym_error = dlerror();
  if (dlsym_error) {
    cout << "destroy : " << dlerror() << endl;
    dlclose(handle);
    return 1;
  }

  cout << "Calling hello..." << endl;

  HelloWorld *hwPtr = (*create)(); 
  hwPtr->sayHello();
  (*destroy)(hwPtr); 

  cout << "Closing library..." << endl;
  dlclose(handle);

  return 0;
}
