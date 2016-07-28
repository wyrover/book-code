// Listing 56-12. Rewriting to_string As a Template Function
#include <ostream> // for the << operator
#include <sstream> // for ostringstream
#include <string>  // for basic_string

template<class T, class Char, class Traits, class Allocator>
std::basic_string<Char, Traits, Allocator> to_string(T const& obj)
{
  std::basic_ostringstream<Char, Traits> out{};
  out << obj;
  return out.str();
}
