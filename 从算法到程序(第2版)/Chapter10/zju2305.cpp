#include <stdio.h>

long long x, y;
long long Extended_Euclid(long long a, long long b)
{
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }

    long long d = Extended_Euclid(b, a % b);
    long long temp = x;
    x = y;
    y = temp - a / b * y;
    return d;
}

int main()
{
    long long a, b, c, k;

    while (scanf("%lld%lld%lld%lld", &a, &b, &c, &k), a || b || c || k) {
        long long n = (long long)1 << k;
        long long d = Extended_Euclid(c, n);

        if ((b - a) % d) {
            printf("FOREVER\n");
            continue;
        }

        long long t = n / d;
        x = (x * (b - a) / d % t + t) % t;
        printf("%lld\n", x);
    }

    return 0;
}
