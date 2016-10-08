#include<stdio.h>
#include <memory.h>

char s[30][30];
int flag[30][30];
int diagonal[4][2] = {{1, 1}, { -1, 1}, { -1, -1}, {1, -1}};
int x_y[4][2] = {{1, 0}, {0, 1}, { -1, 0}, {0, -1}};
int total;
int rows, cols;

void work(int x, int y)
{
    int i;
    int newx, newy;
    flag[x][y] = 1;

    for (i = 0; i < 4; i++) {
        newx = x + x_y[i][0];
        newy = y + x_y[i][1];

        if ((s[newx][newy] == 'X') && (flag[newx][newy] == 0))
            work(newx, newy);
        else if (s[newx][newy] == '.')
            total++;
    }

    for (i = 0; i < 4; i++) {
        newx = x + diagonal[i][0];
        newy = y + diagonal[i][1];

        if ((s[newx][newy] == 'X') && (flag[newx][newy] == 0))
            work(newx, newy);
    }
}

int main()
{
    while (scanf("%d", &rows) && rows) {
        int click_x, click_y;
        scanf("%d%d%d\rows", &cols, &click_x, &click_y);
        memset(s, '.', sizeof(s));
        memset(flag, 0, sizeof(flag));
        int i;

        for (i = 1; i <= rows; i++) {
            scanf("%s", s[i] + 1);
            s[i][cols + 1] = '.';
        }

        total = 0;
        work(click_x, click_y);
        printf("%d\n", total);
    }

    return 0;
}
