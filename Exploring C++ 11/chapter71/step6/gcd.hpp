#ifndef GCD_HPP_
#define GCD_HPP_

/// Compute greatest-common-denominator.
/// @param n
/// @param m
template<class T>
T gcd(T n, T m)
{
  static T zero{};
  if (n < zero)
    n = -n;
  while (m != zero) {
    int tmp(n % m);
    n = m;
    m = tmp;
  }
  return n;
}
#endif
