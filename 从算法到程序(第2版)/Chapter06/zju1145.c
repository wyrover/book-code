#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define LEFT -1
#define RIGHT -2
#define MUL -3
#define ADD -4
#define SUB -5
#define OP -6
#define NONE -10

char a[100];
int b[100];
int best[100];
int op[30];
int bn;
int iLeft;
int possible;
int apos, bpos, opos;

void space()
{
    while (a[apos] && (a[apos] == ' '))
        apos++;
}

int compute();
int bracket()
{
    int sum;

    if (b[bpos] == LEFT) {
        bpos++;
        sum = compute();
        bpos++;
    } else
        sum = b[bpos++];

    return sum;
}

int compute()
{
    int sum = bracket();

    while (b[bpos] == MUL || b[bpos] == ADD || b[bpos] == SUB) {
        int operation = b[bpos++];
        int ret = bracket();

        switch (operation) {
        case MUL:
            sum *= ret;
            break;

        case ADD:
            sum += ret;
            break;

        case SUB:
            sum -= ret;
            break;
        }
    }

    return sum;
}

void backtrack(int dep)
{
    if (possible) return;

    int i;

    if (dep == opos) {
        bpos = 0;
        int iRight = compute();

        if (iRight == iLeft) {
            possible = 1;

            for (i = 0; i < bn; i++)
                best[i] = b[i];
        }

        return;
    }

    b[op[dep]] = MUL;
    backtrack(dep + 1);
    b[op[dep]] = ADD;
    backtrack(dep + 1);
    b[op[dep]] = SUB;
    backtrack(dep + 1);
}

void print(int *q)
{
    printf("%d=", iLeft);
    int i;

    for (i = 0; i < bn; i++)
        switch (q[i]) {
        case ADD:
            printf("+");
            break;

        case MUL:
            printf("*");
            break;

        case SUB:
            printf("-");
            break;

        case LEFT:
            printf("(");
            break;

        case RIGHT:
            printf(")");
            break;

        case OP:
            printf("?");
            break;

        default:
            printf("%d", q[i]);
            break;
        }
}

int main()
{
    int iCase = 0;
    int i;

    while (gets(a) && strchr(a, '=')) {
        possible = 0;

        for (i = 0; i < 90; i++)
            b[i] = NONE;

        apos = 0;
        sscanf(a, "%d", &iLeft);

        while (a[apos] && isdigit(a[apos])) apos++;

        space();
        apos ++;
        bn = 0;
        opos = 0;

        while (space(), a[apos]) {
            if (a[apos] == '(') {
                b[bn++] = LEFT;
                apos++;
                continue;
            }

            if (a[apos] == ')') {
                b[bn++] = RIGHT;
                apos++;
            } else {
                sscanf(a + apos, "%d", &b[bn++]);

                while (a[apos] && isdigit(a[apos])) apos++;
            }

            space();

            if (a[apos] && a[apos] != ')') {
                op[opos++] = bn;
                b[bn++] = OP;
            }
        }

        backtrack(0);
        printf("Equation #%d:\n", ++iCase);

        if (bn == 1 && iLeft == b[0])
            printf("%d=%d\n", iLeft, iLeft);
        else if (bn == 0 || !possible)
            printf("Impossible\n");
        else {
            print(best);
            printf("\n");
        }

        printf("\n");
    }

    return 0;
}
