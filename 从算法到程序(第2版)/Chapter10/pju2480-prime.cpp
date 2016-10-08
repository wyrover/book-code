#include <stdio.h>

#define MAX 50000
__int64 prime[MAX];
int pnum = 0;

void primtable()
{
    bool flag[MAX] = {0};

    for (int i = 2; i < MAX; i++) {
        if (!flag[i]) {
            prime[pnum++] = i;

            for (long j = i + i; j < MAX; j += i)
                flag[j] = 1;
        }
    }
}

int main()
{
    primtable();
    int n;

    while (scanf("%d", &n) != EOF) {
        int i;
        __int64 ans = n;
        int p, a;

        for (i = 0; prime[i]*prime[i] <= n; ++i) {
            p = prime[i];

            if (n % p == 0) {
                a = 0;

                while (n % p == 0) {
                    a++;
                    n /= p;
                }

                ans = ans + ans * a * (p - 1) / p;
            }
        }

        if (n != 1) ans = ans + ans * (n - 1) / n;

        printf("%I64d\n", ans);
    }

    return 0;
}
