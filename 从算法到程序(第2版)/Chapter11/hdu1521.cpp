#include<stdio.h>
#include<string.h>

int main()
{
    int i;
    double a[15];
    a[0] = a[1] = 1;

    for (i = 2; i <= 10; i++)
        a[i] = i * a[i - 1];

    int n, m;

    while (scanf("%d%d", &n, &m) != EOF) {
        int num[15];
        double c1[15], c2[15];

        for (i = 1; i <= n; i++)
            scanf("%d", &num[i]);

        memset(c1, 0, sizeof(c1));
        memset(c2, 0, sizeof(c2));

        for (i = 0; i <= num[1]; i++)
            c1[i] = 1.0 / a[i];

        int j, k;

        for (i = 2; i <= n; i++) {
            for (j = 0; j <= m; j++)
                for (k = 0; k <= num[i] && j + k <= m; k++)
                    c2[j + k] += c1[j] / a[k];

            memcpy(c1, c2, sizeof(c2));
            memset(c2, 0, sizeof(c2));
        }

        printf("%.0lf\n", c1[m]*a[m]);
    }

    return 0;
}
