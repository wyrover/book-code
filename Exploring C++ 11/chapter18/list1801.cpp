// Listing 18-1. Counting Words: Restricting Words to Letters and Letter-Like Characters
#include <iomanip>
#include <iostream>
#include <map>
#include <string>

int main()
{
  typedef std::map<std::string, int>    count_map;
  typedef std::string::size_type        str_size;

  count_map counts{};

  // Read words from the standard input and count the number of times
  // each word occurs.
  std::string okay{"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                   "abcdefghijklmnopqrstuvwxyz"
                   "0123456789-_"};
  std::string word{};
  while (std::cin >> word)
  {
    // Make a copy of word, keeping only the characters that appear in okay.
    std::string copy{};
    for (char ch : word)
      if (okay.find(ch) != std::string::npos)
        copy.push_back(ch);
    // The "word" might be all punctuation, so the copy would be empty.
    // Don't count empty strings.
    if (not copy.empty())
      ++counts[copy];
  }

  // Determine the longest word.
  str_size longest{0};
  for (auto pair : counts)
    if (pair.first.size() > longest)
      longest = pair.first.size();
  
  // For each word/count pair...
  const int count_size{10}; // Number of places for printing the count
  for (auto pair : counts)
    // Print the word, count, newline. Keep the columns neatly aligned.
    std::cout << std::setw(longest)    << std::left << pair.first <<
                 std::setw(count_size) << std::right << pair.second << '\n';
}
