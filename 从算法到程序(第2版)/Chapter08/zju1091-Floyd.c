#include <stdio.h>
#include <stdlib.h>

int i, j, m;
void Floyd(int k[][64])
{
    int x, y;

    for (i = 0; i < 64; k[i][i] = 0, ++i)
        for (j = 0; j < 64; ++j) {
            x = abs(i / 8 - j / 8);
            y = abs(i % 8 - j % 8);

            if (x == 1 && y == 2 || x == 2 && y == 1)
                k[i][j] = k[j][i] = 1;
        }

    for (m = 0; m < 64; ++m)
        for (i = 0; i < 64; ++i)
            for (j = 0; j < 64; ++j)
                if (k[i][m] + k[m][j] < k[i][j])
                    k[i][j] = k[i][m] + k[m][j];
}

int main()
{
    int knight[64][64];

    for (i = 0; i < 64; ++i)
        for (j = 0; j < 64; ++j)
            knight[i][j] = 10;

    Floyd(knight);
    char s[5], t[5];

    while (scanf("%s%s", s, t) != EOF) {
        int x = (s[0] - 'a') * 8 + (s[1] - '1');
        int y = (t[0] - 'a') * 8 + (t[1] - '1');
        printf("To get from %s to %s ", s, t);
        printf("takes %d knight moves.\n", knight[x][y]);
    }

    return 0;
}
