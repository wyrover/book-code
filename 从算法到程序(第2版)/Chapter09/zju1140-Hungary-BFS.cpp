#include<cstdio>
#include<cstring>
#include<queue>
#include<iostream>
using namespace std;

#define P 110
#define N 310
int p;
int n;
int map[P][N];
int match[N];
bool use[N];

int hungary()
{
    int cx[N];
    int cy[N];
    memset(cx, -1, sizeof(cx));
    memset(cy, -1, sizeof(cy));
    queue <int> q;
    int course = 0;

    for (int i = 1; i <= p; i++) {
        while (!q.empty()) q.pop();

        q.push(i);
        bool exist = false;
        memset(use, 0, sizeof(use));
        memset(match, 0, sizeof(match));

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v = 1; v <= n  && !exist; v++) {
                if (map[u][v] && !use[v]) {
                    use[v] = 1;

                    if (cy[v] == -1) {
                        exist = true;
                        int d = u, e = v;

                        while (d != 0) {
                            cy[e] = d;
                            swap(cx[d], e);
                            d = match[d];
                        }
                    } else {
                        match[cy[v]] = u;
                        q.push(cy[v]);
                    }
                }
            }
        }

        if (exist) course++;
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
