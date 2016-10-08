#include <iostream>
using namespace std;

#define NUM 1000
#define maxint 10000000

void  Prim(int n, int c[][NUM])
{
    int lowcost[NUM];
    int closest[NUM];
    bool s[NUM] = {false};

    for (int i = 1; i <= n; i++) {
        lowcost[i] = c[1][i];
        closest[i] = 1;
        s[i] = false;
    }

    s[1] = true;

    for (int i = 1; i < n; i++) {
        int  min = maxint;
        int  j = 1;

        for (int k = 2; k <= n; k++)
            if ((lowcost[k] < min) && (!s[k])) {
                min = lowcost[k];
                j = k;
            }

        printf("%d %d\n", closest[j], j);
        s[j] = true;

        for (int k = 2; k <= n; k++)
            if ((c[j][k] < lowcost[k]) && (!s[k])) {
                lowcost[k] = c[j][k];
                closest[k] = j;
            }
    }
}

int c[NUM][NUM];
int main()
{
    int n, m;

    while (scanf("%d%d", &n, &m) != EOF && n) {
        int i, j;

        for (i = 0; i <= n; i++)
            for (j = 0; j <= n; j++)
                c[i][j] = maxint;

        int v, w, edge;

        for (i = 0; i < m; i++) {
            scanf("%d%d%d", &v, &w, &edge);
            c[v][w] = edge;
            c[w][v] = edge;
        }

        Prim(n, c);
        printf("\n");
    }

    return 0;
}

