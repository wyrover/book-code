#include <stdio.h>
#include <string.h>

#define MIN -99999999

int opt[10001][10][10];
int cost[1000];
int n;

void DP()
{
    int i, j, k, p;
    int temp;

    for (k = 1; k <= 2; k++)
        for (i = 0; i < 10; i++)
            for (j = 0; j < 10; j++)
                opt[k][i][j] = 0;

    for (k = 3; k <= n; k++)
        for (i = 0; i < 10; i++)
            for (j = 0; j < 10; j++) {
                opt[k][i][j] = MIN;

                for (p = 0; p < 10; p++) {
                    temp = opt[k - 1][j][p] + cost[i * 100 + j * 10 + p];

                    if (opt[k][i][j] < temp)
                        opt[k][i][j] = temp;
                }
            }
}

void print()
{
    int i, j, k, p;
    int m = MIN;

    for (i = 0; i < 10; i++)
        for (j = 0; j < 10; j++)
            if (m < opt[n][i][j]) {
                m = opt[n][i][j];
                k = i;
                p = j;
            }

    for (i = n; i >= 3; i--)
        for (j = 0; j < 10; j++)
            if (opt[i - 1][p][j] + cost[k * 100 + p * 10 + j] == m) {
                m -= cost[k * 100 + p * 10 + j];
                printf("%d", k);
                k = p;
                p = j;
                break;
            }

    printf("%d%d\n", k, p);
}

int main()
{
    int i;
    int m;
    int banker, point;

    while (scanf("%d%d", &m, &n) != EOF) {
        memset(cost, 0, sizeof(cost));

        for (i = 0; i < m; i ++) {
            scanf("%d%d", &banker, &point);
            cost[banker] = point;
        }

        DP();

        if (n == 1) printf("0\n");
        else if (n == 2) printf("00\n");
        else print();
    }

    return 0;
}
