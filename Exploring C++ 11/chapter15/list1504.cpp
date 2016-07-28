// Listing 15-4. Searching for a Word in a Map
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

  std::map<std::string,int>::iterator the{counts.find("the")};
  if (the == counts.end())
    std::cout << "\"the\": not found\n";
  else if (the->second == 1)
    std::cout << "\"the\": occurs " << the->second << " time\n";
  else
    std::cout << "\"the\": occurs " << the->second << " times\n";
}

