// Listing 56-13. Demonstrating the Use of to_string
#include <iostream>
#include "to_string.hpp"
#include "from_string.hpp"

int main()
{
    std::string str{
      to_string<int, char, std::char_traits<char>, std::allocator<char>>(42)
    };
    int value{from_string<int>(str)};
}
