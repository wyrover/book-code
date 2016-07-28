// Listing 22-1. Calling transform to Apply a Function to Each Element of an Array
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int times_two(int i)
{
  return i * 2;
}

int main()
{
   std::vector<int> data{};

   std::copy(std::istream_iterator<int>(std::cin),
             std::istream_iterator<int>(),
             std::back_inserter(data));

   std::transform(data.begin(), data.end(), data.begin(), times_two);

   std::copy(data.begin(), data.end(),
             std::ostream_iterator<int>(std::cout, "\n"));
}
