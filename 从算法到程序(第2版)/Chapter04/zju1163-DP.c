#include <stdio.h>

#define MAXN 501
long long f[MAXN][MAXN];

int main()
{
    int i, j;
    int n;
    f[0][0] = 1;

    for (i = 0; i < MAXN; ++i) {
        for (j = 1; j <= i; ++j)
            f[i][j] = f[i][j - 1] + f[i - j][j - 1];

        for (j = i + 1; j < MAXN; ++j)
            f[i][j] = f[i][i];
    }

    while (scanf("%d", &n) && n)
        printf("%lld\n", f[n][n] - 1);

    return 0;
}
