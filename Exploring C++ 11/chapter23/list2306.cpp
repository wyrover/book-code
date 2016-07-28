// Listing 23-6. Testing for Palindromes
#include <algorithm>
#include <iostream>
#include <iterator>
#include <locale>
#include <string>

/** Determine whether @p str is a palindrome.
 * Only letter characters are tested. Spaces and punctuation don't count.
 * Empty strings are not palindromes because that's just too easy.
 * @param str the string to test
 * @return true if @p str is the same forward and backward
 */
bool is_palindrome(std::string str)
{
  // Filter the string to keep only letters
  std::string::iterator end{std::remove_if(str.begin(), str.end(),
    [](char ch)
    { 
      return not std::isalpha(ch, std::locale());
    })
  };

  // Reverse the filtered string.
  std::string rev{str.begin(), end};
  std::reverse(rev.begin(), rev.end());

  // Compare the filtered string with its reversal, without regard to case.
  return not rev.empty() and std::equal(str.begin(), end, rev.begin(),
    [](char a, char b)
    {
        auto lowercase = [](char ch)
        {
           return std::tolower(ch, std::locale());
        };
        return lowercase(a) == lowercase(b);
    }
  );
}

int main()
{
  std::locale::global(std::locale{""});
  std::cin.imbue(std::locale{});
  std::cout.imbue(std::locale{});

  std::string line{};
  while (std::getline(std::cin, line))
    if (is_palindrome(line))
      std::cout << line << '\n';
}
