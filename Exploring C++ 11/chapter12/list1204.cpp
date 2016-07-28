// Listing 12-4. Using Short-Circuiting to Test for Nonzero Vector Elements
#include <iostream>
#include <iterator>
#include <vector>

int main()
{
  std::vector<int> data{};
  data.insert(data.begin(),
              std::istream_iterator<int>(std::cin),
              std::istream_iterator<int>());

  std::vector<int>::iterator iter{}, end{data.end()};
  for (iter = data.begin(); iter != end and *iter == 0; ++iter)
    /*empty*/;
  if (iter == data.end())
    std::cout << "data contains all zeroes\n";
  else
    std::cout << "data does not contain all zeroes\n";
}
