#include <stdio.h>

#define NUM 1001
int a[NUM];
int MaxSum(int n)
{
    int sum = 0;
    int b = 0;

    for (int i = 1; i <= n; i++) {
        if (b > 0) b += a[i];
        else b = a[i];

        if (b > sum) sum = b;
    }

    return sum;
}

int main()
{
    int n;

    while (scanf("%d", &n) && n) {
        for (int i = 1; i <= n; i++)
            scanf("%d", &a[i]);

        printf("%d\n", MaxSum(n));
    }

    return 0;
}
