// Listing 64-10. Mystery Program
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

template<class T>
void append(std::vector<T>& data, const T& value, int max_size)
{
  if (data.size() < max_size - 1)
    data.push_back(value);
}

int main()
{
  std::vector<int> data{};
  append(data, 10, 3);
  append(data, 20, 2);
  append(data, 30, 1);
  append(data, 40, 0);
  append(data, 50, 0);
  std::copy(data.begin(), data.end(),
               std::ostream_iterator<int>(std::cout, " "));
  std::cout << '\n';
}
