#include <stdio.h>

int h[20];

void OutTree(int x, int n)
{
    if (n == 1) {
        printf("X");
        return;
    }

    if (n == 0) return;

    int i, j;
    int left;
    j = n, j--;

    for (i = 0; i < n; i++, j--) {
        left = h[i] * h[j];

        if (x > left) x -= left;
        else break;
    }

    x--;

    if (i != 0) {
        printf("(");
        OutTree((x / h[j]) + 1, i);
        printf(")");
    }

    printf("X");

    if (j != 0) {
        printf("(");
        OutTree((x % h[j]) + 1, j);
        printf(")");
    }
}

int main()
{
    int i, j, k;
    int S[20];
    h[0] = 1;
    h[1] = 1;
    S[0] = 0;
    S[1] = 1;

    for (i = 2; i <= 19; i++) {
        int catalan = 0;
        k = i, k--;

        for (j = 0; j < i; j++, k--)
            catalan += h[j] * h[k];

        h[i] = catalan;
        S[i] = S[i - 1] + catalan;
    }

    int n;

    while (scanf("%d", &n) && n) {
        for (i = 1; i <= 19; i++)
            if (S[i] >= n) break;

        OutTree(n - S[i - 1], i);
        printf("\n");
    }

    return 0;
}
