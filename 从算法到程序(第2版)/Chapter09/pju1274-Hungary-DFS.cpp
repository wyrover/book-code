#include<stdio.h>
#include<string.h>

#define MAX 205
int n, m;
int map[MAX][MAX];
int match[MAX];
bool use[MAX];

bool dfs(int u)
{
    for (int i = 1; i <= m; i++)
        if (map[u][i] && !use[i]) {
            use[i] = true;
            int j = match[i];
            match[i] = u;

            if (j == -1 || dfs(j)) return true;

            match[i] = j;
        }

    return false;
}

int hungary()
{
    int num = 0;
    memset(match, -1, sizeof(match));

    for (int i = 1; i <= n; i++) {
        memset(use, false, sizeof(use));

        if (dfs(i)) num++;
    }

    return num;
}

int main()
{
    int si, stall;

    while (scanf("%d %d", &n, &m) != EOF) {
        memset(map, 0, sizeof(map));

        for (int i = 1; i <= n; i++) {
            scanf("%d", &si);

            for (int j = 1; j <= si; j++) {
                scanf("%d", &stall);
                map[i][stall] = 1;
            }
        }

        printf("%d\n", hungary());
    }

    return 0;
}
