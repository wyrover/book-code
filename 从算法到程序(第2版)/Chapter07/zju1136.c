#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

int cmp(const void * a, const void * b)
{
    return (*(int *)a - * (int *)b);
}

int n, m;
int a[11];

void bfs()
{
    int i, j, k;
    int x;
    int num;
    int y[11];
    char b[5001];
    int q[5001][3];
    q[0][0] = q[0][1] = q[0][2] = 0;
    int head = 0;
    int tail = 0;
    memset(b, 1, sizeof(b));

    do {
        for (i = 0; i < m; i++) {
            if (head == 0 && a[i] == 0) continue;

            x = (q[head][0] * 10 + a[i]) % n;

            if (b[x]) {
                tail++;
                q[tail][0] = x;
                q[tail][1] = a[i];
                q[tail][2] = head;
                b[x] = 0;

                if (x == 0) {
                    num = 0;
                    k = tail;

                    while (1) {
                        num++;
                        y[num] = q[k][1];
                        k = q[k][2];

                        if (k == 0) break;
                    }

                    for (j = num; j >= 1; j--)
                        printf("%d", y[j]);

                    printf("\n");
                    return;
                }
            }
        }

        head++;
    } while (head <= tail);

    printf("0\n");
}

int main()
{
    int i;

    while (scanf("%d %d", &n, &m) != EOF) {
        for (i = 0; i < m; i++)
            scanf("%d", &a[i]);

        qsort(a, m, sizeof(int), cmp);

        if (n == 0) printf("0\n");
        else bfs();
    }

    return 0;
}
