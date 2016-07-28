// Listing 31-2. Output Operator
#include <ostream>  // declares the necessary << operators
#include <sstream>  // declares the std::ostringstream type

std::ostream& operator<<(std::ostream& out, rational const& rat)
{
  std::ostringstream tmp{};
  tmp << rat.numerator;
  if (rat.denominator != 1)
    tmp << '/' << rat.denominator;
  out << tmp.str();

  return out;
}

