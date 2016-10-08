#include <stdio.h>
#include <string.h>
int main()
{
    int a[10002];
    int s[10002] = {0};
    int flag[10002];
    int find = 0;
    int n;
    int i, j;
    memset(flag, 0, sizeof(flag));
    scanf("%d", &n);

    for (i = 1; i <= n; i++) {
        scanf("%d", &a[i]);

        if (!find) {
            s[i] = (s[i - 1] + a[i]) % n;

            if (s[i] == 0) {
                printf("%d\n", i);

                for (j = 1; j <= i; j++)
                    printf("%d\n", a[j]);

                find = 1;
            } else if (!flag[s[i]]) flag[s[i]] = i;
            else {
                find = 1;
                printf("%d\n", i - flag[s[i]]);

                for (j = flag[s[i]] + 1; j <= i; j++)
                    printf("%d\n", a[j]);
            }
        }
    }

    return 0;
}
