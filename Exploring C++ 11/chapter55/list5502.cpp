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
