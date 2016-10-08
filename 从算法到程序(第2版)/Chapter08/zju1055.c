#include <stdio.h>
#include <memory.h>

int rows, cols;
unsigned int map[20][20];
unsigned int steps[20][20];
unsigned int times[20][20];
double factor[20][20];

struct location {
    int x, y;
} site[20],  queue[410];

int xOffset[4] = { -1, 1, 0, 0};
int yOffset[4] = {0, 0, -1, 1};

int siteValid(int x, int y)
{
    if (x >= 0 && x < rows && y >= 0 && y < cols) return steps[x][y];
    else return 500;
}

void loadFactors(int x, int y, double load)
{
    factor[x][y] += load * times[x][y];
    int point = steps[x][y];

    if ((point) <= 1) return;

    point--;
    int i;

    for (i = 0; i < 4; i++)
        if (siteValid(x + xOffset[i], y + yOffset[i]) == point)
            loadFactors(x + xOffset[i], y + yOffset[i], load);
}

void bfs_path(char c1, int dx, int dy)
{
    int i;
    int x0, y0, x1, y1;
    unsigned int bb;
    queue[0].x = site[c1].x;
    queue[0].y = site[c1].y;
    steps[queue[0].x][queue[0].y] = 0;
    times[queue[0].x][queue[0].y] = 1;
    int first = 0, last = 0;

    while (first <= last) {
        if (steps[dx][dy] < 500) break;

        x0 = queue[first].x;
        y0 = queue[first].y;
        bb = steps[x0][y0];
        bb++;

        for (i = 0; i < 4; i++) {
            x1 = x0 + xOffset[i];
            y1 = y0 + yOffset[i];
            int valid = 500;

            if (x1 >= 0 && x1 < rows && y1 >= 0 && y1 < cols)
                valid = map[x1][y1];

            if (valid == 0 && steps[x1][y1] > bb) {
                steps[x1][y1] = bb;
                last++;
                queue[last].x = x1;
                queue[last].y = y1;
            }
        }

        first++;
    }
}

void bfs_times()
{
    int i;
    int x0, y0, x1, y1;
    unsigned int bb;
    int first = 0, last = 0;

    while (first <= last) {
        x0 = queue[first].x;
        y0 = queue[first].y;
        bb = times[x0][y0];
        int temStep = steps[x0][y0];
        temStep++;

        for (i = 0; i < 4; i++) {
            x1 = x0 + xOffset[i];
            y1 = y0 + yOffset[i];

            if (siteValid(x1, y1) == temStep) {
                if (times[x1][y1] == 0) {
                    last++;
                    queue[last].x = x1;
                    queue[last].y = y1;
                }

                times[x1][y1] += bb;
            }
        }

        first++;
    }
}

int main()
{
    int i, j;
    char c;
    char temp[20];
    scanf("%d%d\n", &cols, &rows);

    for (i = 0; i < rows; i++) {
        gets(temp);

        for (j = 0; j < cols; j++) {
            c = temp[j];

            if (c == '.') map[i][j] = 0;
            else {
                map[i][j] = c - '@';

                if (c != 'X') {
                    c -= 'A';
                    site[c].x = i;
                    site[c].y = j;
                }
            }
        }
    }

    memset(factor, 0, sizeof(factor));
    char c1, c2;
    double load;

    while (1) {
        scanf("%c%c%lf\n", &c1, &c2, &load);

        if (c1 == 'X' && c2 == 'X') break;

        memset(steps, 255, sizeof(steps));
        memset(times, 0, sizeof(times));
        c1 -= 'A';
        c2 -= 'A';
        int dx = site[c2].x;
        int dy = site[c2].y;
        map[dx][dy] = 0;
        bfs_path(c1, dx, dy);
        bfs_times();
        map[dx][dy] = (++c2);
        load /= times[dx][dy];
        loadFactors(dx, dy, load);
    }

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (map[i][j]) factor[i][j] = 0;

            if (j) printf(" ");

            printf("%6.2lf", factor[i][j]);
        }

        printf("\n");
    }

    return (0);
}
