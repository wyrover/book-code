#include <iostream>
#include <queue>
using namespace std;

#define inf 1000000
#define NUM 100
int n;
int a[NUM][NUM];
int NoEdge = inf;
int cc;
int bestc;

struct node {
    friend bool operator < (const node& a, const node& b)
    {
        if (a.lcost > b.lcost)  return true;
        else return false;
    }
    int lcost;
    int rcost;
    int cc;
    int s;
    int x[NUM];
};

int queueTSP()
{
    priority_queue <node> H;
    int v[NUM];
    int minOut[NUM];
    int minSum = 0;
    int i, j;

    for (i = 1; i <= n; i++) {
        int Min = NoEdge;

        for (j = 1; j <= n; j++)
            if (a[i][j] != NoEdge && (a[i][j] < Min || Min == NoEdge))
                Min = a[i][j];

        if (Min == NoEdge)  return NoEdge;

        minOut[i] = Min;
        minSum += Min;
    }

    node E;

    for (i = 1; i <= n; i++)
        E.x[i] = i;

    E.s = 1;
    E.cc = 0;
    E.rcost = minSum;
    int bestc = NoEdge;

    while (E.s < n) {
        if (E.s == n - 1) {
            if (a[E.x[n - 1]][E.x[n]] != NoEdge && a[E.x[n]][1] != NoEdge
                && (E.cc + a[E.x[n - 1]][E.x[n]] + a[E.x[n]][1] < bestc
                    || bestc == NoEdge)) {
                bestc = E.cc + a[E.x[n - 1]][E.x[n]] + a[E.x[n]][1];
                E.cc = bestc;
                E.lcost = bestc;
                E.s++;
                H.push(E);
            } else delete[] E.x;
        } else {
            for (i = E.s + 1; i <= n; i++)
                if (a[E.x[E.s]][E.x[i]] != NoEdge) {
                    int cc = E.cc + a[E.x[E.s]][E.x[i]];
                    int rcost = E.rcost - minOut[E.x[E.s]];
                    int B = cc + rcost;

                    if (B < bestc || bestc == NoEdge) {
                        node N;

                        for (j = 1; j <= n; j++)
                            N.x[j] = E.x[j];

                        N.x[E.s + 1] = E.x[i];
                        N.x[i] = E.x[E.s + 1];
                        N.cc = cc;
                        N.s = E.s + 1;
                        N.lcost = B;
                        N.rcost = rcost;
                        H.push(N);
                    }
                }

            delete [] E.x;
        }

        if (H.empty()) break;
        else {
            E = H.top();
            H.pop();
        }
    }

    if (bestc == NoEdge) return NoEdge;

    for (i = 1; i <= n; i++)
        printf("%d ",  E.x[i]);

    printf("\n");

    while (!H.empty()) H.pop();

    return bestc;
}

int main()
{
    int m;

    while (scanf("%d%d", &n, &m) != EOF) {
        int i, j;

        for (i = 0; i <= n; i++)
            for (j = 0; j <= n; j++)
                a[i][j] = inf;

        int from, to, length;

        for (i = 0; i < m; i++) {
            scanf("%d%d%d", &from, &to, &length);
            a[from][to] = length;
            a[to][from] = length;
        }

        cc = 0;
        bestc = 0;
        printf("%d\n", queueTSP());
    }
}
