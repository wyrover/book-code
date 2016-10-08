#include <stdio.h>
#include <stdlib.h>

int n;
struct pair {
    int start, end, pos;
} seq[1000];

int cmp(const void * a, const void * b)
{
    return ((struct pair*)a)->end - ((struct pair*)b)->end;
}

int main()
{
    while (scanf("%d", &n) && n) {
        for (int i = 0; i < n; i++) {
            seq[i].pos = i + 1;
            scanf("%d%d", &seq[i].start, &seq[i].end);
        }

        qsort(seq, n, sizeof(pair), cmp);
        int preEnd = seq[0].end;
        printf("%d", seq[0].pos);

        for (int i = 0; i < n; i++)
            if (seq[i].start > preEnd) {
                preEnd = seq[i].end;
                printf(" %d", seq[i].pos);
            }

        printf("\n");
    }

    return 0;
}
