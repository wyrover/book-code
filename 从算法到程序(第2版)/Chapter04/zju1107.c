#include <stdio.h>
#include <string.h>

int n, k;
int grid[105][105];
int mem[105][105];

int memSearch(int r, int c)
{
    int r1, c1, i;
    int max = 0;

    if (mem[r][c] != -1) return mem[r][c];

    for (i = 1; i <= k; i++) {
        r1 = r - i;

        if (r1 >= 1 && r1 <= n && grid[r][c] < grid[r1][c]) {
            mem[r1][c] = memSearch(r1, c);

            if (mem[r1][c] > max) max = mem[r1][c];
        }

        r1 = r + i;

        if (r1 <= n && r1 >= 1 && grid[r][c] < grid[r1][c]) {
            mem[r1][c] = memSearch(r1, c);

            if (mem[r1][c] > max) max = mem[r1][c];
        }

        c1 = c - i;

        if (c1 >= 1 && c1 <= n && grid[r][c] < grid[r][c1]) {
            mem[r][c1] = memSearch(r, c1);

            if (mem[r][c1] > max) max = mem[r][c1];
        }

        c1 = c + i;

        if (c1 <= n && c1 >= 1 && grid[r][c] < grid[r][c1]) {
            mem[r][c1] = memSearch(r, c1);

            if (mem[r][c1] > max) max = mem[r][c1];
        }
    }

    return max + grid[r][c];
}

int main()
{
    int i, j;

    while (scanf("%d%d", &n, &k)) {
        if (n == -1 && k == -1) break;

        for (i = 1; i <= n; i++)
            for (j = 1; j <= n; j++)
                scanf("%d", &grid[i][j]);

        memset(mem, -1, sizeof(mem));
        printf("%d\n", memSearch(1, 1));
    }

    return 0;
}
