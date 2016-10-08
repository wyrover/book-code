#include <stdio.h>
int main()
{
    int i;
    int n;
    char now, p;
    int N;
    scanf("%d", &N);

    while (N--) {
        scanf("%d\n", &n);
        scanf("%c", &now);
        int ans = 0;

        for (i = 1; i < n;) {
            scanf("%c", &p);

            if (p == 'U' || p == 'D') {
                i++;

                if (p != now) {
                    ans++;
                    now = p;
                }
            }
        }

        printf("%d\n", ans);

        if (N) putchar('\n');
    }

    return 0;
}
