#include <sstream>

#include "calc_error.hpp"

std::string function_error::msg(std::string const& name, std::size_t expected, std::size_t actual)
{
  std::ostringstream stream{};
  stream << "wrong number of arguments in call to " + name + "(), expected " << expected << ", got " << actual;
  return stream.str();
}
