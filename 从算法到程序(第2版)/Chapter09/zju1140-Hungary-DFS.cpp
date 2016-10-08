#include<cstdio>
#include<cstring>
#include<iostream>
using namespace std;

#define P 110
#define N 310
int p;
int n;
int map[P][N];
int match[N];
bool use[N];

bool DFS(int u)
{
    for (int v = 1; v <= n; ++v) {
        if (!use[v] && map[u][v]) {
            use[v] = true;

            if (match[v] == -1 || DFS(match[v])) {
                match[v] = u;
                return true;
            }
        }
    }

    return false;
}

int hungary()
{
    int course = 0;

    for (int i = 1; i <= p; ++i) {
        memset(use, false, sizeof(use));

        if (DFS(i))  course++;
    }

    return course;
}

int main()
{
    int iCase;
    scanf("%d", &iCase);

    while (iCase--) {
        int count;
        int student;
        memset(map, 0, sizeof(map));
        memset(match, -1, sizeof(match));
        scanf("%d%d", &p, &n);

        for (int i = 1; i <= p; ++i) {
            scanf("%d", &count);

            for (int j = 1; j <= count; ++j) {
                scanf("%d", &student);
                map[i][student] = 1;
            }
        }

        printf("%s\n", hungary() == p ? "YES" : "NO");
    }

    return 0;
}
