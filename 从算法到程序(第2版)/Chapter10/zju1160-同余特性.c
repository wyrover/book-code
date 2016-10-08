#include <stdio.h>

int main()
{
    int p, e, i, d;
    int N;
    scanf("%d", &N);

    while (N--) {
        int iCase = 0;

        while (scanf("%d%d%d%d", &p, &e, &i, &d) && p != -1) {
            while ((p - e) % 28 != 0)
                p += 23;

            while ((p - i) % 33 != 0)
                p += 23 * 28;

            while (p > d)
                p -= 23 * 28 * 33;

            while (p <= d)
                p += 23 * 28 * 33;

            printf("Case %d: ", ++iCase);
            printf("the next triple peak occurs in %d days.\n", p - d);
        }

        if (N) printf("\n");
    }

    return 0;
}

