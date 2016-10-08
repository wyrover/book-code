#include <stdio.h>

int n;
int L, C;
int time[1001];
int minLec[1001];
int minDis[1001];

int DI(int t)
{
    if (t == 0) return 0;

    if (t <= 10) return -C;

    return (t - 10) * (t - 10);
}

void  DP()
{
    int  i, j;
    int  cost;
    int  sum;
    minLec[0] = 0;
    minDis[0] = 0;

    for (i = 1; i <= n; i ++) {
        minLec[i] = 30000;
        sum = 0;

        for (j = i - 1; j >= 0; j --) {
            sum += time[j + 1];

            if (sum > L) break;

            cost = minDis[j] + DI(L - sum);

            if (minLec[j] + 1 == minLec[i] && cost >= minDis[i]) continue;

            minLec[i] = minLec[j] + 1;
            minDis[i] = cost;
        }
    }
}

int main()
{
    int  i;
    int  N;
    scanf("%d", &N);

    while (N--) {
        int cases = 0;

        while (scanf("%d", &n) && n) {
            if (cases++) printf("\n");

            scanf("%d %d", &L, &C);

            for (i = 1; i <= n; i ++)
                scanf("%d", &time[i]);

            printf("Case %d:\n\n", cases);
            DP();
            printf("Minimum number of lectures: %d\n", minLec[n]);
            printf("Total dissatisfaction index: %d\n", minDis[n]);
        }

        if (N) printf("\n");
    }
}