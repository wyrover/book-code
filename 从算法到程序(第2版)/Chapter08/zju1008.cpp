#include <stdio.h>

int n;
int q;
int iSquare[25][4];
int iCount[25];
int iTable[25];

int Place(int iPos)
{
    int i;

    if (iPos == n * n) return 1;

    for (i = 0; i < q; i++) {
        if (iCount[i] == 0) continue;

        if (iPos % n != 0)
            if (iSquare[iTable[iPos - 1]][1] != iSquare[i][3])
                continue;

        if (iPos >= n)
            if (iSquare[iTable[iPos - n]][2] != iSquare[i][0])
                continue;

        iTable[iPos] = i;
        iCount[i]--;

        if (Place(iPos + 1) == 1) return 1;

        iCount[i]++;
    }

    return 0;
}

int main()
{
    int i, j;
    int iCase = 0;
    int top, right, bottom, left;

    while (scanf("%d", &n) && n) {
        iCase++;
        q = 0;

        for (i = 0; i < n * n; i++) {
            scanf("%d%d%d%d", &top, &right, &bottom, &left);

            for (j = 0; j < q; j++) {
                if (iSquare[j][0] == top && iSquare[j][1] == right &&
                    iSquare[j][2] == bottom && iSquare[j][3] == left) {
                    iCount[j]++;
                    break;
                }
            }

            if (j == q) {
                iSquare[j][0] = top;
                iSquare[j][1] = right;
                iSquare[j][2] = bottom;
                iSquare[j][3] = left;
                iCount[j] = 1;
                q++;
            }
        }

        if (iCase > 1) printf("\n");

        if (Place(0) == 1) printf("Game %d: Possible\n", iCase);
        else printf("Game %d: Impossible\n", iCase);
    }

    return 0;
}
