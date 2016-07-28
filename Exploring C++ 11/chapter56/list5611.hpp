#ifndef TO_STRING_HPP_
#define TO_STRING_HPP_

// Listing 56-11. The to_string Function Converts a Value to a String
#include <ostream> // for the << operator
#include <sstream> // for ostringstream
#include <string>  // for string

template<class T>
std::string to_string(T const& obj)
{
  std::ostringstream out{};
  out << obj;
  return out.str();
}

#endif

