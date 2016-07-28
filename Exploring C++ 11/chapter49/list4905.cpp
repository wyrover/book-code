// Listing 49-5. Testing rational Comparison Operator
#include <iostream>
#include "rational.hpp"

int main()
{
  rational<int> r{};
  while (std::cin >> r)
    if (r != 0)
      std::cout << r << '\n';
}
