#include<iostream>
#include<cstdio>
using namespace std;

#define LL __int64
#define MAX 100000000000LL

LL phi(LL n)
{
    LL i;
    LL ans = n;

    for (i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            ans = ans / i * (i - 1);

            while (n % i == 0) n /= i;
        }
    }

    if (n != 1)   ans = ans / n * (n - 1);

    return ans;
}

//a*b%m
LL multi(LL a, LL b, LL m)
{
    LL t = 0;

    while (b) {
        if (b & 1) {
            t += a;

            if (t >= m) t -= m;
        }

        a += a;

        if (a >= m) a -= m;

        b >>= 1;
    }

    return t;
}

//a^n%MOD
LL quickmod(LL n, LL MOD)
{
    LL a = 10;
    LL ans = 1;
    ans %= MOD;

    for (; n > 0; n >>= 1, a = multi(a, a, MOD))
        if (n & 1) ans = multi(ans, a, MOD);

    return ans;
}

int main()
{
    int iCase = 0;
    LL n, i;

    while (~scanf("%I64d", &n) && n) {
        int k = 0;

        while (n % 2 == 0) {
            n /= 2;
            k++;
        }

        if (k > 3 || n % 5 == 0) {
            printf("Case %d: 0\n", ++iCase);
            continue;
        }

        n *= 9;
        LL len = phi(n);
        LL number = MAX;

        for (i = 1; i * i <= len; i++) {
            if (len % i == 0) {
                LL f = len / i;

                if (quickmod(i, n) == 1) number = min(number, i);

                if (quickmod(f, n) == 1) number = min(number, f);
            }
        }

        if (number == MAX) number = 0;

        printf("Case %d: %I64d\n", ++iCase, number);
    }

    return 0;
}
