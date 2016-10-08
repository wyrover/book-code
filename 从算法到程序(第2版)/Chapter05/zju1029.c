#include <stdio.h>
#include <memory.h>

int main()
{
    int iCase;
    scanf("%d", &iCase);

    while (iCase--) {
        int i, j;
        int move[200];
        int n;
        int from, to;
        scanf("%d", &n);
        memset(move, 0, sizeof(move));

        for (i = 0; i < n; i++) {
            scanf("%d%d", &from, &to);
            from = (from - 1) / 2;
            to = (to - 1) / 2;

            if (from > to) {
                int temp = from;
                from = to;
                to = temp;
            }

            for (j = from; j <= to; j++)
                move[j]++;
        }

        int max = 0;

        for (i = 0; i < 200; i++)
            if (move[i] > max) max = move[i];

        printf("%d\n", max * 10);
    }

    return 0;
}
