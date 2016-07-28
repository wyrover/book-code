// Listing 15-1. Counting Occurrences of Unique Words
#include <iostream>
#include <map>
#include <string>

int main()
{
  std::map<std::string, int> counts{};
  std::string word{};
  while (std::cin >> word) 
    ++counts[word];
  // TODO: Print the results.
}
