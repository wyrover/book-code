#include <stdio.h>

#define NUM 51
int p[NUM];
int m[NUM][NUM];
int s[NUM][NUM];

void MatrixChain(int n)
{
    for (int i = 1; i <= n; i++) m[i][i] = 0;

    for (int r = 2; r <= n; r++)
        for (int i = 1; i <= n - r + 1; i++) {
            int j = i + r - 1;
            m[i][j] = m[i + 1][j] + p[i - 1] * p[i] * p[j];
            s[i][j] = i;

            for (int k = i + 1; k < j; k++) {
                int t = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j];

                if (t < m[i][j]) {
                    m[i][j] = t;
                    s[i][j] = k;
                }
            }
        }
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
    MatrixChain(n);
    printf("%d\n", m[1][n]);
    TraceBack(1, n);
    return 0;
}
