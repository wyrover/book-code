#include<iostream>
using namespace std;

#define NUM 10000
int n;
int c;
int cw;
int bestw;
int w[NUM];
int x[NUM];
int r;
bool flag;

void backtrack(int t)
{
    if (t > n) {
        if (cw == c) {
            for (int i = 1; i <= n; i++)
                if (x[i]) printf("%d ", w[i]);

            printf("\n");
            flag = false;
        }

        return;
    }

    r -= w[t];

    if (cw + w[t] <= c) {
        x[t] = 1;
        cw += w[t];
        backtrack(t + 1);
        cw -= w[t];
    }

    if (cw + r > bestw) {
        x[t] = 0;
        backtrack(t + 1);
    }

    r += w[t];
}

int main()
{
    while (scanf("%d%d", &n, &c) && (n || c)) {
        r = 0;

        for (int i = 1; i <= n; i++) {
            scanf("%d", &w[i]);
            r += w[i];
        }

        cw = 0;
        bestw = 0;
        flag = true;
        backtrack(1);

        if (flag) printf("No Solution!\n");
    }

    return 0;
}
