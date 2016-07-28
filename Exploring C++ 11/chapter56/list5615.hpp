// Listing 56-15. Improving the Calling Interface of to_string
#include <ostream> // for the << operator
#include <sstream> // for ostringstream

template<class String, class T>
String to_string(T const& obj)
{
  std::basic_ostringstream<typename String::value_type,
                           typename String::traits_type> out{};
  out << obj;
  return out.str();
}
