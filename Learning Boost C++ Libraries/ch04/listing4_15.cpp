#include <boost/algorithm/string.hpp>
#include <string>
#include <iostream>
#include <cassert>

int main()
{
    std::string input = "Hello, World! Hello folks!";
    boost::replace_first(input, "Hello", "Hola");
    assert(input == "Hola, World! Hello folks!");
    boost::erase_first(input, "Hello");
    assert(input == "Hola, World!  folks!");
}

