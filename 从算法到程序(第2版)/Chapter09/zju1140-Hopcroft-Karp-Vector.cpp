#include <iostream>
#include <cstdio>
#include <vector>
#include <queue>
#include <cstring>
using namespace std;

#define MAX 305
#define INF 0x7fffffff

int n, p;
int use[MAX];
int cx[MAX], cy[MAX];
int dx[MAX], dy[MAX];
vector<int>map[MAX];
queue<int>q;
bool dfs(int u)
{
    int len = map[u].size();

    for (int i = 0; i < len; i++) {
        int v = map[u][i];

        if (!use[v] && dy[v] == dx[u] + 1) {
            use[v] = 1;

            if (!cy[v] || dfs(cy[v])) {
                cx[u] = v;
                cy[v] = u;
                return true;
            }
        }
    }

    return false;
}
int Hopcroft_Karp()
{
    memset(cx, 0, sizeof(cx));
    memset(cy, 0, sizeof(cy));
    int course = 0;

    while (true) {
        int flag = 0;

        while (!q.empty()) q.pop();

        memset(dx, 0, sizeof(dx));
        memset(dy, 0, sizeof(dy));

        for (int i = 1; i <= p; i++)
            if (!cx[i]) q.push(i);

        while (!q.empty()) {
            int u = q.front();
            int len = map[u].size();
            q.pop();

            for (int i = 0; i < len; i++) {
                int v = map[u][i];

                if (!dy[v]) {
                    dy[v] = dx[u] + 1;

                    if (cy[v]) {
                        dx[cy[v]] = dy[v] + 1;
                        q.push(cy[v]);
                    } else flag = 1;
                }
            }
        }

        if (!flag) break;

        memset(use, 0, sizeof(use));

        for (int i = 1; i <= p; i++)
            if (!cx[i] && dfs(i)) course++;
    }

    return course;
}

int main()
{
    int iCase;
    scanf("%d", &iCase);

    while (iCase--) {
        scanf("%d%d", &p, &n);

        for (int i = 0; i <= p; i++)
            map[i].clear();

        int num, student;

        for (int u = 1; u <= p; u++) {
            scanf("%d", &num);

            while (num--) {
                scanf("%d", &student);
                map[u].push_back(student);
            }
        }

        puts(Hopcroft_Karp() == p ? "YES" : "NO");
    }

    return 0;
}
