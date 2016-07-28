// Listing 40-8. Declaring and Defining Static Data Members
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <sstream>

/// Compute the greatest common divisor of two integers, using Euclid’s algorithm.
int gcd(int n, int m)
{
  n = std::abs(n);
  while (m != 0) {
    int tmp(n % m);
    n = m;
    m = tmp;
  }
  return n;
}

class rational {
public:
  rational();
  rational(int num);
  rational(int num, int den);
  int numerator() const { return numerator_; }
  int denominator() const { return denominator_; }
  // Some useful constants
  static const rational zero;
  static const rational one;
  static const rational pi;
private:
  void reduce(); //get definition from Listing 35-4

  int numerator_;
  int denominator_;
};

rational::rational() : rational{0, 1} {}
rational::rational(int num) : numerator_{num}, denominator_{1} {}
rational::rational(int num, int den)
: numerator_{num}, denominator_{den}
{
  reduce();
}

void rational::reduce()
{
  assert(denominator() != 0);
  if (denominator() < 0)
  {
    denominator_ = -denominator();
    numerator_ = -numerator();
  }
  int div{gcd(numerator(), denominator())};
  numerator_ = numerator() / div;
  denominator_ = denominator() / div;
}

std::ostream& operator<<(std::ostream& out, rational const& r)
{
  std::ostringstream tmp{};
  tmp << r.numerator() << '/' << r.denominator();
  out << tmp.str();

  return out;
}

const rational rational::zero{};
const rational rational::one{1};
const rational rational::pi{355, 113};

int main()
{
  std::cout << "pi = " << rational::pi << '\n';
}

