#include<stdio.h>
#include<string.h>

#define N 21
__int64 fac[N] = {1, 1};
__int64 stir[N][N];

void Factorial_Stirling()
{
    int i, j;

    for (i = 2; i < N; i++)
        fac[i] = i * fac[i - 1];

    memset(stir, 0, sizeof(stir));
    stir[0][0] = 0;
    stir[1][1] = 1;

    for (i = 2; i < N; i++)
        for (j = 1; j <= i; j++)
            stir[i][j] = stir[i - 1][j - 1] + (i - 1) * stir[i - 1][j];
}

int main()
{
    Factorial_Stirling();
    int iCase;
    scanf("%d", &iCase);

    while (iCase--) {
        int n, k, i;
        scanf("%d %d", &n, &k);
        __int64 cnt = 0;

        for (i = 1; i <= k; i++)
            cnt += stir[n][i] - stir[n - 1][i - 1];

        printf("%.4lf\n", 1.0 * cnt / fac[n]);
    }

    return 0;
}
