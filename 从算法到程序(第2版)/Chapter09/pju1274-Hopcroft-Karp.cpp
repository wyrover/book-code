#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;

const int MAX = 201;
const int INF = INT_MAX;
struct EDGE {
    int b;
    int next;
} edge[2000];

int n, m;
int pre[MAX];
int cx[MAX], cy[MAX];
int dx[MAX], dy[MAX];
int q[MAX];

bool BFS()
{
    int i, j, k;
    bool flag(false);
    int qs, qe;
    memset(dx, 0, sizeof(dx));
    memset(dy, 0, sizeof(dy));
    qs = qe = 0;

    for (i = 1; i <= n; ++i)
        if (cx[i] == -1) q[qe++] = i;

    while (qs < qe) {
        i = q[qs++];

        for (k = pre[i]; k != -1; k = edge[k].next) {
            j = edge[k].b;

            if (!dy[j]) {
                dy[j] = dx[i] + 1;

                if (cy[j] == -1) flag = true;
                else {
                    dx[cy[j]] = dy[j] + 1;
                    q[qe++] = cy[j];
                }
            }
        }
    }

    return flag;
}

bool DFS(int i)
{
    for (int k = pre[i]; k != -1; k = edge[k].next) {
        int j = edge[k].b;

        if (dy[j] == dx[i] + 1) {
            dy[j] = 0;

            if (cy[j] == -1 || DFS(cy[j])) {
                cx[i] = j;
                cy[j] = i;
                return true;
            }
        }
    }

    return false;
}

int Hopcroft_Karp()
{
    int num = 0;;

    while (BFS())
        for (int i = 1; i <= n; ++i)
            if (cx[i] == -1 && DFS(i)) ++num;

    return num;
}

int main()
{
    while (scanf("%d%d", &n, &m) != EOF) {
        memset(cx, -1, sizeof(cx));
        memset(cy, -1, sizeof(cy));
        memset(pre, -1, sizeof(pre));
        int si, stall;
        int edgeNum = 0;
        int v;

        for (int u = 1; u <= n; ++u) {
            scanf("%d", &si);

            for (stall = 0; stall < si; ++stall) {
                scanf("%d", &v);
                edge[edgeNum].b = v;
                edge[edgeNum].next = pre[u];
                pre[u] = edgeNum++;
            }
        }

        printf("%d\n", Hopcroft_Karp());
    }

    return 0;
}
