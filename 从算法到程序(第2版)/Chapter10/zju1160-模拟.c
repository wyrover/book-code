#include <stdio.h>
int main()
{
    int N;
    int p, e, i, d;
    int days;
    scanf("%d", &N);

    while (N--) {
        int iCase = 0;

        while (scanf("%d%d%d%d", &p, &e, &i, &d) && p != -1) {
            days = 1;

            while ((days + d - p) % 23 || (days + d - e) % 28 || (days + d - i) % 33) days++;

            printf("Case %d: ", ++iCase);
            printf("the next triple peak occurs in %d days.\n", days);
        }

        if (N) printf("\n");
    }

    return 0;
}
