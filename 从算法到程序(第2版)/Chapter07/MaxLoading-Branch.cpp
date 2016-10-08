#include<iostream>
#include<queue>
#include<algorithm>
using namespace std;

#define NUM 100
int n;
int c;
int w[NUM];

int MaxLoading()
{
    queue<int> Q;
    Q.push(-1);
    int i = 0;
    int Ew = 0;
    int bestw = 0;
    int r = 0;

    for (int j = 1; j < n; j++)
        r += w[j];

    while (true) {
        int wt = Ew + w[i];

        if (wt <= c) {
            if (wt > bestw) bestw = wt;

            if (i < n - 1) Q.push(wt);
        }

        if (Ew + r > bestw && i < n - 1) Q.push(Ew);

        Ew = Q.front();
        Q.pop();

        if (Ew == -1) {
            if (Q.empty()) return bestw;

            Q.push(-1);
            Ew = Q.front();
            Q.pop();
            i++;
            r -= w[i];
        }
    }

    return bestw;
}

int main()
{
    while (scanf("%d%d", &c, &n) != EOF) {
        for (int i = 0; i < n; i++)
            scanf("%d", &w[i]);

        int ans  =  MaxLoading();

        if (ans) printf("%d\n", ans);
        else printf("No answer!\n");
    }

    return 0;
}
