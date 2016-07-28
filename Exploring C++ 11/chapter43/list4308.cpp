// Listing 43-8. Exploring the lower_bound and upper_bound Functions
#include <algorithm>
#include <iostream>
#include <vector>

#include "data.hpp"

int main()
{
  intvector data{};
  read_data(data);
  std::sort(data.begin(), data.end());
  write_data(data);

  std::cin.clear();
  int test{};
  while (std::cin >> test)
  {
    intvec_iterator lb{std::lower_bound(data.begin(), data.end(), test)};
    intvec_iterator ub{std::upper_bound(data.begin(), data.end(), test)};
    std::cout << "lower bound = " << std::distance(data.begin(), lb) << '\n' <<
                 "upper bound = " << std::distance(data.begin(), ub) << '\n';
  }
}
