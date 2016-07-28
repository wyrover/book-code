// Listing 45-1. Accessing an Element of a Vector
#include <iostream>
#include <vector>

int main()
{
  std::vector<int> data{ 10, 20 };
  data.at(5) = 0;
  std::cout << data.at(5) << '\n';
}
