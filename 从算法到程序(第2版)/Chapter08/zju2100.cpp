#include<stdio.h>

#define NUM 10
char field[NUM][NUM];
int n, m;
int visited;
int flag;
int count;

void dfs(int x, int y)
{
    if (x < 0 || y < 0 || x >= n || y >= m) return;

    if (field[x][y] == 'S') return;

    if (flag) return;

    count ++;

    if (count > 1500) return;

    field[x][y] = 'S';
    visited ++;

    if (visited == n * m) {
        flag = 1;
        return;
    }

    dfs(x + 1, y);
    dfs(x - 1, y);
    dfs(x, y + 1);
    dfs(x, y - 1);
    visited --;
    field[x][y] = '.';
}

int main()
{
    while (scanf("%d%d", &n, &m) && n && m) {
        int i, j;

        for (i = 0; i < n; i++)
            scanf("%s", field[i]);

        visited = 0;

        for (i = 0; i < n ; i++)
            for (j = 0; j < m; j++)
                if (field[i][j] == 'S') visited ++;

        flag = 0;
        count = 0;
        dfs(0, 0);

        if (flag) printf("YES\n");
        else printf("NO\n");
    }

    return 0;
}
