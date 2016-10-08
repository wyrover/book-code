#include<stdio.h>
#include<string.h>

int fun(int m, __int64 n)
{
    int res = 1;

    while (n) {
        if (n & 1) res = res * m % 100;

        m = m * m % 100;
        n >>= 1;
    }

    return res;
}

int main()
{
    int iCase, i;
    __int64 n;

    while (scanf("%d", &iCase), iCase) {
        for (i = 1; i <= iCase; i++) {
            scanf("%I64d", &n);
            printf("Case %d: %d\n", i, (fun(4, n - 1) + fun(2, n - 1)) % 100);
        }

        puts("");
    }

    return 0;
}
