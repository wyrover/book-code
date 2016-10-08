#include <iostream>
#include <queue>
using namespace std;

#define inf 1000000
#define NUM 100
int n;
int edge;
int c[NUM][NUM];
int prev[NUM];
int dist[NUM];

struct MinHeapNode {
    friend bool operator < (const MinHeapNode& a, const MinHeapNode& b)
    {
        if (a.length > b.length) return true;
        else return false;
    }
    int i;
    int length;
};

void ShortestPaths(int v)
{
    priority_queue <MinHeapNode, vector<MinHeapNode>, less<MinHeapNode> > H;
    MinHeapNode E;
    E.i = v;
    E.length = 0;
    dist[v] = 0;

    while (true) {
        for (int j = 1; j <= n; j++)
            if ((c[E.i][j] < inf) && (E.length + c[E.i][j] < dist[j])) {
                dist[j] = E.length + c[E.i][j];
                prev[j] = E.i;
                MinHeapNode N;
                N.i = j;
                N.length = dist[j];
                H.push(N);
            }

        if (H.empty()) break;
        else {
            E = H.top();
            H.pop();
        }
    }
}

int main()
{
    cin >> n >> edge;

    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= n; j++)
            c[i][j] = inf;

    int from, to, d;

    for (int i = 0; i < edge; i++) {
        cin >> from >> to >> d;
        c[from][to] = d;
    }

    int s, t;

    while (cin >> s >> t) {
        memset(prev, 0, sizeof(prev));

        for (int i = 0; i <= n; i++)
            dist[i] = inf;

        ShortestPaths(s);
        int best = dist[t];

        if (best == inf) cout << "No path!";
        else cout << best;

        cout << endl;
    }

    return 0;
}

