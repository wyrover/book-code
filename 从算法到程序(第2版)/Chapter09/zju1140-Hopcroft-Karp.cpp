#include <cstdio>
#include <memory.h>
#include <queue>
using namespace std;

const int MAX = 310;
const int INF = 1 << 8;
int p, n;
int Cx[MAX], Cy[MAX];
int dx[MAX], dy[MAX];
int dis;
bool use[MAX];
bool map[110][MAX];

bool BFS(void)
{
    queue <int> q;
    dis = INF;
    memset(dx, -1, sizeof(dx));
    memset(dy, -1, sizeof(dy));

    for (int i = 1; i <= p; i++)
        if (Cx[i] == -1) {
            q.push(i);
            dx[i] = 0;
        }

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        if (dx[u] > dis) break;

        for (int v = 1; v <= n; v++)
            if (map[u][v] && dy[v] == -1) {
                dy[v] = dx[u] + 1;

                if (Cy[v] == -1) dis = dy[v];
                else {
                    dx[Cy[v]] = dy[v] + 1;
                    q.push(Cy[v]);
                }
            }
    }

    return dis != INF;
}

bool DFS(int u)
{
    for (int v = 1; v <= n; v++)
        if (!use[v] && map[u][v] && dy[v] == dx[u] + 1) {
            use[v] = 1;

            if (Cy[v] != -1 && dy[v] == dis) continue;

            if (Cy[v] == -1 || DFS(Cy[v])) {
                Cy[v] = u;
                Cx[u] = v;
                return true;
            }
        }

    return false;
}

int Hopcroft_Karp(void)
{
    int course = 0;
    memset(Cx, -1, sizeof(Cx));
    memset(Cy, -1, sizeof(Cy));

    while (BFS()) {
        memset(use, 0, sizeof(use));

        for (int i = 1; i <= p; i++)
            if (Cx[i] == -1 && DFS(i)) course++;
    }

    return course;
}

int main()
{
    int iCase;
    scanf("%d", &iCase);

    while (iCase--) {
        memset(map, 0, sizeof(map));
        int count, student;
        scanf("%d %d", &p, &n);

        for (int i = 1; i <= p; i++) {
            scanf("%d", &count);

            while (count--) {
                scanf("%d", &student);
                map[i][student] = true;
            }
        }

        printf("%s\n", Hopcroft_Karp() == p ? "YES" : "NO");
    }

    return 0;
}
