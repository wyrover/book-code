// Listing 56-9. The from_string Function Extracts a Value from a String
#include <istream> // for the >> operator
#include <sstream> // for istringstream
#include <string>  // for string
#include "conversion_error.hpp"

template<class T>
T from_string(std::string const& str)
{
  std::istringstream in{str};
  T result{};
  if (in >> result)
    return result;
  else
    throw conversion_error{str};
}

