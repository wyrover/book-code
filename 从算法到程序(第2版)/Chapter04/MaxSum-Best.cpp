#include <stdio.h>

#define NUM 1001
int a[NUM];
int MaxSum(int n, int &besti, int &bestj)
{
    int sum = 0;
    int b = 0;
    int begin = 0;

    for (int i = 1; i <= n; i++) {
        if (b > 0) b += a[i];
        else {
            b = a[i];
            begin = i;
        }

        if (b > sum) {
            sum = b;
            besti = begin;
            bestj = i;
        }
    }

    return sum;
}

int main()
{
    int n;
    int besti, bestj;

    while (scanf("%d", &n) && n) {
        besti = 0;
        bestj = 0;

        for (int i = 1; i <= n; i++)
            scanf("%d", &a[i]);

        printf("%d\n", MaxSum(n, besti, bestj));
        printf("From %d to %d\n", besti, bestj);
    }

    return 0;
}
