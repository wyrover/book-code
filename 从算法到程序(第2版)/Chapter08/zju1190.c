#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

char OP[5][4] = {"DUP", "ADD", "SUB", "MUL", "DIV"};
enum {ADD, DIV, DUP, MUL, SUB};
int ID[5] = {DUP, ADD, SUB, MUL, DIV};
int n;
int iBest;
int x[11][11], y[11];
int best[11];
int order[11];

void Check(int d)
{
    int i;

    for (i = 1; i <= n; i++)
        if (x[i][0] != y[i]) return;

    d--;

    if (iBest == d) {
        for (i = 1; i <= d; i++)
            if (ID[best[i]] < ID[order[i]]) return;
            else if (ID[best[i]] > ID[order[i]]) break;
    }

    iBest = d;
    memcpy(best, order, sizeof(order));
}

void DFS(int deep, int index)
{
    int i, j;
    int bx[11], bx1[11];

    if (index == 0) Check(deep);

    if (deep > iBest || deep > 10) return;

    for (i = 1; i <= n; i++)
        if (abs(x[i][index]) > 30000) return;

    order[deep] = 0;

    for (i = 1; i <= n; i++)
        x[i][index + 1] = x[i][index];

    DFS(deep + 1, index + 1);

    if (index > 0) {
        for (i = 1; i <= n; i++) {
            bx[i]  = x[i][index];
            bx1[i] = x[i][index - 1];
        }

        for (i = 1; i <= 4; i++) {
            order[deep] = i;

            switch (i) {
            case 1:
                for (j = 1; j <= n; j++)
                    x[j][index - 1] += x[j][index];

                break;

            case 2:
                for (j = 1; j <= n; j++)
                    x[j][index - 1] -= x[j][index];

                break;

            case 3:
                for (j = 1; j <= n; j++)
                    x[j][index - 1] = x[j][index - 1] * x[j][index];

                break;

            case 4:
                for (j = 1; j <= n; j++)
                    if (x[j][index] == 0) x[j][index - 1] = 30001;
                    else x[j][index - 1] = x[j][index - 1] / x[j][index];

                break;
            }

            DFS(deep + 1, index - 1);

            for (j = 1; j <= n; j++) {
                x[j][index]   = bx[j];
                x[j][index - 1] = bx1[j];
            }
        }
    }
}

int main()
{
    int i;
    int iCase = 1;

    while (scanf("%d", &n) && n) {
        for (i = 1; i <= n; i++)
            scanf("%d", &x[i][0]);

        for (i = 1; i <= n; i++)
            scanf("%d", &y[i]);

        iBest = 11;
        DFS(1, 0);
        printf("Program %d\n", iCase++);

        switch (iBest) {
        case 0:
            printf("Empty sequence\n");
            break;

        case 11:
            printf("Impossible\n");
            break;

        default:
            for (i = 1; i <= iBest; i++) {
                if (i != 1) printf(" ");

                printf("%s", OP[best[i]]);
            }

            printf("\n");
            break;
        }

        printf("\n");
    }

    return 0;
}
