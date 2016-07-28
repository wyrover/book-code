// Listing 15-2. Printing Word Frequencies
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

  // For each word/count pair...
  for (auto element : counts)
    // Print the word, tab, the count, newline.
    std::cout << element.first << '\t' << element.second << '\n';
}
