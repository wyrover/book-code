#include <stdio.h>

char seq[1024][1024];

int main()
{
    int heap[2048];
    int n, l;

    while (scanf("%d %d", &n, &l) && (n || l)) {
        int i, k;

        for (k = 0 ; k < n ; k++)
            scanf("%s", seq[k]);

        int cost = 0;

        for (i = 0 ; i < l ; i++) {
            for (k = 0 ; k < n ; k++)
                heap[n + k] = 1 << (seq[k][i] - 'A');

            for (k = n - 1 ; k > 0 ; k--)
                if (!(heap[k] = heap[k + k] & heap[k + k + 1])) {
                    ++cost;
                    heap[k] = heap[k + k] | heap[k + k + 1];
                }

            char ch = 'A';

            while (heap[1] >>= 1) ++ch;

            printf("%c", ch);
        }

        printf(" %d\n", cost);
    }

    return 0;
}
