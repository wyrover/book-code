#include <stdio.h>
#include <memory.h>
#include <iostream>
using namespace std;

const int mask[13] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096};
struct {
    int number, length;
} back[4096];
int part[13];
int board, saw;
int n;

void backtrack(int a)
{
    if (back[a].number >= 0) return;

    int i;
    int nowNum, nowLen;
    int bestNum = 1000;
    int bestLen = 0;
    int index;

    for (i = 0; i < n; i++)
        if ((a & mask[i]) != 0) {
            index = a ^ mask[i];
            backtrack(index);
            nowNum = back[index].number;

            if (back[index].length >= part[i]) {
                nowLen = back[index].length - part[i];
            } else {
                nowNum++;
                nowLen = board - part[i];
            }

            nowLen = max(nowLen - saw, 0);

            if (bestNum > nowNum ||
                (bestNum == nowNum && bestLen < nowLen)) {
                bestNum = nowNum;
                bestLen = nowLen;
            }
        }

    back[a].number = bestNum;
    back[a].length = bestLen;
}

int main()
{
    printf("Problem 7 by team x\n");
    char c;

    while ((c = getc(stdin)) != EOF) {
        ungetc(c, stdin);
        memset(back, 255, sizeof(back));
        scanf("%d %d", &board, &saw);
        n = 0;

        while ((c = getc(stdin)) != '\n') {
            ungetc(c, stdin);
            scanf("%d", &part[n++]);
        }

        back[0].number = back[0].length = 0;
        backtrack(mask[n] - 1);
        printf("\n");
        printf("Board length            =%6d\n", board);
        printf("Saw width               =%6d\n", saw);
        printf("Number of boards needed =%6d\n", back[mask[n] - 1].number);
    }

    printf("End of problem 7 by team x\n");
    return 0;
}
