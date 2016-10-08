#include<stdio.h>
#include<string.h>

int a[7];

void DP(int sum)
{
    int i, j, k;
    int mid = sum / 2;
    char visit[200];
    memset(visit, 0, sizeof(visit));
    int t;
    visit[0] = 1;

    for (i = 1; i <= 6; i++)
        for (j = mid; j >= 0; j--)
            if (visit[j])
                for (k = 1; k <= a[i]; k++) {
                    t = j + i * k;

                    if (t > mid) break;
                    else visit[t] = 1;

                    if (t == mid) {
                        printf("Can be divided.\n\n");
                        return;
                    }
                }

    printf("Can't be divided.\n\n");
}

int main()
{
    int i;
    int iCase = 1;

    while (1) {
        int sum = 0;

        for (i = 1; i <= 6; i++) {
            scanf("%d", &a[i]);

            if (a[i] != 0 && a[i] % 6 == 0) a[i] = 6;
            else a[i] = a[i] % 6;

            sum += a[i] * i;
        }

        if (sum == 0) break;

        printf("Collection #%d:\n", iCase++);

        if (sum % 2) {
            printf("Can't be divided.\n\n");
            continue;
        }

        DP(sum);
    }

    return 0;
}

