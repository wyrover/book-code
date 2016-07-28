// Listing 14-1. Copying Integers from a File to Standard Output
#include <cstdio>
#include <fstream>
#include <iostream>

int main()
{
  std::ifstream in{"list1401.txt"};
  if (not in)
    std::perror("list1401.txt");
  else
  {
    int x{};
    while (in >> x)
      std::cout << x << '\n'; 
    in.close();

  }
}
