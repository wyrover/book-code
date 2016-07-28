// Listing 56-14. Passing the Destination String As an Argument to to_string
#include <ostream> // for the << operator
#include <sstream> // for ostringstream
#include <string>  // for string
#include "from_string.hpp"

template<class T, class Char, class Traits, class Allocator>
void to_string(T const& obj, std::basic_string<Char, Traits, Allocator>& result)
{
  std::basic_ostringstream<Char, Traits> out{};
  out << obj;
  result = out.str();
}

int main()
{
    std::string str{};
    to_string(42, str);
    int value(from_string<int>(str));
}
