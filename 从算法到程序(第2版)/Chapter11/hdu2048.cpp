#include <stdio.h>
double a[22] = {1, 1};
double d[22] = {0, 0, 1, 2};
int main()
{
    int i, n, m;

    for (i = 1; i < 21; i++)
        a[i] = i * a[i - 1];

    for (i = 3; i < 21; i++)
        d[i] = (i - 1) * (d[i - 1] + d[i - 2]);

    scanf("%d", &m);

    while (m--) {
        scanf("%d", &n);
        printf("%.2lf%%\n", d[n] * 100 / a[n]);
    }

    return 0;
}
