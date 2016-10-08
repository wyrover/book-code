#include<stdio.h>
#include<string.h>
#include<algorithm>
using namespace std;

int number;
int h;
char tri[100][200];

void downDFS(int row, int col, int height)
{
    number = height;

    if (row < h && col > 0) {
        for (int i = 0; i < 2 * height + 1; i++)
            if (tri[row][col + i] != '-')
                return;

        downDFS(row + 1, col - 2, height + 1);
    }
}

void upDFS(int row, int col, int height)
{
    number = height;

    if (row >= 0) {
        for (int i = 0; i < 2 * height + 1; i++)
            if (tri[row][col + i] != '-')
                return;

        upDFS(row - 1, col, height + 1);
    }
}

int main()
{
    int cases = 0;
    int i, j;

    while (scanf("%d\n", &h) && h) {
        memset(tri, 0, sizeof(tri));

        for (i = 0; i < h; i++)
            scanf("%s", tri[i]);

        int best = 0;

        for (i = 0; i < h - best; i++)
            for (j = 1 + 2 * best; j < strlen(tri[i]) - 2 * best - 1; j += 2)
                if (tri[i][j] == '-') {
                    downDFS(i, j, 0);

                    if (number > best) best = number;
                }

        for (i = h - 1; i >= best; i--)
            for (j = 0; j < strlen(tri[i]); j += 2)
                if (tri[i][j] == '-') {
                    upDFS(i, j, 0);

                    if (number > best) best = number;
                }

        printf("Triangle #%d\n", ++cases);
        printf("The largest triangle area is %d.\n\n", best * best);
    }

    return 0;
}
