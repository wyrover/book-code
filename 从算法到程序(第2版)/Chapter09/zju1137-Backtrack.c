#include<stdio.h>
#include<memory.h>
#define MAX 500

int n;
int match[MAX];
char vist[MAX];
int mate[MAX][20];

int DFS(int stu)
{
    int i, t;

    for (i = 1; i <= mate[stu][0]; i++) {
        if (!vist[mate[stu][i]]) {
            vist[mate[stu][i]] = 1;
            t = match[mate[stu][i]];
            match[mate[stu][i]] = stu;

            if (t == -1 || DFS(t)) return 1;

            match[mate[stu][i]] = t;
        }
    }

    return 0;
}

int hungary()
{
    int stu;
    int num = 0;
    memset(match, -1, sizeof(match));

    for (stu = 0; stu < n; stu++) {
        memset(vist, 0, sizeof(vist));

        if (DFS(stu)) num++;
    }

    return num;
}

int main()
{
    int i, j;
    int a, b, m;

    while (scanf("%d", &n) != EOF) {
        memset(mate, 0, sizeof(mate));

        for (i = 0; i < n; i++) {
            scanf("%d: (%d)", &a, &m);
            mate[a][0] = m;

            for (j = 1; j <= m; j++) {
                scanf("%d", &b);
                mate[a][j] = b;
            }
        }

        printf("%d\n", n - hungary() / 2);
    }

    return 0;
}
