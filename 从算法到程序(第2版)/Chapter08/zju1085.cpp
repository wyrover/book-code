#include <stdio.h>
#include <string.h>
#include <iostream>
#include <queue>
using namespace std;

#define NUM 100
int n;
int et;
bool used[NUM];
bool data[NUM][NUM];
int dis[NUM];

void bfs_path()
{
    queue<int> q;
    q.push(et);
    dis[et] = 0;
    int x;

    while (!q.empty()) {
        x = q.front();
        q.pop();

        for (int i = 0; i < n; ++i)
            if (data[i][x] && dis[x] + 1 < dis[i]) {
                q.push(i);
                dis[i] = dis[x] + 1;
            }
    }
}

int dfs_search(int id)
{
    if (id == et) return 1;

    used[id] = 1;

    for (int i = 0; i < n; ++i)
        if (!used[i] && data[id][i])
            if (dfs_search(i)) return 1;

    return 0;
}

int main()
{
    int cases;
    int room;
    scanf("%d", &cases);

    for (int t = 0; t < cases; ++t) {
        scanf("%d%d\n", &n, &et);
        memset(data, 0, sizeof(data));

        for (int i = 0; i < NUM; ++i)
            dis[i] = 100000;

        char line[10];
        int a, b;

        while (gets(line)) {
            if (strcmp(line, "") == 0) break;

            sscanf(line, "%d%d", &a, &b);
            data[a][b] = true;
        }

        bfs_path();
        int d = dis[0];
        room = 0;

        for (int i = 1; i < n; ++i) {
            if (i == et) continue;

            memset(used, 0, sizeof(used));
            used[i] = 1;

            if (!dfs_search(0) && dis[i] < d) {
                room = i;
                d = dis[i];
            }
        }

        if (t) cout << endl;

        cout << "Put guards in room " << room << "." << endl;
    }

    return 0;
}
