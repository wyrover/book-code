#include <stdio.h>
#include <memory.h>
#include <string.h>

int candy[40][4];
char f[41][41][41][41];
int x[4];
int n;
int ans;

void memSearch(int basket[], int num)
{
    int i, j;
    int tb[5];
    int tnum = num;
    int full = 1;

    for (i = 0; i < 5; i++) {
        tb[i] = basket[i];

        if (!basket[i]) full = 0;
    }

    if (full) return;

    for (i = 0; i < 4; i++) {
        if (x[i] < n) {
            int get = 0;

            for (j = 0; j < 5; j++)
                if (candy[x[i]][i] == basket[j]) {
                    basket[j] = 0;
                    num ++;
                    get = 1;
                    break;
                }

            if (!get)
                for (j = 0; j < 5; j++)
                    if (!basket[j]) {
                        basket[j] = candy[x[i]][i];
                        break;
                    }

            x[i]++;

            if (f[x[0]][x[1]][x[2]][x[3]] < num) {
                f[x[0]][x[1]][x[2]][x[3]] = num;

                if (num > ans) ans = num;

                memSearch(basket, num);
            }

            num = tnum;
            x[i]--;
            memcpy(basket, tb, sizeof(tb));
        }
    }
}

int main()
{
    while (scanf("%d", &n) && n) {
        int i, j;
        int b[5];

        for (i = 0; i < n; i++)
            for (j = 0; j < 4; j++)
                scanf("%d", &candy[i][j]);

        ans = 0;
        memset(f, 0xff, sizeof(f));
        memset(x, 0, sizeof(x));
        memset(b, 0, sizeof(b));
        f[0][0][0][0] = 0;
        memSearch(b, 0);
        printf("%d\n", ans);
    }

    return 0;
}
