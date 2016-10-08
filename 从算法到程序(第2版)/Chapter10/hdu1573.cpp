#include<stdio.h>

int x, y;
int extend_gcd(int m, int b)
{
    if (b == 0) {
        y = 0;
        x = 1;
        return m;
    }

    int d = extend_gcd(b, m % b);
    int t = x;
    x = y;
    y = t - m / b * y;
    return d;
}

int a[10], b[10];
int n, m;
int main()
{
    int iCase;
    scanf("%d", &iCase);

    while (iCase--) {
        int i;
        scanf("%d%d", &n, &m);

        for (i = 0; i < m; i++)
            scanf("%d", &a[i]);

        for (i = 0; i < m; i++)
            scanf("%d", &b[i]);

        int m1, m2, b1, b2;
        int flag = 0;
        m1 = a[0];
        b1 = b[0];

        for (i = 1; i < m; i++) {
            m2 = a[i];
            b2 = b[i];
            int r = b2 - b1;
            int d = extend_gcd(m1, m2);

            if ((r) % d) {
                flag = 1;
                break;
            }

            int t = m2 / d;
            x = ((r / d * x) % t + t) % t;
            b1 = m1 * x + b1;
            m1 = (m1 * m2) / d;
        }

        int ans = (n - b1) / m1 + (b1 == 0 ? 0 : 1);

        if (flag || n < b1) ans = 0;

        printf("%d\n", ans);
    }

    return 0;
}
