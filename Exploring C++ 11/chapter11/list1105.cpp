// Listing 11-5. Taking Advantage of the Standard Library
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main()
{
  std::vector<int> data{};

  // Read integers from the standard input, and append them to the end of data.
  data.insert(data.end(),
              std::istream_iterator<int>(std::cin), std::istream_iterator<int>());

  // Reverse the order of the contents of data.
  std::reverse(data.begin(), data.end());
 
 // Print data, one number per line.
  std::copy(data.begin(), data.end(), std::ostream_iterator<int>(std::cout, "\n"));
}
