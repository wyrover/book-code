// Listing 62-3. Echoing Command-Line Arguments
#include <iostream>

int main(int argc, char* argv[])
{
  char const* separator{""};
  while (--argc != 0)
  {
    std::cout << separator << *++argv;
    separator = " ";
  }
}
