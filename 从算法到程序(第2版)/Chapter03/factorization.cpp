#include <stdio.h>

int total;
void solve(int n)
{
    if (n == 1) total++;
    else for (int i = 2; i <= n; i++)
            if (n % i == 0) solve(n / i);
}

int main()
{
    int n;

    while (scanf("%d", &n) != EOF) {
        total = 0;
        solve(n);
        printf("%d\n", total);
    }

    return 0;
}
