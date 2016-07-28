// Listing 11-4. Rewriting the for Loop
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
       start != end and start != --end;
       ++start)
  {
      std::iter_swap(start, end);
  }

  std::copy(data.begin(), data.end(), std::ostream_iterator<int>(std::cout, "\n"));
}
