// Listing 7-1. Using a for Loop to Print Ten Non-Negative Numbers
#include <iostream>

int main()
{
  for (int i{0}; i != 10; i = i + 1)
    std::cout << i << '\n';
}
