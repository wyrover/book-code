#include <stdio.h>
#include <string.h>

__int64 b[13][2100];
int tran[14000][2];

int H, W, maxMove, nTran;

void dfs(int n, int from, int to)
{
    if (n > W) return;

    if (n == W) {
        tran[nTran][0] = from;
        tran[nTran ++][1] = to;
        return;
    }

    dfs(n + 2, (from << 2) + 3, (to << 2) + 3);
    dfs(n + 1, (from << 1) + 1, to << 1);
    dfs(n + 1, from << 1, (to << 1) + 1);
}

void dp()
{
    memset(b, 0x00, sizeof(b));
    b[0][(1 << W) - 1] = 1;
    int i, j;

    for (i = 0; i < H; i ++)
        for (j = 0; j < nTran; j ++)
            b[i + 1][tran[j][1]] += b[i][tran[j][0]];
}

int main()
{
    while (scanf("%d%d", &H, &W)) {
        if (!H)  break;

        int t;

        if (H < W) {
            t = H;
            H = W;
            W = t;
        }

        nTran = 0;
        dfs(0, 0, 0);
        dp();
        printf("%I64d\n", b[H][(1 << W) - 1]);
    }

    return 0;
}
