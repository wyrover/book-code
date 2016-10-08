#include<stdio.h>
#include<string.h>

#define LL long long

LL gcd(LL a, LL b)
{
    return b == 0 ? a : gcd(b, a % b);
}

//¦Õ(n)
LL eular(LL n)
{
    LL ans = n;

    for (LL i = 2; i * i <= n; i++)
        if (n % i == 0) {
            ans -= ans / i;

            while (n % i == 0) n /= i;
        }

    if (n > 1) ans -= ans / n;

    return ans;
}

//a*b%p
LL p;
LL multi(LL a, LL b)
{
    LL ans = 0;

    while (b) {
        if (b & 1) ans = (ans + a) % p;

        a = a * 2 % p;
        b >>= 1;
    }

    return ans;
}

//a^b%p
LL power_mod(LL a, LL b)
{
    LL ans = 1;

    while (b) {
        if (b & 1) ans = multi(ans, a);

        a = multi(a, a);
        b >>= 1;
    }

    return ans;
}

int main()
{
    int iCase = 1;
    int L;

    while (scanf("%d", &L) != EOF && L) {
        p = L / gcd(L, 8) * 9;

        if (gcd(10, p) != 1 || L % 5 == 0) {
            printf("Case %d: 0\n", iCase++);
            continue;
        }

        LL n = eular(p);
        LL number = n;

        for (LL i = 2; i * i < n; i++)
            if (n % i == 0) {
                int cnt = 0;

                while (n % i == 0) {
                    cnt++;
                    n /= i;
                }

                while (cnt) {
                    if (power_mod(10, number / i) % p == 1) {
                        number /= i;
                        cnt--;
                    } else break;
                }
            }

        if (n > 1 && power_mod(10, number / n) % p == 1) number /= n;

        printf("Case %d: %lld\n", iCase++, number);
    }

    return 0;
}
