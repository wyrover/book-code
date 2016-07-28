// Listing 10-2. Sorting Integers by Using Iterators to Print the Results
#include <algorithm>
#include <iostream>
#include <vector>

int main()
{
  std::vector<int> data{};     // initialized to be empty
  int x{};

  // Read integers one at a time.
  while (std::cin >> x)
    // Store each integer in the vector.
    data.push_back(x);

  // Sort the vector.
  std::sort(data.begin(), data.end());

  // Print the vector, one number per line.
  for (std::vector<int>::iterator i{data.begin()}, end{data.end()}; i != end; ++i)
    std::cout << *i << '\n';
}
