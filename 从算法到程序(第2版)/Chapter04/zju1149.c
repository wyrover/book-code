#include<stdio.h>

int a[7];

void enumerate(int sum)
{
    int mid;
    int i1, i2, i3, i4, i5, i6;

    for (i1 = 0; i1 <= a[1]; i1++)
        for (i2 = 0; i2 <= a[2]; i2++)
            for (i3 = 0; i3 <= a[3]; i3++)
                for (i4 = 0; i4 <= a[4]; i4++)
                    for (i5 = 0; i5 <= a[5]; i5++)
                        for (i6 = 0; i6 <= a[6]; i6++) {
                            mid = 1 * i1 + 2 * i2 + 3 * i3 + 4 * i4 + 5 * i5 + 6 * i6;

                            if (mid == sum / 2) {
                                printf("Can be divided.\n\n");
                                return;
                            }
                        }

    printf("Can't be divided.\n\n");
}

int main()
{
    int i;
    int iCase = 1;

    while (1) {
        int sum = 0;

        for (i = 1; i <= 6; i++) {
            scanf("%d", &a[i]);

            if (a[i] != 0 && a[i] % 6 == 0) a[i] = 6;
            else a[i] = a[i] % 6;

            sum += a[i] * i;
        }

        if (sum == 0) break;

        printf("Collection #%d:\n", iCase++);

        if (sum % 2 == 1) {
            printf("Can't be divided.\n\n");
            continue;
        }

        enumerate(sum);
    }

    return 0;
}
