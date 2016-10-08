#include <stdio.h>
#include <memory.h>

int a[7];
int b[100];
int dp[60000];

int multiplePack()
{
    int n = 0;
    int k;
    int x;
    int i;

    for (i = 1; i <= 6; i++) {
        k = 0;
        b[n++] = (k++) * i;
        x = 0;

        while (k < a[i]) {
            b[n++] = k * i;
            x += k;
            k *= 2;
        }

        if (a[i] > x) b[n++] = (a[i] - x) * i;
    }

    return n;
}

int main()
{
    int i, j;
    int iCase = 1;

    while (1) {
        int sum = 0;

        for (i = 1; i <= 6; i++) {
            scanf("%d", &a[i]);
            sum += a[i] * i;
        }

        if (sum == 0) break;

        printf("Collection #%d:\n", iCase++);

        if (sum % 2) {
            printf("Can't be divided.\n\n");
            continue;
        }

        int n = multiplePack();
        memset(dp, 0, sizeof(dp));
        dp[0] = 1;

        for (i = 0; i < n; i++) {
            for (j = sum / 2; j >= b[i]; j--)
                if (dp[j - b[i]]) dp[j] = 1;
        }

        if (dp[sum / 2]) printf("Can be divided.\n\n");
        else printf("Can't be divided.\n\n");
    }

    return 0;
}
