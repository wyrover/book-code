#include <stdio.h>
#include <memory.h>

#define MAXN 501
long long f[MAXN];

int main()
{
    int i, j;
    int n;
    memset(f, 0, sizeof(f));
    f[0] = 1;

    for (i = 1; i < MAXN; i++)
        for (j = MAXN - 1; j >= i; j--)
            f[j] += f[j - i];

    while (scanf("%d", &n) && n)
        printf("%lld\n", f[n] - 1);

    return 0;
}
