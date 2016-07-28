// Listing 56-10. Enhancing the from_string Function
#include <ios> // for std::noskipws
#include <istream> // for the >> operator
#include <sstream> // for istringstream
#include <string>  // for string
#include "conversion_error.hpp"

template<class T>
T from_string(std::string const& str, bool skipws = true)
{
  std::istringstream in{str};
  if (not skipws)
    in >> std::noskipws;
  T result{};
  char extra;
  if (not (in >> result))
    throw conversion_error{str};
  else if (in >> extra)
    throw conversion_error{str};
  else
    return result;
}
