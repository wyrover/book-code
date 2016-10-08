#include <stdio.h>
#include <memory.h>

#define min(x,y) ((x)<(y)?(x):(y))
#define NUM 501
int trade[NUM][NUM], carry[NUM][NUM];

int main()
{
    int iCase = 0;
    int n;

    while (scanf("%d", &n) && n) {
        if (iCase++) printf("\n");

        int w[4], s[4], d[4];
        int i;

        for (i = 1; i <= 3; i++)
            scanf("%d%d%d", &w[i], &s[i], &d[i]);

        int c1, c2, c3, d4;
        scanf("%d%d%d%d", &c1, &c2, &c3, &d4);
        d4 -= c1 * d[1] + c2 * d[2] + c3 * d[3];
        memset(carry, 255, sizeof(carry));
        carry[0][0] = 0;
        int row = 0;
        int col = 0;
        int j, k, ja, ka;

        for (i = 0; i < n; i++) {
            int weight, size;
            scanf("%d%d", &weight, &size);
            memset(trade, 255, sizeof(trade));
            int newRow = row;
            int newCol = col;
            int weight1, size1, weight2, size2;

            for (j = 0; j <= row; j++)
                for (k = 0; k <= col; k++)
                    if (carry[j][k] >= 0)
                        for (ja = j, weight1 = size1 = 0; (weight1 <= weight && size1 <= size); weight1 += w[1], size1 += s[1], ja++)
                            for (ka = k, weight2 = weight1, size2 = size1; (weight2 <= weight && size2 <= size); weight2 += w[2], size2 += s[2], ka++) {
                                if (newRow < ja) newRow = ja;

                                if (newCol < ka) newCol = ka;

                                int bootWeight = (weight - weight2) / w[3];
                                int bootSize = (size - size2) / s[3];

                                if (bootWeight > bootSize) bootWeight = bootSize;

                                bootWeight += carry[j][k];

                                if (trade[ja][ka] < bootWeight) trade[ja][ka] = bootWeight;
                            }

            memcpy(carry, trade, sizeof(trade));
            row = newRow;
            col = newCol;
        }

        int iBest = 0;

        for (j = 0; j <= row; j++)
            for (k = 0; k <= col; k++)
                if (carry[j][k] >= 0) {
                    int defend = j * d[1] + k * d[2] + carry[j][k] * d[3];
                    int helms = j / c1;
                    int armors = k / c2;
                    int boots = carry[j][k] / c3;

                    if (d4 > 0) defend += d4 * min(helms, min(armors, boots));

                    if (iBest < defend) iBest = defend;
                }

        printf("Case %d: %d\n", iCase, iBest);
    }

    return 0;
}
