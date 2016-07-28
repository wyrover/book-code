// Listing 35-4. The Latest Rewrite of the rational Class
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

/// Represent a rational number (fraction) as a numerator and denominator.
class rational
{
public:
  rational(): rational{0}  {}
  rational(int num): numerator_{num}, denominator_{1} {} // no need to reduce
  rational(rational const&) = default;
  rational(int num, int den)
  : numerator_{num}, denominator_{den}
  {
    reduce();
  }

  rational(double r)
  : rational{static_cast<int>(r * 10000), 10000}
  {
    reduce();
  }

  int numerator()   const { return numerator_; }
  int denominator() const { return denominator_; }
  float to_float()
  const
  {
    return static_cast<float>(numerator()) / denominator();
  }

  double to_double()
  const
  {
    return static_cast<double>(numerator()) / denominator();
  }

  long double to_long_double()
  const
  {
    return static_cast<long double>(numerator()) / 
           denominator();
  }

  /// Assign a numerator and a denominator, then reduce to normal form.
  void assign(int num, int den)
  {
    numerator_ = num;
    denominator_ = den;
    reduce();
  }
private:
  /// Reduce the numerator and denominator by their GCD.
  void reduce()
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

  int numerator_;
  int denominator_;
};

/// Absolute value of a rational number.
rational abs(rational const& r)
{
  return rational{std::abs(r.numerator()), r.denominator()};
}

/// Unary negation of a rational number.
rational operator-(rational const& r)
{
  return rational{-r.numerator(), r.denominator()};
}

/// Add rational numbers.
rational operator+(rational const& lhs, rational const& rhs)
{
  return rational{
          lhs.numerator() * rhs.denominator() + rhs.numerator() * lhs.denominator(),
          lhs.denominator() * rhs.denominator()};
}

/// Subtraction of rational numbers.
rational operator-(rational const& lhs, rational const& rhs)
{
  return rational{
          lhs.numerator() * rhs.denominator() - rhs.numerator() * lhs.denominator(),
          lhs.denominator() * rhs.denominator()};
}

/// Multiplication of rational numbers.
rational operator*(rational const& lhs, rational const& rhs)
{
  return rational{lhs.numerator() * rhs.numerator(),
                  lhs.denominator() * rhs.denominator()};
}

/// Division of rational numbers.
/// TODO: check for division-by-zero
rational operator/(rational const& lhs, rational const& rhs)
{
  return rational{lhs.numerator() * rhs.denominator(),
                  lhs.denominator() * rhs.numerator()};
}

/// Compare two rational numbers for equality.
bool operator==(rational const& a, rational const& b)
{
  return a.numerator() == b.numerator() and a.denominator() == b.denominator();
}

/// Compare two rational numbers for inequality.
inline bool operator!=(rational const& a, rational const& b)
{
  return not (a == b);
}
/// Compare two rational numbers for less-than.
bool operator<(rational const& a, rational const& b)
{
  return a.numerator() * b.denominator() < b.numerator() * a.denominator();
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
  int n{}, d{};
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

/// Write a rational numbers.
/// Format is @em numerator @c / @em denominator.
std::ostream& operator<<(std::ostream& out, rational const& rat)
{
  std::ostringstream tmp{};
  tmp << rat.numerator() << '/' << rat.denominator();
  out << tmp.str();

  return out;
}
