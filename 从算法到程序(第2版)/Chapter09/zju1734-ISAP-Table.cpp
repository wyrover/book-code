#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

#define inf 0x7fffffff
#define N 300
#define M 30000
int CurrentArc[N];
int pre[N];
int gap[N];
int dis[N];
int head[N];
int size;

struct Edge {
    int v, w, next;
    Edge() {}
    Edge(int V, int W, int NEXT): v(V), w(W), next(NEXT) {}
} edge[M];

void InsertEdge(int u, int v, int w)
{
    edge[size] = Edge(v, w, head[u]);
    head[u] = size++;
    edge[size] = Edge(u, 0, head[v]);
    head[v] = size++;
}

int ISAP(int st, int ed, int n)
{
    for (int i = 0; i <= n; i++) {
        CurrentArc[i] = head[i];
        gap[i] = dis[i] = 0;
    }

    int u = pre[st] = st;
    int minflow = inf;
    int maxflow = 0;

    while (dis[st] < n) {
        bool relabel = false;

        for (int &i = CurrentArc[u]; i != -1; i = edge[i].next) {
            int v = edge[i].v;

            if (edge[i].w && dis[u] == dis[v] + 1) {
                minflow = min(minflow, edge[i].w);
                pre[v] = u;
                u = v;

                if (v == ed) {
                    maxflow += minflow;

                    for (u = pre[u]; v != st; v = u, u = pre[u]) {
                        edge[CurrentArc[u]].w -= minflow;
                        edge[CurrentArc[u] ^ 1].w += minflow;
                    }

                    minflow = inf;
                }

                relabel = true;
            }

            if (relabel) break;
        }

        if (relabel) continue;

        int mindis = n;

        for (int i = head[u]; i != -1; i = edge[i].next) {
            int v = edge[i].v;

            if (edge[i].w && dis[v] < mindis) {
                CurrentArc[u] = i;
                mindis = dis[v];
            }
        }

        if (--gap[dis[u]] == 0) break;

        gap[dis[u] = mindis + 1]++;
        u = pre[u];
    }

    return maxflow;
}

int main()
{
    int n, np, nc, m;
    int st, ed, nv;
    int u, v, w;

    while (scanf("%d%d%d%d", &n, &np, &nc, &m) != EOF) {
        size = 0;
        memset(head, -1, sizeof(head));
        nv = n + 2;
        st = n;
        ed = n + 1;

        for (int i = 1; i <= m; i++) {
            scanf(" (%d,%d)%d", &u, &v, &w);
            InsertEdge(u, v, w);
        }

        for (int i = 1; i <= np; i++) {
            scanf(" (%d)%d", &v, &w);
            InsertEdge(st, v, w);
        }

        for (int i = 1; i <= nc; i++) {
            scanf(" (%d)%d", &u, &w);
            InsertEdge(u, ed, w);
        }

        printf("%d\n", ISAP(st, ed, nv));
    }

    return 0;
}
