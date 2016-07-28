#include <cassert>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "rational.hpp"

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

// Listing 46-2. Using a Function and Conditional Statements Instead of Conditional Expressions
/// Construct a rational object from a numerator and a denominator.
/// If the denominator is zero, throw zero_denominator. If the denominator
/// is negative, normalize the value by negating the numerator and denominator.
/// @post denominator_ > 0
/// @throws zero_denominator
rational::rational(int num, int den)
: numerator_{den < 0 ? -num : num}, denominator_{init_denominator(den)}
{
  reduce();
}

/// Return an initial value for the denominator_ member. This function is used
/// only in a constructor's initializer list.
int rational::init_denominator(int den)
{
  if (den == 0)
    throw zero_denominator("0 denominator");
  else if (den < 0)
    return -den;
  else
    return den;
}

rational::rational(double r)
: rational{static_cast<int>(r * 10000), 10000}
{
  reduce();
}

float rational::to_float()
const
{
  return static_cast<float>(numerator()) / denominator();
}

double rational::to_double()
const
{
  return static_cast<double>(numerator()) / denominator();
}

long double rational::to_long_double()
const
{
  return static_cast<long double>(numerator()) / denominator();
}

/// Assign a numerator and a denominator, then reduce to normal form.
void assign(int num, int den)
{
  numerator_ = den < 0 ? -num : num;
  denominator_ = init_denominator(den);
  reduce();
}

void rational::reduce()
{
  int div{gcd(numerator(), denominator())};
  numerator_ = numerator() / div;
  denominator_ = denominator() / div;
}

// Listing 46-5. Implementing the Multiplication Assignment Operator
rational& rational::operator*=(rational const& rhs)
{
  numerator_ *= rhs.numerator();
  denominator_ *= rhs.denominator();
  reduce();
  return *this;
}

// Listing 46-7. Other Arithmetic Assignment Operators
rational& rational::operator+=(rational const& rhs)
{
  numerator_ = numerator() * rhs.denominator() + rhs.numerator() * denominator();
  denominator_ *= rhs.denominator();
  reduce();
  return *this;
}

rational& rational::operator-=(rational const& rhs)
{
  numerator_ = numerator() * rhs.denominator() - rhs.numerator() * denominator();
  denominator_ *= rhs.denominator();
  reduce();
  return *this;
}

rational& rational::operator/=(rational const& rhs)
{
  if (rhs.numerator() == 0)
    throw zero_denominator{"divide by zero"};
  numerator_ *= rhs.denominator();
  denominator_ *= rhs.numerator();
  if (denominator_ < 0)
  {
    denominator_ = -denominator_;
    numerator_ = -numerator_;
  }
  reduce();
  return *this;
}

// Listing 46-8. The Prefix Increment Operator for rational
rational& rational::operator++()
{
  numerator_ += denominator_;
  return *this;
}

rational& rational::operator--()
{
  numerator_ -= denominator_;
  return *this;
}

// Listing 46-9. Postfix Increment and Decrement Operators
rational rational::operator++(int)
{
  rational result{*this};
  numerator_ += denominator_;
  return result;
}

rational rational::operator--(int)
{
  rational result{*this};
  numerator_ -= denominator_;
  return result;
}

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
  rational result{lhs};
  result += rhs;
  return result;
}

/// Subtraction of rational numbers.
rational operator-(rational const& lhs, rational const& rhs)
{
  rational result{lhs};
  result -= rhs;
  return result;
}

/// Multiplication of rational numbers.
rational operator*(rational const& lhs, rational const& rhs)
{
  rational result{lhs};
  result *= rhs;
  return result;
}

rational operator/(rational const& lhs, rational const& rhs)
{
  rational result{lhs};
  result /= rhs;
  return result;
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
