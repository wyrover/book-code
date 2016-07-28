// Listing 55-1. Supporting Cast for the is_palindrome Function Template
#include <algorithm>
#include <iostream>
#include <iterator>
#include <locale>
#include <string>

/** Test for non-letter.
 * @param ch the character to test
 * @return true if @p ch is not a letter
 */
template<class Char>
bool non_letter(Char ch)
{
  return not std::isalpha(ch, std::locale{});
}

/** Convert to lowercase.
 * Use a canonical form by converting to uppercase first,
 * and then to lowercase.
 * @param ch the character to test
 * @return the character converted to lowercase
 */
template<class Char>
Char lowercase(Char ch)
{
  return std::tolower(ch, std::locale{});
}

/** Compare two characters without regard to case. */
template<class Char>
bool same_char(Char a, Char b)
{
  return lowercase(a) == lowercase(b);
}

// Listing 55-2. Changing is_palindrome to a Function Template
/** Determine whether @p str is a palindrome.
 * Only letter characters are tested. Spaces and punctuation don't count.
 * Empty strings are not palindromes because that's just too easy.
 * @param str the string to test
 * @return true if @p str is the same forward and backward
 */
template<class Char, class Traits, class Allocator>
bool is_palindrome(std::basic_string<Char, Traits, Allocator> str)
{
  typedef typename std::basic_string<Char, Traits, Allocator> string;
  typename string::iterator end{
      std::remove_if(str.begin(), str.end(), non_letter<Char>)};
  string rev{str.begin(), end};
  std::reverse(rev.begin(), rev.end());
  return not rev.empty() and
             std::equal(str.begin(), end, rev.begin(), same_char<Char>);
}

// Listing 55-3. The main Program for Testing is_palindrome
int main()
{
  std::locale::global(std::locale{""});
  std::wcin.imbue(std::locale{});
  std::wcout.imbue(std::locale{});

  std::wstring line{};
  while (std::getline(std::wcin, line))
    if (is_palindrome(line))
      std::wcout << line << L'\n';
}

