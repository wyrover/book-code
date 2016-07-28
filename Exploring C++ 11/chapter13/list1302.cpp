// Listing 13-2. Local Variable Definitions
#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

int main()
{
  std::vector<int> data{};
  data.insert(data.begin(), std::istream_iterator<int>(std::cin),
                            std::istream_iterator<int>());

  // Silly way to sort a vector. Assume that the initial portion
  // of the vector has already been sorted, up to the iterator iter.
  // Find where *iter belongs in the already sorted portion of the vector.
  // Erase *iter from the vector and re-insert it at its sorted position.
  // Use binary search to speed up the search for the proper position.
  // Invariant: elements in range [begin(), iter are already sorted.
  for (std::vector<int>::iterator iter{data.begin()}, end{data.end()}; iter != end; )
  {
    // Find where *iter belongs by calling the standard algorithm
    // lower_bound, which performs a binary search and returns an iterator
    // that points into data at a position where the value should be inserted.
    int value{*iter};
    std::vector<int>::iterator here{std::lower_bound(data.begin(), iter, value)};
    if (iter == here)
      ++iter; // already in sorted position
    else
    {

      // erase the out-of-position item, advancing iter at the same time.

      iter = data.erase(iter);
      // re-insert the value at the correct position.

      data.insert(here, value);

    }
  }

  // Debugging code: check that the vector is actually sorted. Do this by comparing
  // each element with the preceding element in the vector. 
  for (std::vector<int>::iterator iter{data.begin()}, prev{data.end()}, end{data.end()};
       iter != end;
      ++iter)
  {
    if (prev != data.end())
      assert(not (*iter < *prev));
     prev = iter;
  }

  // Print the sorted vector all on one line. Start the line with "{" and
  // end it with "}". Separate elements with commas. 
  // An empty vector prints as "{ }".
  std::cout << '{';
  std::string separator{" "};
  for (int element : data)
  {
    std::cout << separator << element;
    separator = ", ";
  }
  std::cout << " }\n";
}
