// Listing 11-3. Reversing the Input Order
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

  for (std::vector<int>::iterator start{data.begin()}, end{data.end()};
       start != end;
       /*empty*/)
  {
    --end;
    if (start != end)
    {
      std::iter_swap(start, end);
      ++start;
    }
  }

  std::copy(data.begin(), data.end(), std::ostream_iterator<int>(std::cout, "\n"));
}
