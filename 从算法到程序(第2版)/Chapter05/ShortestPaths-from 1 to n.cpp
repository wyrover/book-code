#include <iostream>
using namespace std;

#define NUM 1000
#define maxint 10000000

void Dijkstra(int n, int v, int dist[], int prev[], int c[][NUM])
{
    bool s[NUM];

    for (int i = 1; i <= n; ++i) {
        dist[i] = c[v][i];
        s[i] = false;

        if (dist[i] > maxint) prev[i] = 0;
        else  prev[i] = v;
    }

    dist[v] = 0;
    s[v] = true;

    for (int i = 1; i < n; ++i) {
        int temp = maxint;
        int u = v;

        for (int j = 1; j <= n; ++j)
            if ((!s[j]) && (dist[j] < temp)) {
                u = j;
                temp = dist[j];
            }

        s[u] = true;

        for (int j = 1; j <= n; ++j)
            if ((!s[j]) && c[u][j] < maxint) {
                int newdist = dist[u] + c[u][j];

                if (newdist < dist[j]) {
                    dist[j] = newdist;
                    prev[j] = u;
                }
            }
    }

    for (int i = 2; i <= n; ++i)
        printf("%d ", dist[i]);

    printf("\n");
}

void traceback(int v, int i, int prev[])
{
    cout << i << "<--";
    i = prev[i];

    if (i != v) traceback(v, i, prev);

    if (i == v) cout << i << endl;
}

int dist[NUM];
int prev[NUM];
int c[NUM][NUM];
int main()
{
    int n, m;

    while (scanf("%d%d", &n, &m) != EOF && n) {
        memset(dist, 0, sizeof(dist));
        memset(prev, 0, sizeof(prev));
        memset(c, 1, sizeof(c));
        int s, t, edge;

        for (int i = 0; i < m; i++) {
            scanf("%d%d%d", &s, &t, &edge);
            c[s][t] = edge;
        }

        Dijkstra(n, 1, dist, prev, c);
        traceback(1, n, prev);
    }

    return 0;
}
