#include<cstdio>
#include<cstring>
#include<queue>
#include<iostream>
using namespace std;

#define inf 99999999
#define MAX 120
int cap[MAX][MAX];
int level[MAX];
int n, m, nc, np;

bool BFS()
{
    queue <int > q;
    memset(level, -1, sizeof(level));

    while (!q.empty()) q.pop();

    level[n] = 0;
    q.push(n);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v <= n + 1; v++) {
            if (cap[u][v] && level[v] == -1) {
                level[v] = level[u] + 1;
                q.push(v);
            }
        }
    }

    if (level[n + 1] > 0) return true;
    else return false;
}

int DFS(int u, int flow)
{
    if (u == n + 1) return flow;

    int aug;

    for (int v = 0; v <= n + 1; v++) {
        if (cap[u][v] && (level[v] == level[u] + 1)
            && (aug = DFS(v, min(cap[u][v], flow)))) {
            cap[u][v] -= aug;
            cap[v][u] += aug;
            return aug;
        }
    }

    return 0;
}

int Dinic()
{
    int maxflow = 0;

    while (BFS()) {
        int minflow;

        while (minflow = DFS(n, inf))  maxflow += minflow;
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

        printf("%d\n", Dinic());
    }
}
