#include<stdio.h>

int n;
int a[110];
int GCD(int x, int y)
{
    return y ? GCD(y, x % y) : x;
}

int LCM(int x, int y)
{
    return x / GCD(x, y) * y;
}

int multiple(int x, int *cnt)
{
    int i;
    int res = 1;
    *cnt = 0;

    for (i = 0; i < n; i++)
        if (x & (1 << i)) {
            (*cnt)++;
            res = LCM(res, a[i]);
        }

    return res;
}

int main()
{
    int i, k;
    int m;
    int lcms;
    int ans;

    while (~scanf("%d%d", &n, &m)) {
        ans = 0;

        for (i = 0; i < n; i++)
            scanf("%d", &a[i]);

        for (i = 1; i < (1 << n); i++) {
            lcms = multiple(i, &k);

            if (k & 1) ans += m / lcms;
            else ans -= m / lcms;
        }

        printf("%d\n", ans);
    }

    return 0;
}
