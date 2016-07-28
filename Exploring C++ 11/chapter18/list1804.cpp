// Listing18-4. Creating and Sharing a Single Locale Object
#include <iostream>
#include <locale>
#include <map>
#include <string>

int main()
{
  typedef std::map<std::string, int>    count_map;

  std::locale native{""};             // get the native locale
  std::cin.imbue(native);             // interpret the input and output according to
  std::cout.imbue(native);            // the native locale

  count_map counts{};

  // Read words from the standard input and count the number of times
  // each word occurs.
  std::string word{};
  while (std::cin >> word)
  {
    // Make a copy of word, keeping only alphabetic characters.
    std::string copy{};
    for (char ch : word)
      if (std::isalnum(ch, native))
        copy.push_back(ch);
    // The "word" might be all punctuation, so the copy would be empty.
    // Don't count empty strings.
    if (not copy.empty())
      ++counts[copy];
  }

  // For each word/count pair, print the word & count on one line.
  for (auto pair : counts)
    std::cout << pair.first << '\t' << pair.second << '\n';
}
