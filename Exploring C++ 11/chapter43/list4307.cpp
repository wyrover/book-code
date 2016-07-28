// Listing 43-7. Searching for an Integer Using Binary Search
#include <algorithm>
#include <iostream>

#include "data.hpp"

int main()
{
  intvector data{};
  read_data(data);
  std::sort(data.begin(), data.end());
  write_data(data);
  intvec_iterator iter{std::lower_bound(data.begin(), data.end(), 42)};
  if (iter == data.end())
    std::cout << "Value 42 not found\n";
  else
  {
    *iter = 0;
    std::cout << "Value 42 changed to 0:\n";
    write_data(data);
  }
}
