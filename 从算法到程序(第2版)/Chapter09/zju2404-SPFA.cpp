#include <cstdio>
#include <string.h>
#include <cmath>
#include <queue>
#include <iostream>
using namespace std;

#define MAX 205
#define INF 20000
int nHouse, nMan, total;
int pre[MAX];
int cost[MAX][MAX];
int cap[MAX][MAX];
struct node {
    int x, y;
} house[MAX], man[MAX];

void make_map()
{
    int i, j;
    memset(cap, 0, sizeof(cap));

    for (i = 0; i < nMan; i++) {
        cap[0][i + 2] = 1;
        cost[0][i + 2] = 0;
    }

    for (i = 0; i < nHouse; i++) {
        cap[nMan + i + 2][1] = 1;
        cost[nMan + i + 2][1] = 0;
    }

    for (i = 0; i < nMan; i++)
        for (j = 0; j < nHouse; j++) {
            cap[i + 2][nMan + j + 2] = 1;
            cost[i + 2][nMan + j + 2] = abs(man[i].x - house[j].x)
                                        + abs(man[i].y - house[j].y);
            cost[nMan + j + 2][i + 2] = -cost[i + 2][nMan + j + 2];
        }
}

bool SPFA()
{
    int i;
    int d[MAX];
    bool vis[MAX];

    for (i = 1; i <= total; i++) {
        d[i] = INF;
        vis[i] = false;
    }

    d[0] = 0;
    queue <int> q;
    q.push(0);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (i = 1; i <= total; i++)
            if (cap[u][i] && d[i] > d[u] + cost[u][i]) {
                d[i] = d[u] + cost[u][i];
                pre[i] = u;

                if (!vis[i]) {
                    vis[i] = true;
                    q.push(i);
                }
            }

        vis[u] = false;
    }

    if (d[1] < INF) return true;

    return false;
}

int MinCost_MaxFlow()
{
    int result = 0;

    while (SPFA()) {
        int i;
        int minflow = INF;

        for (i = 1; i != 0; i = pre[i])
            minflow = min(minflow, cap[pre[i]][i]);

        for (i = 1; i != 0; i = pre[i]) {
            cap[pre[i]][i] -= minflow;
            cap[i][pre[i]] += minflow;
            result += cost[pre[i]][i] * minflow;
        }
    }

    return result;
}

int main()
{
    int n, m;

    while (scanf("%d%d", &n, &m), n && m) {
        char c;
        int i, j;
        nHouse = nMan = 0;

        for (i = 0; i < n; i++)
            for (j = 0; j < m; j++) {
                scanf(" %c", &c);

                if (c == 'H') {
                    house[nHouse].x = i;
                    house[nHouse].y = j;
                    nHouse++;
                } else if (c == 'm') {
                    man[nMan].x = i;
                    man[nMan].y = j;
                    nMan++;
                }
            }

        total = nHouse + nMan + 1;
        make_map();
        printf("%d\n", MinCost_MaxFlow());
    }

    return 0;
}
