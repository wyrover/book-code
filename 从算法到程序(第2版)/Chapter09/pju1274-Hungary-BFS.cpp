#include<stdio.h>
#include<string.h>
#define MAX 205

int n, m;
int map[MAX][50];

int hungary()
{
    int i, j;
    int num = 0;
    int q[MAX];
    int match[MAX];
    int qs, qe;
    int cx[MAX], cy[MAX];
    memset(cx, -1, sizeof(cx));
    memset(cy, -1, sizeof(cy));
    int x;

    for (i = 1; i <= n; ++i) {
        if (cx[i] != -1) continue;

        memset(match, -1, sizeof(match));
        qs = qe = 0;

        for (j = 1; j <= map[i][0]; ++j) {
            match[map[i][j]] = 0;
            q[qe++] = map[i][j];
        }

        while (qs < qe) {
            x = q[qs];

            if (cy[x] == -1) break;

            ++qs;

            for (j = 1; j <= map[cy[x]][0]; ++j)
                if (match[map[cy[x]][j]] == -1) {
                    match[map[cy[x]][j]] = x;
                    q[qe++] = map[cy[x]][j];
                }
        }

        if (qs == qe) continue;

        while (match[x] > 0) {
            cx[cy[match[x]]] = x;
            cy[x] = cy[match[x]];
            x = match[x];
        }

        cx[i] = x;
        cy[x] = i;
        ++num;
    }

    return num;
}

int main()
{
    int i, j;

    while (scanf("%d%d", &n, &m) != EOF) {
        memset(map, 0, sizeof(map));

        for (i = 1; i <= n; ++i) {
            scanf("%d", &map[i][0]);

            for (j = 1; j <= map[i][0]; ++j)
                scanf("%d", &map[i][j]);
        }

        printf("%d\n", hungary());
    }

    return 0;
}
