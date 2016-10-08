#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;

#define MAX 500
int n;
int map[MAX][MAX];
int match[MAX];
int use[MAX];

bool DFS(int stu)
{
    for (int i = 0; i < n; i++) {
        if (!use[i] && map[stu][i]) {
            use[i] = 1;

            if (match[i] == -1 || DFS(match[i])) {
                match[i] = stu;
                return true;
            }
        }
    }

    return false;
}

int hungary()
{
    int num = 0;
    memset(match, -1, sizeof(match));

    for (int i = 0; i < n; i++) {
        memset(use, 0, sizeof(use));

        if (DFS(i)) num++;
    }

    return n - num / 2;
}

int main()
{
    int i, j;
    int a, b, m;

    while (scanf("%d", &n) != EOF) {
        memset(map, 0, sizeof(map));

        for (i = 0; i < n; i++) {
            scanf("%d: (%d)", &a, &m);

            for (j = 0; j < m; j++) {
                scanf("%d", &b);
                map[a][b] = 1;
            }
        }

        printf("%d\n", hungary());
    }

    return 0;
}
