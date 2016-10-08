#include <stdio.h>
int main()
{
    int i;
    int n;
    char p[100000];
    int N;
    scanf("%d", &N);

    while (N--) {
        scanf("%d", &n);

        for (i = 0; i < n;) {
            scanf("%c", p + i);

            if (p[i] == 'U' || p[i] == 'D') i++;
        }

        int ans = 0;
        int pos = 0;

        for (i = 1; i < n; i++)
            if (p[i] != p[pos]) {
                pos = i;
                ans++;
            }

        printf("%d\n", ans);

        if (N) putchar('\n');
    }

    return 0;
}
