// Listing 56-16. Making to_string Too Complicated
#include <ios>
#include <ostream> // for the << operator
#include <sstream> // for ostringstream

template<class String, class T>
String to_string(T const& obj,
  std::ios_base::fmtflags flags = std::ios_base::fmtflags{},
  int width = 0,
  char fill = ' ')
{
  std::basic_ostringstream<typename String::value_type,
                           typename String::traits_type> out{};
  out.flags(flags);
  out.width(width);
  out.fill(fill);
  out << obj;
  return out.str();
}
