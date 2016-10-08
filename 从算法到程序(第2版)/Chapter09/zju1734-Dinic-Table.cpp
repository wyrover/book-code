#include<iostream>
#include<cstdio>
#include<cstring>
#include<queue>

using namespace std;

const int VM = 150;
const int EM = 20500;
const int INF = 0x3f3f3f3f;

struct Edge {
    int u, v, next;
    int cap;
} edge[EM];

int n, np, nc, m, cnt, head[VM];
int src, des, dep[VM];

void addedge(int cu, int cv, int cw)
{
    edge[cnt].u = cu;
    edge[cnt].v = cv;
    edge[cnt].cap = cw;
    edge[cnt].next = head[cu];
    head[cu] = cnt++;
    edge[cnt].v = cv;
    edge[cnt].v = cu;
    edge[cnt].cap = 0;
    edge[cnt].next = head[cv];
    head[cv] = cnt++;
}

int BFS()
{
    queue<int> q;

    while (!q.empty()) q.pop();

    memset(dep, -1, sizeof(dep));
    dep[src] = 0;
    q.push(src);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int i = head[u]; i != -1; i = edge[i].next) {
            int v = edge[i].v;

            if (edge[i].cap > 0 && dep[v] == -1) {
                dep[v] = dep[u] + 1;
                q.push(v);
            }
        }
    }

    if (dep[des] == -1) return 0;

    return 1;
}

int DFS(int u, int minflow)
{
    if (u == des) return minflow;

    int aug;

    for (int i = head[u]; i != -1; i = edge[i].next) {
        int v = edge[i].v;

        if (edge[i].cap > 0 && dep[v] == dep[u] + 1 && (aug = DFS(v, min(minflow, edge[i].cap)))) {
            edge[i].cap -= aug;
            edge[i ^ 1].cap += aug;
            return aug;
        }
    }

    dep[u] = -1;
    return 0;
}

int Dinic()
{
    int maxflow = 0;

    while (BFS()) {
        while (true) {
            int minflow = DFS(src, INF);

            if (minflow == 0) break;

            maxflow += minflow;
        }
    }

    return maxflow;
}

int main()
{
    while (scanf("%d%d%d%d", &n, &np, &nc, &m) != EOF) {
        cnt = 0;
        memset(head, -1, sizeof(head));
        src = n,  des = n + 1;
        int u, v, w;

        while (m--) {
            scanf(" (%d,%d)%d", &u, &v, &w);
            addedge(u, v, w);
        }

        while (np--) {
            scanf(" (%d)%d", &v, &w);
            addedge(src, v, w);
        }

        while (nc--) {
            scanf(" (%d)%d", &u, &w);
            addedge(u, des, w);
        }

        printf("%d\n", Dinic());
    }

    return 0;
}
