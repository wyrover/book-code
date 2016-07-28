// Listing 48-9. The gcd Function Template
#ifndef GCD_HPP_
#define GCD_HPP_

#include <cstdlib> // for std::abs

template<class T>
/// Compute the greatest common divisor of two integers, using Euclid's algorithm.
T gcd(T n, T m)
{
  n = std::abs(n);
  while (m != 0) {
    T tmp{n % m};
    n = m;
    m = tmp;
  }
  return n;
}
#endif
