#include<iostream>
#include<algorithm>
using namespace std;

#define N 1000
struct graph {
    int u, v, cost;
    void set(int a, int b, int w)
    {
        u = a, v = b, cost = w;
    }
};

graph d[N * (N + 1) / 2];

int father[N];
int Find(int x)
{
    if (father[x] == -1) return x;

    return father[x] = Find(father[x]);
}

bool Union(int x, int y)
{
    x = Find(x);
    y = Find(y);

    if (x == y) return false;

    if (x > y) father[x] = y;

    if (x < y) father[y] = x;

    return true;
}

int cmp(graph x, graph y)
{
    if (x.cost < y.cost) return true;

    return false;
}

int main()
{
    int n, m;

    while (scanf("%d%d", &n, &m) != EOF && n) {
        int i;
        int v, w, edge;

        for (i = 0; i < m; i++) {
            scanf("%d%d%d", &v, &w, &edge);
            d[i].set(v, w, edge);
        }

        sort(d, d + m, cmp);
        memset(father, -1, sizeof(father));
        int sum = 0;
        int count = 0;

        for (i = 0; i < n * (n + 1) / 2; i++) {
            if (Union(d[i].u, d[i].v)) {
                printf("%d %d\n", d[i].u, d[i].v);
                sum += d[i].cost;
                ++count;
            }

            if (count == n - 1) break;
        }

        printf("%d\n", sum);
    }
}
