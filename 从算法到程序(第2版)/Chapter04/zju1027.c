#include<stdio.h>
#define max(x,y) ((x)>(y)?(x):(y))

#define NUM 101
char str1[NUM], str2[NUM];
char map[128];
int gene[NUM][NUM];

int main()
{
    map['A'] = 0;
    map['C'] = 1;
    map['G'] = 2;
    map['T'] = 3;
    map['-'] = 4;
    int score[5][5] =  {{5, -1, -2, -1, -3},
        { -1, 5, -3, -2, -4},
        { -2, -3, 5, -2, -2},
        { -1, -2, -2, 5, -1},
        { -3, -4, -2, -1, 0}
    };
    int iCase;
    int i, j, k;
    int first, second;
    scanf("%d ", &iCase);

    for (k = 0 ; k < iCase ; k++) {
        scanf("%d%s", &first, str1);
        scanf("%d%s", &second, str2);
        gene[0][0] = 0;

        for (i = 1; i <= second; i++)
            gene[0][i] = gene[0][i - 1] + score[4][map[str2[i - 1]]];

        for (i = 1; i <= first; i++)
            gene[i][0] = gene[i - 1][0] + score[map[str1[i - 1]]][4];

        int m1, m2, m3;

        for (i = 1; i <= first; i++)
            for (j = 1; j <= second ; j++) {
                m1 = gene[i - 1][j] + score[map[str1[i - 1]]][4];
                m2 = gene[i][j - 1] + score[4][map[str2[j - 1]]];
                m3 = gene[i - 1][j - 1] + score[map[str1[i - 1]]][map[str2[j - 1]]];
                gene[i][j] = max(m1, max(m2, m3));
            }

        printf("%d\n", gene[first][second]);
    }

    return 0;
}
