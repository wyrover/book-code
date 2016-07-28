// Listing 49-4. Simple I/O Test of the rational Class Template
#include <iostream>
#include "rational.hpp"

template<class T>
std::istream& operator>>(std::istream& in, rational<T>& rat)
{
  T n{}, d{};
  char sep{};
  if (not (in >> n >> sep))
    // Error reading the numerator or the separator character.
    in.setstate(in.failbit);
  else if (sep != '/')
  {
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
    in.setstate(in.failbit);

  return in;
}

int main()
{
  rational<int> r{};
  while (std::cin >> r)
    std::cout << r << '\n';
}

