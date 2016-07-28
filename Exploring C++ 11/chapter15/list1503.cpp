// Listing 15-3. Aligning Words and Counts Neatly
#include <iomanip>
#include <iostream>
#include <map>
#include <string>

int main()
{
  std::map<std::string, int> counts{};

  // Read words from the standard input and count the number of times
  // each word occurs.
  std::string word{};
  while (std::cin >> word)
    ++counts[word];

  // Determine the longest word.
  std::string::size_type longest{};
  for (auto element : counts)
    if (element.first.size() > longest)
      longest = element.first.size();
  
  // For each word/count pair...
  const int count_size{10}; // Number of places for printing the count
  for (auto element : counts)
    // Print the word, count, newline. Keep the columns neatly aligned.
    std::cout << std::setw(longest)    << std::left  << element.first <<
            std::setw(count_size) << std::right << element.second << '\n';
}
