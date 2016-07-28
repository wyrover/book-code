// Listing 30-5. Testing the rational Comparison Operators
#include <cassert>
#include <cstdlib>
#include <iostream>
#include "test.hpp"

// Listing 29-3. Adding the reduce Member Function
/// Compute the greatest common divisor of two integers, using Euclid’s algorithm.
int gcd(int n, int m)
{
  n = std::abs(n);
  while (m != 0) {
    int tmp{n % m};
    n = m;
    m = tmp;
  }
  return n;
}

// Listing 30-1. Overloading the Equality Operator
/// Represent a rational number.
struct rational
{
  /// Construct a rational object, given a numerator and a denominator.
  /// Always reduce to normal form.
  /// @param num numerator
  /// @param den denominator
  /// @pre denominator > 0
  rational(int num, int den)
  : numerator{num}, denominator{den}
  {
    reduce();
  }

  /// Assign a numerator and a denominator, then reduce to normal form.
  /// @param num numerator
  /// @param den denominator
  /// @pre denominator > 0
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

  int numerator;     ///< numerator gets the sign of the rational value
  int denominator;   ///< denominator is always positive
};

// Listing 30-2. Using inline for Trivial Functions
/// Compare two rational numbers for equality.
/// @pre @p a and @p b are reduced to normal form
bool operator==(rational const& a, rational const& b)
{
  return a.numerator == b.numerator and a.denominator == b.denominator;
}

/// Compare two rational numbers for inequality.
/// @pre @p a and @p b are reduced to normal form
inline bool operator!=(rational const& a, rational const& b)
{
  return not (a == b);
}

// Listing 30-3. Implementing the < Operator for rational
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

// Listing 30-4. Implementing the > and >= Operators in Terms of <
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

int main()
{
  rational a{60, 5};
  rational b{12, 1};
  rational c{-24, -2};
  TEST(a == b);
  TEST(a >= b);
  TEST(a <= b);
  TEST(b <= a);
  TEST(b >= a);
  TEST(b == c);
  TEST(b >= c);
  TEST(b <= c);
  TEST(a == c);
  TEST(a >= c);
  TEST(a <= c);

  rational d{109, 10};
  TEST(d < a);
  TEST(d <= a);
  TEST(d != a);
  TEST(a > d);
  TEST(a >= d);
  TEST(a != d);

  rational e{241, 20};
  TEST(e > a);
  TEST(e >= a);
  TEST(e != a);
  TEST(a < e);
  TEST(a <= e);
  TEST(a != e);
}
