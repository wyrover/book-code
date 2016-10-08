#include <cstring>
#include <cstdio>
#include <iostream>
using namespace std;
const int MAX = 510;
struct edge {
    int to, next;
} e[MAX * MAX];
int p[MAX];
int match[MAX];
bool vist[MAX];

bool DFS(int stu)
{
    for (int x = p[stu]; x != -1; x = e[x].next) {
        int v = e[x].to;

        if (!vist[v]) {
            vist[v] = true;

            if (match[v] == -1 || DFS(match[v])) {
                match[v] = stu;
                return true;
            }
        }
    }

    return false;
}

int hungary(int n)
{
    memset(match, -1, sizeof(match));
    int num = 0;

    for (int i = 0; i < n; i++) {
        memset(vist, 0, sizeof(vist));

        if (DFS(i))  num++;
    }

    return n - num / 2;
}

int main()
{
    int n;

    while (scanf("%d", &n) != EOF) {
        int id = 0;
        memset(p, -1, sizeof(p));

        for (int i = 0; i < n; i++) {
            int x, num;
            scanf("%d: (%d)", &num, &num);

            for (int j = 1; j <= num; j++) {
                scanf("%d", &x);
                e[id].to = x;
                e[id].next = p[i];
                p[i] = id++;
            }
        }

        printf("%d\n", hungary(n));
    }

    return 0;
}
