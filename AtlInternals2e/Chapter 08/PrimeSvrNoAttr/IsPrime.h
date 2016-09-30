
#include <math.h>

inline
bool IsPrime(long n)
{
    // Special case: < 2
    if (n < 2) return false;

    // Special case: first couple of primes
    if (n == 2 || n == 3) {
        return true;
    }

    // Special case: even
    if (n % 2 == 0) return false;

    // Special case: has a square root
    double  root = sqrt(double(n));

    if (double(long(root)) == root) return false;

    const long begin = 3;
    const long end = long(root) + 1;

    // Special case: root < begin
    if (end < begin) return true;

    for (long i = begin; i != end; ++i) {
        if (n % i == 0) return false;
    }

    return true;
}
