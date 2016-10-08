#include<stdio.h>
#include<string.h>
#define NUM 101

int main()
{
    int a[NUM][NUM];
    int b[NUM];
    int n;
    int i, j, k;

    while (scanf("%d", &n) != EOF) {
        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++)
                scanf("%d", &a[i][j]);

        int max = -32767;

        for (i = 0; i < n; i++) {
            memset(b, 0, sizeof(b));

            for (j = i; j < n; j++) {
                int sum = 0;

                for (k = 0; k < n; k++) {
                    b[k] += a[j][k];
                    sum += b[k];

                    if (sum < b[k]) sum = b[k];

                    if (sum > max) max = sum;
                }
            }
        }

        printf("%d\n", max);
    }

    return 0;
}
