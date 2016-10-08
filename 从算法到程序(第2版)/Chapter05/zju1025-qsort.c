#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define maxN 5001
struct stick {
    int l;
    int w;
};

int DP(int n, stick a[])
{
    int b[maxN];
    memset(b, 0, sizeof(b));
    int i, j, k;
    b[0] = 1;

    for (i = 1; i < n; i++) {
        k = 0;

        for (j = 0; j < i; j++)
            if (a[i].w < a[j].w && k < b[j]) k = b[j];

        b[i] = k + 1;
    }

    int max = 0;

    for (i = 0; i < n; i++)
        if (b[i] > max) max = b[i];

    return max;
}

int cmp(const void *a, const void *b)
{
    struct stick *c = (struct stick*)a;
    struct stick *d = (struct stick*)b;

    if (c->l == d->l)
        return c->w - d->w;

    return c->l - d->l;
}

int main()
{
    stick data[maxN];
    int i, k;
    int test;
    scanf("%d", &test);

    for (k = 0; k < test; ++k) {
        int n;
        scanf("%d", &n);

        for (i = 0; i < n; ++i)
            scanf("%d%d", &data[i].l, &data[i].w);

        qsort(data, n, sizeof(struct stick), cmp);
        printf("%d\n", DP(n, data));
    }

    return 0;
}
