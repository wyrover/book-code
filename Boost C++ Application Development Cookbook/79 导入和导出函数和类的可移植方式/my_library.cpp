#define MY_LIBRARY_LINK_DYNAMIC
#define MY_LIBRARY_COMPILATION

#include <iostream>
#include "my_library.hpp"

int MY_LIBRARY_API foo()
{
    // Implementation
    // ...
    std::cout << ">>>" << std::endl;
    return 0;
}

int bar::meow()const
{
    throw bar_exception();
}