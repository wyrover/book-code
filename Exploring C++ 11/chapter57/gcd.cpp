#include "gcd.hpp"

int gcd(int n, int m)
{
  if (n < 0)
    n = -n;
  while (m != 0) {
    int tmp{n % m};
    n = m;
    m = tmp;
  }
  return n;
}
