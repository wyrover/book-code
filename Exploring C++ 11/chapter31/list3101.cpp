// Listing 31-1. Input Operator
#include <ios>      // declares failbit, etc.
#include <istream>  // declares std::istream and the necessary >> operators

std::istream& operator>>(std::istream& in, rational& rat)
{
  int n{0}, d{0};
  char sep{'\0'};
  if (not (in >> n >> sep))
    // Error reading the numerator or the separator character.
    in.setstate(std::cin.failbit);
  else if (sep != '/')
  {
    // Read numerator successfully, but it is not followed by /.
    // Push sep back into the input stream, so the next input operation
    // will read it.
    in.unget();
    rat.assign(n, 1);
  }
  else if (in >> d)
    // Successfully read numerator, separator, and denominator.
    rat.assign(n, d);
  else
    // Error reading denominator.
    in.setstate(std::cin.failbit);

  return in;
}

