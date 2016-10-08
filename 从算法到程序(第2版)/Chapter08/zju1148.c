#include <stdio.h>
#include <memory.h>

struct {
    int x, y;
} q[3000];

char g[80][80];
int steps[80][80];
int dir[4][2] = {{ -1, 0}, {1, 0}, {0, -1}, {0, 1}};

int main()
{
    int i;
    int n = 1;
    int sx, sy, dx, dy;
    int h, w;

    while (scanf("%d%d", &w, &h) && (h || w)) {
        getchar();
        int m = 1;

        for (i = 1; i <= h; i++)
            gets(&g[i][1]);

        for (i = 0; i < w + 2; i++)
            g[0][i] = g[h + 1][i] = ' ';

        for (i = 1; i <= h; i++)
            g[i][0] = g[i][w + 1] = ' ';

        printf("Board #%d:\n", n++);

        while (scanf("%d%d%d%d", &sy, &sx, &dy, &dx)) {
            if (!(sx || sy || dx || dy)) break;

            memset(steps, -1, sizeof(steps));
            g[sx][sy] = g[dx][dy] = ' ';
            steps[sx][sy] = 0;
            int first = 0;
            int last = 0;
            int x0, y0;
            q[first].x = sx;
            q[first].y = sy;

            while (first <= last && steps[dx][dy] == -1) {
                for (i = 0; i < 4; i++) {
                    x0 = q[first].x + dir[i][0];
                    y0 = q[first].y + dir[i][1];

                    while (x0 >= 0 && x0 <= h + 1 && y0 >= 0 && y0 <= w + 1
                           && g[x0][y0] == ' ' && steps[x0][y0] == -1) {
                        last++;
                        q[last].x = x0;
                        q[last].y = y0;
                        steps[x0][y0] = steps[q[first].x][q[first].y] + 1;
                        x0 += dir[i][0];
                        y0 += dir[i][1];
                    }
                }

                first++;
            }

            g[sx][sy] = g[dx][dy] = 'X';
            printf("Pair %d: ", m++);

            if (steps[dx][dy] == -1)
                printf("impossible.\n");
            else
                printf("%d segments.\n", steps[dx][dy]);
        }

        printf("\n");
    }

    return 0;
}
