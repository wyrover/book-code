// Listing 16-1. Counting Words, with a Clean Program That Uses typedef
#include <iomanip>
#include <iostream>
#include <map>
#include <string>

int main()
{
  std::map<std::string, int> counts{};
  typedef std::map<std::string,int> count_map;
  typedef count_map::iterator count_iterator;


  // Read words from the standard input and count the number of times
  // each word occurs.
  std::string word{};
  while (std::cin >> word)
    ++counts[word];

  count_iterator the{counts.find("the")};

  if (the == counts.end())
    std::cout << "\"the\": not found\n";
  else if (the->second == 1)
    std::cout << "\"the\": occurs " << the->second << " time\n";
  else
    std::cout << "\"the\": occurs " << the->second << " times\n";
}
