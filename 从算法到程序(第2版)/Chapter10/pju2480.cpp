#include <stdio.h>

int main()
{
    int n;

    while (scanf("%d", &n) != EOF) {
        __int64 i;
        __int64 ans = n;
        int p, a;

        for (i = 2; i * i <= n; ++i) {
            if (n % i == 0) {
                a = 0;
                p = i;

                while (n % p == 0) {
                    a++;
                    n /= p;
                }

                ans = ans + ans * a * (p - 1) / p;
            }
        }

        if (n != 1) ans = ans + ans * (n - 1) / n;

        printf("%I64d\n", ans);
    }

    return 0;
}
