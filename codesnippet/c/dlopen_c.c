#include <dlfcn.h>
#include <stdio.h>

int main() {

    printf("C++ dlopen demo\n\n");
    printf("Opening hello.so...\n");
    void* handle = dlopen("./helloworld.so", RTLD_LAZY);

    if (!handle) {
        printf("Cannot open library: %s", dlerror());
        return 1;
    }

    printf("Loading symbol hello...\n");
    typedef void (*hello_t)();

    dlerror();
    hello_t hello = (hello_t) dlsym(handle, "say_hello");
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
        printf("Cannot load symbol 'hello': %s\n", dlerror());
        dlclose(handle);
        return 1;
    }

    printf("Calling hello...\n");
    
    hello();

    // close the library
    printf("Closing library...\n");
    dlclose(handle);
   
    return 0; 
}
