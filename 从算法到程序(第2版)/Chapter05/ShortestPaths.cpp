#include<stdio.h>
#include<string.h>

#define NUM 100
#define maxint 10000

void dijkstra(int n, int v, int dist[], int prev[], int c[][NUM])
{
    int i, j;
    bool s[NUM];

    for (i = 1; i <= n; i++) {
        dist[i] = c[v][i];
        s[i] = false;

        if (dist[i] > maxint) prev[i] = 0;
        else prev[i] = v;
    }

    dist[v] = 0;
    s[v] = true;

    for (i = 1; i < n; i++) {
        int tmp = maxint;
        int u = v;

        for (j = 1; j <= n; j++)
            if (!(s[j]) && (dist[j] < tmp)) {
                u = j;
                tmp = dist[j];
            }

        s[u] = 1;

        for (j = 1; j <= n; j++)
            if (!(s[j]) && c[u][j] < maxint) {
                int newdist = dist[u] + c[u][j];

                if (newdist < dist[j]) {
                    dist[j] = newdist;
                    prev[j] = u;
                }
            }
    }
}

int main()
{
    int m, n;

    while (scanf("%d%d", &n, &m) != EOF && (m || n)) {
        int i, j;
        int dist[NUM] = {0};
        int prev[NUM] = {0};
        int c[NUM][NUM];
        memset(c, 1, sizeof(c));
        int v, w, edge;

        for (i = 0; i < m; i++) {
            scanf("%d%d%d", &v, &w, &edge);
            c[v][w] = edge;
        }

        dijkstra(n, 1, dist, prev, c);

        for (i = 2; i <= n; i++)
            printf("%d ", dist[i]);

        printf("\n");

        for (j = 2; j <= n; j++) {
            printf("%d", j);
            int t = prev[j];

            while (t != 1) {
                printf("-->%d", t);
                t = prev[t];
            }

            printf("-->1\n");
        }
    }

    return 0;
}
