#include<cstdio>
#include<cstring>
using namespace std;

#define MAX 120
int n, np, nc, m;
int cap[MAX][MAX];

int ISAP(int s, int t)
{
    int CurrentArc[MAX];
    int level[MAX];
    int gap[MAX];
    int pre[MAX];
    memset(CurrentArc, 0, sizeof CurrentArc);
    memset(level, 0, sizeof level);
    memset(gap, 0, sizeof gap);
    int u = pre[s] = s;
    int v;
    int maxFlow = 0;
    int minFlow = 100000;
    n += 2;
    gap[s] = t;

    while (level[s] < n) {
        for (v = CurrentArc[u]; v < n; v++)
            if (cap[u][v] > 0 && level[u] == level[v] + 1)
                break;

        if (v < n) {
            pre[v] = u;

            if (minFlow > cap[u][v]) minFlow = cap[u][v];

            u = CurrentArc[u] = v;

            if (u == t) {
                maxFlow += minFlow;

                for (v = t; v != s; v = pre[v]) {
                    cap[pre[v]][v] -= minFlow;
                    cap[v][pre[v]] += minFlow;
                }

                minFlow = 100000;
                u = s;
            }
        } else {
            int minLabel = n;

            for (v = 0; v < n; v++) {
                if (cap[u][v] > 0 && minLabel > level[v]) {
                    CurrentArc[u] = v;
                    minLabel = level[v];
                }
            }

            if (--gap[level[u]] == 0) return maxFlow;

            level[u] = minLabel + 1;
            gap[level[u]] ++;
            u = pre[u];
        }
    }

    return maxFlow;
}
int main()
{
    int from, to, value;

    while (scanf("%d%d%d%d", &n, &np, &nc, &m) != EOF) {
        memset(cap, 0, sizeof cap);

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

        printf("%d\n", ISAP(n, n + 1));
    }

    return 0;
}
