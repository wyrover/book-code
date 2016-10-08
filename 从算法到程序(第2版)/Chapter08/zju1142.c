#include<stdio.h>
#include<string.h>

#define N 160
char wall[N * N][4];
char used[N * N];
int next[N * N][4];
int path[N * N];
int n;
int h, w;

void side(char maze[][N])
{
    memset(wall, 0, sizeof(wall));
    int k = 0;
    int i, j;

    for (i = 0; i < 2 * h - 1; i++)
        for (j = 0; j < w; j++, k++) {
            if (i % 2 == 0 && j == w - 1) break;

            if (i == 0) {
                if (maze[0][j] == '/') wall[k][3] = 1;

                if (maze[0][j + 1] != '/') wall[k][2] = 1;

                next[k][3] = w - 1 + j;
                next[k][2] = w + j;
            } else if (i == 2 * h - 2) {
                if (maze[h - 1][j] != '/') wall[k][0] = 1;

                if (maze[h - 1][j + 1] == '/') wall[k][1] = 1;

                next[k][0] = n - (w + w - 1) + j;
                next[k][1] = n - (w + w - 1) + j + 1;
            } else {
                if (i % 2 == 0) {
                    if (maze[i / 2][j] == '/') wall[k][3] = 1;
                    else wall[k][0] = 1;

                    if (maze[i / 2][j + 1] != '/') wall[k][2] = 1;
                    else wall[k][1] = 1;

                    next[k][0] = k - w;
                    next[k][1] = k - w + 1;
                    next[k][2] = k + w;
                    next[k][3] = k + w - 1;
                } else {
                    if (maze[i / 2][j] == '/') wall[k][1] = 1;
                    else wall[k][0] = 1;

                    if (maze[i / 2 + 1][j] == '/') wall[k][3] = 1;
                    else wall[k][2] = 1;

                    next[k][0] = k - w;
                    next[k][1] = k - w + 1;
                    next[k][2] = k + w;
                    next[k][3] = k + w - 1;
                }
            }

            if (j == 0 && i % 2) wall[k][0] = wall[k][3] = 0;

            if (i % 2 && j == w - 1)
                wall[k][2] = wall[k][1] = 0;
        }
}

char dfs(int i, int length, int start)
{
    path[i] = length;

    if (i == start && length > 2) return 1;

    if (used[i]) return 0;

    used[i] = 1;

    if (wall[i][0] && dfs(next[i][0], length + 1, start)) return 1;

    if (wall[i][1] && dfs(next[i][1], length + 1, start)) return 1;

    if (wall[i][2] && dfs(next[i][2], length + 1, start)) return 1;

    if (wall[i][3] && dfs(next[i][3], length + 1, start)) return 1;

    return 0;
}

int main()
{
    int i;
    int iCase = 1;
    char maze[N][N];

    while (scanf("%d %d", &w, &h) && w) {
        for (i = 0; i < h; i++)
            scanf("%s", maze[i]);

        n = (h - 1) * (w + w - 1) + w - 1;
        side(maze);
        memset(used, 0, sizeof(used));
        int total = 0;
        int max = 0;
        memset(path, 0, sizeof(path));

        for (i = 0; i < n; i++)
            if (!used[i]) {
                if (dfs(i, 0, i)) {
                    if (path[i] > max) max = path[i];

                    total++;
                }

                used[i] = 1;
            }

        printf("Maze #%d:\n", iCase++);

        if (total > 0)
            printf("%d Cycles; the longest has length %d.\n", total, max);
        else
            printf("There are no cycles.\n");

        printf("\n");
    }

    return 0;
}
