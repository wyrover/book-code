#include <stdio.h>
#include <memory.h>
#include <iostream>
using namespace std;

int n;
char g[51][51];
int step[51][51][51];
struct str {
    char a, b, c;
} list[51 * 51 * 51];

bool bfs(int &a, int &b, int &c)
{
    int i;
    int first = 0;
    int last = 0;
    list[0].a = a;
    list[0].b = b;
    list[0].c = c;
    step[a][b][c] = 0;
    bool flag = false;

    while (first <= last) {
        a = list[first].a;
        b = list[first].b;
        c = list[first].c;

        if ((a == b) && (a == c)) {
            flag = true;
            break;
        }

        unsigned int current = step[a][b][c];
        current++;
        char bcColour = g[b][c];
        char *colour = g[a];
        colour++;

        for (i = 1; i <= n; i++, colour++)
            if (i != a && *colour == bcColour && step[i][b][c] > current) {
                step[i][b][c] = current;
                last++;
                list[last].a = i;
                list[last].b = b;
                list[last].c = c;
            }

        bcColour = g[a][c];
        colour = g[b], colour++;

        for (i = 1; i <= n; i++, colour++)
            if (i != b && *colour == bcColour && step[a][i][c] > current) {
                step[a][i][c] = current;
                last++;
                list[last].a = a;
                list[last].b = i;
                list[last].c = c;
            }

        bcColour = g[a][b];
        colour = g[c], colour++;

        for (i = 1; i <= n; i++, colour++)
            if (i != c && *colour == bcColour && step[a][b][i] > current) {
                step[a][b][i] = current;
                last++;
                list[last].a = a;
                list[last].b = b;
                list[last].c = i;
            }

        first++;
    }

    return flag;
}

int main()
{
    int i, j;
    int a, b, c;

    while (scanf("%d", &n) && n) {
        scanf("%d%d%d\n", &a, &b, &c);
        memset(step, 255, sizeof(step));
        char arrow;

        for (i = 1; i <= n; i++)
            for (j = 1; j <= n; j++) {
                scanf("%c ", &arrow);
                g[i][j] = arrow;
            }

        if (bfs(a, b, c)) printf("%d\n", step[a][b][c]);
        else printf("impossible\n");
    }

    return 0;
}
