#include <iostream>
using namespace std;

#define NUM 10000
int n;
int c;
int cw;
int bestw;
int w[NUM];
int x[NUM];
int bestx[NUM];
int r;

bool backtrack(int t)
{
    if (t > n) {
        for (int i = 1; i <= n; i++)
            bestx[i] = x[i];

        bestw = cw;

        if (bestw == c) return true;
        else return false;
    }

    r -= w[t];

    if (cw + w[t] <= c) {
        x[t] = 1;
        cw += w[t];

        if (backtrack(t + 1)) return true;

        cw -= w[t];
    }

    if (cw + r > bestw) {
        x[t] = 0;

        if (backtrack(t + 1)) return true;
    }

    r += w[t];
    return false;
}

int main()
{
    while (scanf("%d%d", &n, &c) && (n || c)) {
        int i;
        r = 0;

        for (i = 1; i <= n; i++) {
            scanf("%d", &w[i]);
            r += w[i];
        }

        cw = 0;
        bestw = 0;

        if (backtrack(1)) {
            for (i = 1; i <= n; i++)
                if (bestx[i]) printf("%d ", w[i]);

            printf("\n");
        } else printf("No Solution!");
    }

    return 0;
}
