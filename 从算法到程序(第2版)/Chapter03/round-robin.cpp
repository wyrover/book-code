//循环赛程表
#include <stdio.h>
#define MAX 100

int a[MAX][MAX];

void Copy(int tox, int toy, int fromx, int fromy, int r)
{
    for (int i = 0; i < r; i++)
        for (int j = 0; j < r; j++)
            a[tox + i][toy + j] = a[fromx + i][fromy + j];
}

void Table(int k)
{
    int i, r;
    int n = 1 << k;

    for (i = 0; i < n; i++)
        a[0][i] = i + 1;

    for (r = 1; r < n; r <<= 1)
        for (i = 0; i < n; i += 2 * r) {
            Copy(r, i + r, 0, i, r);
            Copy(r, i, 0, i + r, r);
        }
}

void Out(int n)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%3d", a[i][j]);

        printf("\n");
    }

    printf("\n");
}

int main()
{
    int k;

    while (scanf("%d", &k) && k) {
        int n = 1 << k;
        Table(k);
        Out(n);
    }

    return 0;
}
