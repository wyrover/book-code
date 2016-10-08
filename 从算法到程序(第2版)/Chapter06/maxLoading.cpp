#include<iostream>
using namespace std;

#define NUM 1000
int n;
int c;
int w[NUM];
int x[NUM];
int r;
int cw;
int bestw;
int bestx[NUM];

void Backtrack(int t)
{
    if (t > n) {
        if (cw > bestw) {
            for (int i = 1; i <= n; i++)
                bestx[i] = x[i];

            bestw = cw;
        }

        return;
    }

    r -= w[t];

    if (cw + w[t] <= c) {
        x[t] = 1;
        cw += w[t];
        Backtrack(t + 1);
        cw -= w[t];
    }

    if (cw + r > bestw) {
        x[t] = 0;
        Backtrack(t + 1);
    }

    r += w[t];
}

int main()
{
    while (scanf("%d%d", &c, &n) != EOF) {
        r = 0;

        for (int i = 1; i <= n; i++) {
            scanf("%d", &w[i]);
            r += w[i];
        }

        cw = 0;
        bestw = 0;
        Backtrack(1);
        printf("%d\n", bestw);

        for (int i = 1; i <= n; i++)
            if (bestx[i]) printf("%d ", i);

        printf("\n");
    }
}
