#include <stdio.h>
#include <algorithm>
#include <iostream>
using namespace std;

int n;
struct gene {
    int start, end, pos;
} seq[1000];

bool cmp(gene a, gene b)
{
    if (a.end < b.end) return true;

    return false;
}

int main()
{
    while (scanf("%d", &n) && n) {
        for (int i = 0; i < n; i++) {
            seq[i].pos = i + 1;
            scanf("%d%d", &seq[i].start, &seq[i].end);
        }

        sort(seq, seq + n, cmp);
        int preEnd = seq[0].end;
        printf("%d", seq[0].pos);

        for (int i = 0; i < n; i++)
            if (seq[i].start > preEnd) {
                preEnd = seq[i].end;
                printf(" %d", seq[i].pos);
            }

        printf("\n");
    }

    return 0;
}
