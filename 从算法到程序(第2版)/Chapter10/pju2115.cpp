#include <stdio.h>

__int64 x, y;
__int64 Extended_Euclid(__int64 a, __int64 b)
{
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }

    __int64 d = Extended_Euclid(b, a % b);
    __int64 temp = x;
    x = y;
    y = temp - a / b * y;
    return d;
}

int main()
{
    __int64 a, b, c, k;

    while (scanf("%I64d%I64d%I64d%I64d", &a, &b, &c, &k), a || b || c || k) {
        __int64 n = (__int64)1 << k;
        __int64 d = Extended_Euclid(c, n);

        if ((b - a) % d) {
            printf("FOREVER\n");
            continue;
        }

        __int64 t = n / d;
        x = (x * (b - a) / d % t + t) % t;
        printf("%I64d\n", x);
    }

    return 0;
}
