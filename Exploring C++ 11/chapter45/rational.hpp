// Listing 45-6. Throwing an Exception for a Zero Denominator
#ifndef RATIONAL_HPP_
#define RATIONAL_HPP_

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

// Listing 35-4. The Latest Rewrite of the rational Class

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


class rational
{
public:
  class zero_denominator : public std::logic_error
  {
  public:
    zero_denominator(std::string const& what_arg) : logic_error{what_arg} {}
  };

  rational() : rational{0} {}
  rational(int num) : numerator_{num}, denominator_{1} {}
  rational(int num, int den) : numerator_{num}, denominator_{den}
  {
    if (denominator_ == 0)
      throw zero_denominator{"zero denominator"};
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
#endif
