// Listing 32-5. Complete Definition of rational and Its Operators
#include <cassert>
#include <cstdlib>
#include <istream>
#include <ostream>
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

/// Represent a rational number (fraction) as a numerator and denominator.
struct rational
{
  rational()
  : numerator{0}, denominator{1}
  {/*empty*/}

  rational(int num)
  : numerator{num}, denominator{1}
  {/*empty*/}

  rational(int num, int den)
  : numerator{num}, denominator{den}
  {
    reduce();
  }

  rational(double r)
  : rational{static_cast<int>(r * 10000), 10000}
  {/*empty*/}

  rational& operator=(rational const& that)
  {
    this->numerator = that.numerator;
    this->denominator = that.denominator;
    reduce();
    return *this;
  }

  float as_float()
  {
    return static_cast<float>(numerator) / denominator;
  }

  double as_double()
  {
    return static_cast<double>(numerator) / denominator;
  }

  long double as_long_double()
  {
    return static_cast<long double>(numerator) / 
           denominator;
  }

  /// Assign a numerator and a denominator, then reduce to normal form.
  void assign(int num, int den)
  {
    numerator = num;
    denominator = den;
    reduce();
  }

  /// Reduce the numerator and denominator by their GCD.
  void reduce()
  {
    assert(denominator != 0);
    if (denominator < 0)
    {
      denominator = -denominator;
      numerator = -numerator;
    }
    int div{gcd(numerator, denominator)};
    numerator = numerator / div;
    denominator = denominator / div;
  }

  int numerator;
  int denominator;
};

/// Absolute value of a rational number.
rational abs(rational const& r)
{
  return rational{std::abs(r.numerator), r.denominator};
}

/// Unary negation of a rational number.
rational operator-(rational const& r)
{
  return rational{-r.numerator, r.denominator};
}

/// Add rational numbers.
rational operator+(rational const& lhs, rational const& rhs)
{
  return rational{lhs.numerator * rhs.denominator + rhs.numerator * lhs.denominator,
                  lhs.denominator * rhs.denominator};
}

/// Subtraction of rational numbers.
rational operator-(rational const& lhs, rational const& rhs)
{
  return rational{lhs.numerator * rhs.denominator - rhs.numerator * lhs.denominator,
                  lhs.denominator * rhs.denominator};
}

/// Multiplication of rational numbers.
rational operator*(rational const& lhs, rational const& rhs)
{
  return rational{lhs.numerator * rhs.numerator, lhs.denominator * rhs.denominator};
}

/// Division of rational numbers.
/// TODO: check for division-by-zero
rational operator/(rational const& lhs, rational const& rhs)
{
  return rational{lhs.numerator * rhs.denominator, lhs.denominator * rhs.numerator};
}

/// Compare two rational numbers for equality.
bool operator==(rational const& a, rational const& b)
{
  return a.numerator == b.numerator and a.denominator == b.denominator;
}

/// Compare two rational numbers for inequality.
inline bool operator!=(rational const& a, rational const& b)
{
  return not (a == b);
}
/// Compare two rational numbers for less-than.
bool operator<(rational const& a, rational const& b)
{
  return a.numerator * b.denominator < b.numerator * a.denominator;
}

/// Compare two rational numbers for less-than-or-equal.
inline bool operator<=(rational const& a, rational const& b)
{
  return not (b < a);
}
/// Compare two rational numbers for greater-than.
inline bool operator>(rational const& a, rational const& b)
{
  return b < a;
}

/// Compare two rational numbers for greater-than-or-equal.
inline bool operator>=(rational const& a, rational const& b)
{
  return not (b > a);
}

/// Read a rational number.
/// Format is @em integer @c / @em integer.
std::istream& operator>>(std::istream& in, rational& rat)
{
  int n{0}, d{0};
  char sep{'\0'};
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

/// Write a rational numbers.
/// Format is @em numerator @c / @em denominator.
std::ostream& operator<<(std::ostream& out, rational const& rat)
{
  std::ostringstream tmp{};
  tmp << rat.numerator << '/' << rat.denominator;
  out << tmp.str();

  return out;
}
