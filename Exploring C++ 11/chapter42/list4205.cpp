// Listing 42-5. The Main Program for Generating Successive Integers
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

#include "sequence.hpp"

int main()
{
  int size{};
  std::cout << "How many integers do you want? ";
  std::cin >> size;
  int first{};
  std::cout << "What is the first integer? ";
  std::cin >> first;
  int step{};
  std::cout << "What is the interval between successive integers? ";
  std::cin >> step;
 
  std::vector<int> data(size);
  // Generate the integers to fill the vector.
  std::generate(data.begin(), data.end(), sequence(first, step));

  // Print the resulting integers, one per line.
  std::copy(data.begin(), data.end(),
            std::ostream_iterator<int>(std::cout, "\n"));
}
