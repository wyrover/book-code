// Listing 46-10. The rational Class Definition
#ifndef RATIONAL_HPP_
#define RATIONAL_HPP_

#include <iostream>
#include <stdexcept>
#include <string>

/// Represent a rational number (fraction) as a numerator and denominator.
class rational
{
public:
  class zero_denominator : public std::logic_error
  {
  public:
    zero_denominator(std::string const& what) : logic_error{what} {}
  };
  rational(): rational{0} {}
  rational(int num): numerator_{num}, denominator_{1} {}
  rational(int num, int den);
  rational(double r);

  int numerator()              const { return numerator_; }
  int denominator()            const { return denominator_; }
  float as_float()             const;
  double as_double()           const;
  long double as_long_double() const;

  rational& operator=(int); // optimization to avoid an unneeded call to reduce()
  rational& operator+=(rational const& rhs);
  rational& operator-=(rational const& rhs);
  rational& operator*=(rational const& rhs);
  rational& operator/=(rational const& rhs);
  rational& operator++();
  rational& operator--();
  rational operator++(int);
  rational operator--(int);

private:
  /// Reduce the numerator and denominator by their GCD.
  void reduce();
  /// Reduce the numerator and denominator, and normalize the signs of both,
  /// that is, ensure denominator is not negative.
  void normalize();
  /// Return an initial value for denominator_. Throw a zero_denominator
  /// exception if @p den is zero. Always return a positive number.
  int init_denominator(int den);
  int numerator_;
  int denominator_;
};

/// Compute the greatest common divisor of two integers, using Euclid's algorithm.
int gcd(int n, int m);

rational abs(rational const& r);
rational operator-(rational const& r);
rational operator+(rational const& lhs, rational const& rhs);
rational operator-(rational const& lhs, rational const& rhs);
rational operator*(rational const& lhs, rational const& rhs);
rational operator/(rational const& lhs, rational const& rhs);

bool operator==(rational const& a, rational const& b);
bool operator<(rational const& a, rational const& b);

inline bool operator!=(rational const& a, rational const& b)
{
  return not (a == b);
}

inline bool operator<=(rational const& a, rational const& b)
{
  return not (b < a);
}

inline bool operator>(rational const& a, rational const& b)
{
  return b < a;
}

inline bool operator>=(rational const& a, rational const& b)
{
  return not (b > a);
}

std::istream& operator>>(std::istream& in, rational& rat);
std::ostream& operator<<(std::ostream& out, rational const& rat);

#endif
