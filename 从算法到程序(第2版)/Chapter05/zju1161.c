#include <stdio.h>
#include <string.h>

#define NUM 30
int f[NUM];
int d[NUM];
int t[NUM];
int plan[NUM];
int best;

void greedy(int pos, int time)
{
    if (time <= 0) return;

    int i, j;
    int fish[NUM];
    int p[NUM];
    int t = 0;

    for (i = 0; i < pos; ++i)
        fish[i] = f[i];

    memset(p, 0, sizeof(p));

    for (i = 0; i < time; ++i) {
        int max = 0;
        int id = -1;

        for (j = 0; j < pos; ++j)
            if (fish[j] > max) {
                max = fish[j];
                id = j;
            }

        if (id != -1) {
            ++p[id];
            fish[id] -= d[id];
            t += max;
        } else ++p[0];
    }

    if (t > best) {
        best = t;
        memset(plan, 0, sizeof(plan));

        for (i = 0; i < pos; ++i)
            plan[i] = p[i];
    }
}

int main()
{
    int N;
    scanf("%d", &N);

    while (N--) {
        int i;
        int n;
        int h;
        int flag = 0;

        while (scanf("%d", &n) && n) {
            scanf("%d", &h);
            h *= 12;

            for (i = 0; i < n; ++i)
                scanf("%d", &f[i]);

            for (i = 0; i < n; ++i)
                scanf("%d", &d[i]);

            for (i = 1; i < n; ++i)
                scanf("%d", &t[i]);

            int  time = 0;
            best = -1;

            for (i = 1; i <= n && h - time; ++i) {
                greedy(i, h - time);
                time += t[i];
            }

            if (flag++) printf("\n");

            for (i = 0; i < n - 1; ++i)
                printf("%d, ", plan[i] * 5);

            printf("%d\n", plan[n - 1] * 5);
            printf("Number of fish expected: %d\n", best);
        }

        if (N) printf("\n");
    }

    return 0;
}
