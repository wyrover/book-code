#include<iostream>
#include<cstdio>
#include<cstring>
#include<vector>
#include<cmath>
using namespace std;
#define INF 1000000
#define MAX 105

int lx[MAX], ly[MAX];
int cx[MAX], cy[MAX];
bool visx[MAX], visy[MAX];
int w[MAX][MAX];
int slack;
int n;

struct node {
    int x, y;
    node(int x0, int y0)
    {
        x = x0;
        y = y0;
    }
};

bool findPath(int x)
{
    int temp;
    visx[x] = true;

    for (int y = 1; y <= n; y++) {
        if (visy[y])continue;

        temp = w[x][y] - lx[x] - ly[y];

        if (temp == 0) {
            visy[y] = true;

            if (!cy[y] || findPath(cy[y])) {
                cx[x] = y;
                cy[y] = x;
                return true;
            }
        } else if (slack > temp) slack = temp;
    }

    return false;
}

int Kuhn_Munkres()
{
    memset(cx, 0, sizeof(cx));
    memset(cy, 0, sizeof(cy));
    memset(ly, 0, sizeof(ly));

    for (int i = 0; i <= n; i++)
        lx[i] = INF;

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            if (lx[i] > w[i][j])
                lx[i] = w[i][j];

    for (int x = 1; x <= n; x++) {
        while (true) {
            memset(visx, 0, sizeof(visx));
            memset(visy, 0, sizeof(visy));
            slack = INF;

            if (findPath(x)) break;

            for (int i = 1; i <= n; i++) {
                if (visx[i]) lx[i] += slack;;

                if (visy[i]) ly[i] -= slack;
            }
        }
    }

    int result = 0;

    for (int y = 1; y <= n; y++)
        result += w[cy[y]][y];

    return result;
}

int main()
{
    int row, col;
    char grid[MAX][MAX];

    while (scanf("%d%d", &row, &col), row && col) {
        n = 0;
        vector<node> home, man;

        for (int i = 0; i < row; i++) {
            scanf("%s", grid[i]);

            for (int j = 0; j < col; j++) {
                if (grid[i][j] == 'm') {
                    man.push_back(node(i, j));
                    ++n;
                }

                if (grid[i][j] == 'H')
                    home.push_back(node(i, j));
            }
        }

        for (int i = 0; i < man.size(); i++)
            for (int j = 0; j < home.size(); j++)
                w[i + 1][j + 1] = abs(man[i].x - home[j].x)
                                  + abs(man[i].y - home[j].y);

        printf("%d\n", Kuhn_Munkres());
    }

    return 0;
}
