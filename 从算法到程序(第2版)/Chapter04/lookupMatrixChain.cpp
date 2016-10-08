#include <stdio.h>
#include <memory.h>

#define NUM 51
int p[NUM];
int m[NUM][NUM];
int s[NUM][NUM];

int LookupChain(int i, int j)
{
    if (m[i][j] > 0) return m[i][j];

    if (i == j) return 0;

    int u = LookupChain(i, i) + LookupChain(i + 1, j) + p[i - 1] * p[i] * p[j];
    s[i][j] = i;

    for (int k = i + 1; k < j; k++) {
        int t = LookupChain(i, k) + LookupChain(k + 1, j) + p[i - 1] * p[k] * p[j];

        if (t < u) {
            u = t;
            s[i][j] = k;
        }
    }

    m[i][j] = u;
    return u;
}

void TraceBack(int i, int j)
{
    if (i == j) printf("A%d", i);
    else {
        printf("(");
        TraceBack(i, s[i][j]);
        TraceBack(s[i][j] + 1, j);
        printf(")");
    }
}

int main()
{
    int n;
    scanf("%d", &n);
    int i, temp;

    for (i = 0; i < n; i++)
        scanf("%d%d", &p[i], &temp);

    p[n] = temp;
    memset(m, 0, sizeof(m));
    LookupChain(1, n);
    printf("%d\n", m[1][n]);
    TraceBack(1, n);
    return 0;
}
