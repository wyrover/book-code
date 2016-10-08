#include <iostream>
#include <stdio.h>
using namespace std;

int grid[101][101];
int cheese[101][101];
int n, k;
int d[4][2] = {{1, 0}, { -1, 0}, {0, -1}, {0, 1}};

int memSearch(int x, int y)
{
    int i, j;
    int max = 0;

    if (cheese[x][y] > 0) return cheese[x][y];

    for (i = 0; i < 4; i++)
        for (j = 1; j <= k; j++) {
            int tx = x + d[i][0] * j;
            int ty = y + d[i][1] * j;

            if (tx >= 0 && tx < n && ty >= 0 && ty < n
                && grid[x][y] < grid[tx][ty]) {
                int temp = memSearch(tx, ty);

                if (max < temp) max = temp;
            }
        }

    cheese[x][y] = max + grid[x][y];
    return cheese[x][y];
}

int main()
{
    int i, j;

    while (scanf("%d%d", &n, &k)) {
        if (n == -1 && k == -1) break;

        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++) {
                scanf("%d", &grid[i][j]);
                cheese[i][j] = 0;
            }

        memSearch(0, 0);
        printf("%d\n", cheese[0][0]);
    }

    return 0;
}