#include<cstdio>
#include<iostream>
using namespace std;

typedef long long LL;

LL x, y;

LL Extended_Euclid(LL a, LL b)
{
    if (b == 0) {
        y = 0;
        x = 1;
        return a;
    }

    LL d = Extended_Euclid(b, a % b);
    LL t = x;
    x = y;
    y = t - (a / b) * y;
    return d;
}

int main()
{
    int n;
    LL m1, m2, b1, b2;

    while (scanf("%d", &n) != EOF) {
        bool flag = false;
        scanf("%I64d%I64d", &m1, &b1);

        for (int i = 1; i < n; i++) {
            scanf("%I64d%I64d", &m2, &b2);

            if (flag) continue;

            LL r = b2 - b1;
            LL d = Extended_Euclid(m1, m2);

            if (r % d != 0) {
                flag = true;
                continue;
            }

            LL t = m2 / d;
            x = ((r / d * x) % t + t) % t;
            b1 = m1 * x + b1;
            m1 = m1 * m2 / d;
        }

        printf("%I64d\n", flag ? -1 : b1);
    }

    return 0;
}
