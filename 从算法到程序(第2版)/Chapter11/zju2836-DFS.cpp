#include <stdio.h>
#include <string.h>

int n, m;
int vis[11], va[11], cnt;
int ans;

int gcd(int a, int b)
{
    if (a % b == 0) return b;

    return gcd(b, a % b);
}

int lcm(int a, int b)
{
    return a / gcd(a, b) * b;
}

void dfs(int le, int cur, int num)
{
    if (num > cnt || le > m) return;

    int i;

    for (i = cur; i <= cnt; i++) {
        int temp = lcm(le, va[i]);

        if (num & 1)   ans += m / temp;
        else    ans -= m / temp;

        dfs(temp, i + 1, num + 1);
    }
}

int main()
{
    while (~scanf("%d%d", &n, &m)) {
        cnt = 0;
        memset(vis, 0, sizeof(vis));
        int i;

        for (i = 1; i <= n; i++) {
            int a;
            scanf("%d", &a);

            if (!vis[a]) {
                vis[a] = 1;
                va[++cnt] = a;
            }
        }

        ans = 0;

        for (i = 1; i <= cnt; i++) {
            ans += m / va[i];
            dfs(va[i], i + 1, 2);
        }

        printf("%d\n", ans);
    }

    return 0;
}
