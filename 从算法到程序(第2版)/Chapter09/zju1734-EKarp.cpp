#include <cstdio>
#include <iostream>
#include <string.h>
#include <queue>
using namespace std;

#define MAX 120
int n;
int np;
int nc;
int m;
int cap[MAX][MAX];

int EKarp(int s, int t)
{
    queue<int> Q;
    int flow[MAX][MAX];
    int pre[MAX];
    int node[MAX];
    int u, v;
    int maxflow = 0;
    memset(flow, 0, sizeof(flow));

    while (true) {
        Q.push(s);
        memset(node, 0, sizeof(node));
        node[s] = 100000;

        while (!Q.empty()) {
            u = Q.front();
            Q.pop();

            for (v = 0; v <= t; v++)
                if (!node[v] && cap[u][v] > flow[u][v]) {
                    Q.push(v);
                    node[v] = min(node[u], cap[u][v] - flow[u][v]);
                    pre[v] = u;
                }
        }

        if (node[t] == 0)   break;

        for (u = t; u != s; u = pre[u]) {
            flow[pre[u]][u] += node[t];
            flow[u][pre[u]] -= node[t];
        }

        maxflow += node[t];
    }

    return maxflow;
}

int main()
{
    int from, to, value;

    while (scanf("%d%d%d%d", &n, &np, &nc, &m) != EOF) {
        memset(cap, 0, sizeof(cap));

        while (m--) {
            scanf(" (%d,%d)%d", &from, &to, &value);
            cap[from][to] = value;
        }

        while (np--) {
            scanf(" (%d)%d", &from, &value);
            cap[n][from] = value;
        }

        while (nc--) {
            scanf(" (%d)%d", &from, &value);
            cap[from][n + 1] = value;
        }

        printf("%d\n", EKarp(n, n + 1));
    }

    return 0;
}
