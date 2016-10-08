#include <iostream>
using namespace std;

#define NUM 20
#define infinite 10000
int n;
int job[NUM][3];
int x[NUM];
int bestx[NUM];
int f1;
int f2[NUM];
int f;
int bestf;

void Backtrack(int t)
{
    if (t > n) {
        bestf = f;

        for (int i = 1; i <= n; i++)
            bestx[i] = x[i];

        return;
    }

    for (int i = t; i <= n; i++) {
        f1 += job[x[i]][1];
        f2[t] = ((f2[t - 1] > f1) ? f2[t - 1] : f1) + job[x[i]][2];
        f += f2[t];

        if (f < bestf) {
            swap(x[t], x[i]);
            Backtrack(t + 1);
            swap(x[t], x[i]);
        }

        f1 -= job[x[i]][1];
        f -= f2[t];
    }
}

int main()
{
    int i;

    while (cin >> n) {
        memset(bestx, 0, sizeof(bestx));
        memset(f2, 0, sizeof(f2));

        for (i = 1; i <= n; i++)
            scanf("%d%d", &job[i][1], &job[i][2]);

        bestf = infinite;
        f1 = 0;
        f = 0;

        for (i = 0; i <= n; i++)
            x[i] = i;

        Backtrack(1);
        cout << bestf << endl;
    }
}
