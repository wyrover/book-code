#include <stdio.h>
#include <string.h>

#define maxn 999999999
#define sqare(x) (x)*(x)
#define min(x,y) (x)<(y)?(x):(y)

int k, n;
int stick[5010];
int a[5010], b[5010];

int dp()
{
    int temp;
    int i, j;

    for (i = 1; i <= k; i++) {
        for (j = i * 2; j <= n; j++) {
            b[j] = maxn;

            if (j > i * 2) b[j] = b[j - 1];

            if (n - j > (k - i) * 3) {
                temp = a[j - 2] + sqare(stick[j - 1] - stick[j]);
                b[j] = min(b[j], temp);
            }
        }

        memcpy(a, b, sizeof(b));
    }

    return 0;
}

int main()
{
    int cases;
    scanf("%d", &cases);
    int i;

    while (cases--) {
        scanf("%d%d", &k, &n);

        for (i = 1; i <= n; i++)
            scanf("%d", &stick[i]);

        k += 8;
        memset(a, 0, sizeof(a));
        dp();
        printf("%d\n", a[n]);
    }

    return 0;
}
