#include <iostream>
#include<ctime>
using namespace std;

#define NUM 50
#define CAP 1500
int v[NUM];
int w[NUM];
int p[NUM][CAP];
void knapsack(int c, int n)
{
    int jMax = min(w[n] - 1, c);

    for (int j = 0; j <= jMax; j++)
        p[n][j] = 0;

    for (int j = w[n]; j <= c; j++)
        p[n][j] = v[n];

    for (int i = n - 1; i > 1; i--) {
        jMax = min(w[i] - 1, c);

        for (int j = 0; j <= jMax; j++)
            p[i][j] = p[i + 1][j];

        for (int j = w[i]; j <= c; j++)
            p[i][j] = max(p[i + 1][j], p[i + 1][j - w[i]] + v[i]);
    }

    p[1][c] = p[2][c];

    if (c >= w[1])
        p[1][c] = max(p[1][c], p[2][c - w[1]] + v[1]);
}

void traceback(int c, int n, int x[ ])
{
    for (int i = 1; i < n; i++) {
        if (p[i][c] == p[i + 1][c]) x[i] = 0;
        else {
            x[i] = 1;
            c -= w[i];
        }
    }

    x[n] = (p[n][c]) ? 1 : 0;
}

int main()
{
    int x[NUM];
    int W;
    int n;

    while (scanf("%d", &W) && W) {
        scanf("%d", &n);

        for (int i = 1; i <= n; i++)
            scanf("%d%d", &w[i], &v[i]);

        memset(p, 0, sizeof(p));
        knapsack(W, n);
        printf("%d\n", p[1][W]);
        traceback(W, n, x);

        for (int i = 1; i <= n; i++)
            if (x[i]) printf("%d ", i);

        printf("\n");
    }

    return 0;
}
