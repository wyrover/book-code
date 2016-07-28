// Listing 54-6. Demonstrating How Collation Order Depends on Locale
#include <algorithm>
#include <iostream>
#include <iterator>
#include <locale>
#include <string>
#include <vector>

void sort_words(std::vector<std::string> words, std::locale loc)
{
  std::sort(words.begin(), words.end(), loc);
  std::cout << loc.name() << ":\n";
  std::copy(words.begin(), words.end(),
            std::ostream_iterator<std::string>(std::cout, "\n"));
}

int main()
{
  using namespace std;
  vector<string> words{ 
    "circus",
    "\u00e5ngstrom",     // ångstrom
    "\u00e7irc\u00ea",   // çircê
    "angle",
    "essen",
    "ether",
    "\u00e6ther",        // æther
    "aether",
    "e\u00dfen"         // eßen
  };
  sort_words(words, locale::classic());
  sort_words(words, locale{"en_GB.utf8"});  // Great Britain
  sort_words(words, locale{"no_NO.utf8"});  // Norway
}
