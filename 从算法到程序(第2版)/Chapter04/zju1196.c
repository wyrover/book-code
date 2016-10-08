#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    int i, j, m;
    int d[202];
    int n, k;
    int cost[202][202];
    int dis[205][205];
    int temp;
    int number = 1;

    while (scanf("%d %d", &n, &k) && (n || k)) {
        for (i = 1; i <= n; i ++)
            scanf("%d", &d[i]);

        for (i = 1; i <= n; i++)
            for (j = 1; j <= n; j++) {
                temp = (i + j) / 2;
                cost[i][j] = 0;

                for (m = i; m <= j; m++)
                    cost[i][j] += abs(d[m] - d[temp]);
            }

        memset(dis, 1, sizeof(dis));

        for (i = 1; i <= n; i++)
            dis[1][i] = cost[1][i];

        for (i = 2; i <= k; i++)
            for (j = i; j <= n; j++)
                for (m = i - 1; m < j; m++) {
                    temp = dis[i - 1][m] + cost[m + 1][j];

                    if (temp < dis[i][j])
                        dis[i][j] = temp;
                }

        printf("Chain %d\nTotal distance sum = %d\n\n", number++, dis[k][n]);
    }

    return 0;
}
