// Listing 11-1. Demonstrating the Difference Between Prefix and Postfix Increment
#include <iostream>

int main()
{
  int x{42};

  std::cout << "x   = " << x   << "\n";
  std::cout << "++x = " << ++x << "\n";
  std::cout << "x   = " << x   << "\n";
  std::cout << "x++ = " << x++ << "\n";
  std::cout << "x   = " << x   << "\n";
}
