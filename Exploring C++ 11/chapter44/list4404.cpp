// Listing 44-4. Really Printing the Middle Item of a Series of Integers
#include <iostream>

#include "data.hpp"

int main()
{
  intvector data{};
  read_data(data);
  intvector::const_iterator iter{data.begin()};
  std::advance(iter, data.size() / 2); // move to middle of vector
  if (not data.empty())
    std::cout << "middle item = " << *iter << '\n';
}
