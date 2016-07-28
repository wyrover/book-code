// Listing 10-3. Sorting Integers by Using Only Generic Algorithms and Iterator Adapters
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main()
{
  std::vector<int> data{};

  // Read integers one at a time.
  std::copy(std::istream_iterator<int>(std::cin),
            std::istream_iterator<int>(),
            std::back_inserter(data));

  // Sort the vector.
  std::sort(data.begin(), data.end());

  // Print the vector, one number per line.
  std::copy(data.begin(), data.end(),
            std::ostream_iterator<int>(std::cout, "\n"));
}
