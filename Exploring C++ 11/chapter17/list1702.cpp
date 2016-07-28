// Listing 17-2. Echoing Input to Output, One Character at a Time
#include <iostream>

int main()
{
  std::cin >> std::noskipws;
  char ch;
  while (std::cin >> ch)
    std::cout << ch;
}
