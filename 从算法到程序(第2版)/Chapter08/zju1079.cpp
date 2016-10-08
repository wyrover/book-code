#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <iostream>
using namespace std;

int n, k;
int dx[4] = {1, 0, -1, 0};
int dy[4] = {0, 1, 0, -1};
int queue[501];
int used[500];

struct {
    int id;
    int side;
} piece[500][4];

struct position {
    int x, y;
    int id;
    int side;
} pos[500];

bool cmp(position a, position b)
{
    if (a.x == b.x) return a.y < b.y;
    else return a.x < b.x;
}

void bfs()
{
    int a, b, d;
    int i;

    for (i = 0; i < n; i++)
        pos[i].id = i;

    pos[0].x = 0;
    pos[0].y = 0;
    pos[0].side = 0;
    int first = 0;
    int count = 0;
    queue[0] = 0;
    used[0] = 1;

    while (first <= count) {
        a = queue[first];
        int x0 = pos[a].x;
        int y0 = pos[a].y;
        d = pos[a].side;
        int number;

        for (i = 0; i < 4; i++)
            if ((number = piece[a][i].id) >= 0)
                if (used[number] == 0) {
                    b = piece[a][i].side;
                    used[number] = 1;
                    pos[number].x = x0 + dx[(i - d) & 3];
                    pos[number].y = y0 + dy[(i - d) & 3];
                    pos[number].side = ((b - i + d + 2) & 3);
                    queue[++count] = number;
                }

        first++;
    }
}

int main()
{
    int i;
    int cases = 1;

    while (scanf("%d", &n) && n) {
        scanf("%d", &k);
        memset(piece, 255, sizeof(piece));
        memset(used, 0, sizeof(used));
        int a, b, c, d;

        for (i = 0; i < k; i++) {
            scanf("%d%d%d%d", &a, &b, &c, &d);
            piece[a][b].id = c;
            piece[a][b].side = d;
            piece[c][d].id = a;
            piece[c][d].side = b;
        }

        bfs();
        sort(pos, pos + n, cmp);
        a = pos[0].x;
        b = pos[0].y;
        printf("Instance %d:\n", cases++);

        for (i = 0; i < n; i++)
            printf("%5d%5d%6d%2d\n", pos[i].x - a, pos[i].y - b, pos[i].id, pos[i].side);
    }

    return 0;
}
