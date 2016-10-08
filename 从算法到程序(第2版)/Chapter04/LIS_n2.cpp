#include <stdio.h>

#define NUM 100
int a[NUM];
int LIS_n2(int n)
{
    int b[NUM] = {0};
    int i, j;
    b[1] = 1;
    int max = 0;

    for (i = 2; i <= n; i++) {
        int k = 0;

        for (j = 1; j < i; j++)
            if (a[j] <= a[i] && k < b[j]) k = b[j];

        b[i] = k + 1;

        if (max < b[i]) max = b[i];
    }

    return max;
}

int main()
{
    int n;

    while (scanf("%d", &n) && n) {
        for (int i = 1; i <= n; i++)
            scanf("%d", &a[i]);

        printf("%d\n", LIS_n2(n));
    }
}
