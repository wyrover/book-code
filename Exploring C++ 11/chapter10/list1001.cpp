// Listing 10-1. Demonstrating the std::copy Function
#include <algorithm>
#include <cassert>

#include <vector>

int main()
{
  std::vector<int> input{ 10, 20, 30 };
  std::vector<int> output{};
  output.resize(input.size());
  std::copy(input.begin(), input.end(), output.begin());
  // Now output has a complete copy of input.
  assert(input == output);
}
