#include <stdio.h>
#include <memory.h>

bool g[26][26];
int used[26];
int n;

bool dfs(int id, int color)
{
    int i, j;
    bool flag;

    for (i = 0; i < color; i++) {
        flag = true;
        used[id] = i;

        for (j = 0; j < id; j++)
            if (g[id][j] && used[id] == used[j]) {
                flag = false;
                break;
            }

        if (flag && (id == n - 1 || dfs(id + 1, color)))
            return true;
    }

    return false;
}

int main()
{
    int i, j;
    bool one;
    char adjacent[50];

    while (scanf("%d", &n) && n) {
        memset(g, 0, sizeof(g));
        memset(used, 0, sizeof(used));
        one = true;

        for (i = 0; i < n; i++)  {
            scanf("%s", adjacent);

            for (j = 2; adjacent[j]; j++, one = false) {
                g[i][adjacent[j] - 'A'] = true;
                g[adjacent[j] - 'A'][i] = true;
            }
        }

        if (one)
            printf("1 channel needed.\n");
        else if (dfs(1, 2))
            printf("2 channels needed.\n");
        else if (dfs(1, 3))
            printf("3 channels needed.\n");
        else
            printf("4 channels needed.\n");
    }

    return 0;
}
