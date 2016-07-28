// Listing 56-17. Calling to_string
#include <iostream>
#include "to_string.hpp"

int main()
{
  std::cout << to_string<std::string>(42, std::ios_base::hex) << '\n';
  std::cout << to_string<std::string>(42.0, std::ios_base::scientific, 10) << '\n';
  std::cout << to_string<std::string>(true, std::ios_base::boolalpha) << '\n';
}
