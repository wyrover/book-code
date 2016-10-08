#include<stdio.h>
#include<string.h>

int a[20] = {20, 72, 72, 56, 60, 12, 92, 56, 0, 52, 12, 56, 40, 92, 32, 56, 80, 32, 52, 56};

int main()
{
    int iCase;
    __int64 n;

    while (scanf("%d", &iCase), iCase) {
        int i;

        for (i = 1; i <= iCase; i++) {
            scanf("%I64d", &n);
            printf("Case %d: ", i);

            if (n == 1) puts("2");
            else if (n == 2) puts("6");
            else if (n <= 22) printf("%d\n", a[n - 3]);
            else printf("%d\n", a[(n - 3) % 20]);
        }

        printf("\n");
    }

    return 0;
}
