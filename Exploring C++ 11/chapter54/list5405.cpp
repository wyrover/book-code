// Listing 54-5. Counting Words Again, This Time with Cached Facets

// Copy the initial portion of Listing 22-2 here, including print_counts,
// but stopping just before sanitize.
// Listing 22-2. New main Function That Sets the Global Locale
#include <iomanip>
#include <iostream>
#include <locale>
#include <map>
#include <string>

typedef std::map<std::string, int> count_map;  ///< Map words to counts
typedef count_map::value_type      count_pair; ///< pair of a word and a count
typedef std::string::size_type     str_size;   ///< String size type

/** Initialize the I/O streams by imbuing them with
 * the global locale. Use this function to imbue the streams
 * with the native locale. C++ initially imbues streams with
 * the classic locale.
 */
void initialize_streams()
{
  std::cin.imbue(std::locale{});
  std::cout.imbue(std::locale{});
}

/** Find the longest key in a map.
 * @param map the map to search
 * @returns the size of the longest key in @p map
 */
str_size get_longest_key(count_map const& map)
{
  str_size result{0};
  for (auto pair : map)
    if (pair.first.size() > result)
      result = pair.first.size();
  return result;
}

/** Print the word, count, newline. Keep the columns neatly aligned.
 * Rather than the tedious operation of measuring the magnitude of all
 * the counts and then determining the necessary number of columns, just
 * use a sufficiently large value for the counts column.
 * @param pair a word/count pair
 * @param longest the size of the longest key; pad all keys to this size
 */
void print_pair(count_pair const& pair, str_size longest)
{
  int const count_size{10}; // Number of places for printing the count
  std::cout << std::setw(longest)    << std::left  << pair.first <<
               std::setw(count_size) << std::right << pair.second << '\n';
}

/** Print the results in neat columns.
 * @param counts the map of all the counts
 */
void print_counts(count_map counts)
{
  str_size longest{get_longest_key(counts)};
  
  // For each word/count pair...
  for (count_pair pair: counts)
    print_pair(pair, longest);
}

/** Base class to hold a ctype facet. */
class function
{
public:
  function(std::locale loc) : ctype_{std::use_facet<std::ctype<char>>(loc)} {}
  bool isalnum(char ch) const { return ctype_.is(std::ctype_base::alnum, ch); }
  char tolower(char ch) const { return ctype_.tolower(ch); }
private:
  std::ctype<char> const& ctype_;
};

/** Sanitize a string by keeping only alphabetic characters.
 * @param str the original string
 * @return a santized copy of the string
 */
class sanitizer : public function
{
public:
  typedef std::string argument_type;
  typedef std::string result_type;
  sanitizer(std::locale loc) : function{loc} {}
  std::string operator()(std::string const& str)
  {
    std::string result{};
    for (char c : str)
      if (isalnum(c))
        result.push_back(tolower(c));
    return result;
  }
};

/** Main program to count unique words in the standard input. */
int main()
{
  // Set the global locale to the native locale.
  std::locale::global(std::locale{""});
  initialize_streams();

  count_map counts{};

  // Read words from the standard input and count the number of times
  // each word occurs.
  std::string word{};
  sanitizer sanitize{std::locale{}};
  while (std::cin >> word)
  {
    std::string copy{sanitize(word)};

    // The "word" might be all punctuation, so the copy would be empty.
    // Don't count empty strings.
    if (not copy.empty())
      ++counts[copy];
  }

  print_counts(counts);
}
