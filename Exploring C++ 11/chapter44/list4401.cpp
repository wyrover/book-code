// Listing 44-1. Comparing Iterators by Using the < Operator
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main()
{
  std::vector<int> data{};
  int x{};
  while (std::cin >> x)
    data.push_back(x);

  for (auto start(data.begin()), end(data.end()); start < end; ++start)
  {
    --end; // now end points to a real position, possibly start
    std::iter_swap(start, end); // swap contents of two iterators
  }

  std::copy(data.begin(), data.end(), std::ostream_iterator<int>(std::cout, "\n"));
}
