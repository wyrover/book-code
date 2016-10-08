#include <stdio.h>

#define NUM 100
int tri[NUM][NUM];
int triangle(int n)
{
    int i, j;

    for (i = n - 2; i >= 0; i--)
        for (j = 0; j <= i; j++)
            if (tri[i + 1][j] > tri[i + 1][j + 1])
                tri[i][j] += tri[i + 1][j];
            else tri[i][j] += tri[i + 1][j + 1];

    return  tri[0][0];
}

int main()
{
    int n;
    scanf("%d", &n);
    int i, j;

    for (i = 0; i < n; i++)
        for (j = 0; j <= i; j++)
            scanf("%d", &tri[i][j]);

    printf("%d\n", triangle(n));
    return 0;
}
