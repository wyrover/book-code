// Listing 45-2. A Bad Way to Access an Element of a Vector
#include <iostream>
#include <vector>

int main()
{
  std::vector<int> data{ 10, 20 };
  data[5] = 0;
  std::cout << data[5] << '\n';
}
