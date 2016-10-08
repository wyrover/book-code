//480ms
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <iostream>
using namespace std;

int m;
int num;
int a[35];
int pos[35];
bool visit[35];
bool exist;

inline bool equal(int sum)
{
    for (int i = 0; i < m; ++i)
        if (a[i] == sum) return true;

    return false;
}

void dfs(int k, int sum)
{
    int i;

    if (sum > a[m - 1]) return;

    if (k > num) return;

    if (k == num && equal(sum)) {
        exist = true;

        for (i = 0; i < k - 1; ++i)
            printf("%d+", a[pos[i]]);

        printf("%d=%d\n", a[pos[k - 1]], sum);
        return;
    }

    int b;

    if (k == 0) b = 0;
    else b = pos[k - 1] + 1;

    for (i = b; i < m; ++i) {
        if (!visit[i] && (a[i] > a[pos[k - 1]] || k == 0)) {
            visit[i] = true;
            pos[k] = i;
            dfs(k + 1, sum + a[i]);
            visit[i] = false;
        }
    }
}

int main()
{
    int iCase;
    scanf("%d", &iCase);

    while (iCase--) {
        scanf("%d", &m);

        for (int i = 0; i < m; ++i)
            scanf("%d", &a[i]);

        sort(a, a + m);
        memset(visit, false, sizeof(visit));
        exist = false;

        for (num = 2; num < m; ++num)
            dfs(0, 0);

        if (!exist) printf("Can't find any equations.\n");

        printf("\n");
    }

    return 0;
}
