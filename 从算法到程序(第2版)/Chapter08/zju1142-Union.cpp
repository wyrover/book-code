#include <stdio.h>
#include <memory.h>

struct CNode {
    int lg, dad;
} dset[11251];

int ls[2][151];
char s1[2][76];

void Find(int &a)
{
    int x = a;
    int dad;

    while (dset[a].dad >= 0) a = dset[a].dad;

    while (dset[x].dad >= 0) {
        dad = dset[x].dad;
        dset[x].dad = a;
        x = dad;
    }
}

void Union(int &a, int &b)
{
    int x = a;
    int y = b;

    while (dset[x].dad >= 0) x = dset[x].dad;

    while (dset[y].dad >= 0) y = dset[y].dad;

    if (x != y) {
        if (x == 0) dset[y].dad = 0;
        else {
            dset[x].dad = y;
            dset[y].lg += dset[x].lg;
        }
    }

    Find(a);
    Find(b);
}

int main()
{
    int iCase = 1;
    int W, H;
    int i, j, k;
    int sp, sp1;
    int iNow, iLast;
    char maze[76];
    char c;

    while (scanf("%d%d", &W, &H) && (W || H)) {
        memset(ls, 0, sizeof(ls));
        memset(s1, '/', sizeof(s1));
        dset[0].dad = -1;
        dset[0].lg = 0;
        iNow = 1;
        iLast = 0;
        sp = 0;

        for (i = 1; i <= H; i++) {
            ls[iNow][0] = 0;
            sp1 = 0;
            scanf("%s", maze);

            for (j = 1; j <= W; j++) {
                c = maze[j - 1];
                s1[iNow][j] = c;
                sp1++;
                k = ls[iNow][sp1 - 1];

                if (c == '/') {
                    if (s1[iLast][j] == '/')
                        Union(k, ls[iLast][sp1 + 1]);
                    else
                        Union(k, ls[iLast][sp1]);
                } else Find(k);

                if (k > 0) dset[k].lg++;

                ls[iNow][sp1++] = k;

                if (c == '\\') {
                    if (s1[iLast][j] == '/')
                        k = ls[iLast][sp1];
                    else
                        k = ls[iLast][sp1 - 1];
                } else {
                    sp++;
                    dset[sp].lg = 0;
                    dset[sp].dad = -1;
                    k = sp;
                }

                Find(k);

                if (k > 0) dset[k].lg++;

                ls[iNow][sp1] = k;
            }

            k = 0;
            Union(ls[iNow][sp1], k);
            iLast = iNow;
            iNow ^= 1;
        }

        k = 0;
        sp1 = 0;

        for (i = 1; i <= W; i++) {
            sp1++;

            if (s1[iLast][i] == '/')
                Union(ls[iLast][sp1 + 1], k);
            else
                Union(ls[iLast][sp1], k);

            sp1++;
        }

        int count = 0;
        int max = 0;

        for (i = 1; i <= sp; i++)
            if (dset[i].dad < 0) {
                count++;

                if (max < dset[i].lg) max = dset[i].lg;
            }

        printf("Maze #%d:\n", iCase++);

        if (count == 0) printf("There are no cycles.\n");
        else printf("%d Cycles; the longest has length %d.\n", count, max);

        printf("\n");
    }

    return 0;
}
