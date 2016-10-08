#include <iostream>
#include <queue>
using namespace std;

#define inf 1000000
class Graph
{
    friend int path(int n, int s, int t, int **c);
public:
    void ShortestPaths(int);
private:
    int n,              //图G的顶点数
        *prev,          //前驱顶点数组
        **c,            //图G的邻接矩阵
        *dist;          //最短距离数组
};

class MinHeapNode
{
    friend class Graph;
public:
    friend bool operator < (MinHeapNode a, MinHeapNode b)
    {
        if (a.length < b.length) return true;
        else return false;
    }
private:
    int i;          //顶点编号
    int length;     //当前路长
};

//单源最短路径问题的优先队列分支限界法
void Graph :: ShortestPaths(int v)
{
    //定义优先队列,不需要大小
    priority_queue<MinHeapNode> H;
    //定义源为初始扩展结点
    MinHeapNode E;
    E.i = v;
    E.length = 0;
    dist[v] = 0;

    //搜索问题的解空间
    while (true) {
        for (int j = 1; j <= n; j++)
            if ((c[E.i][j] < inf) && (E.length + c[E.i][j] < dist[j])) {
                // 顶点i到顶点j可达，且满足控制约束
                dist[j] = E.length + c[E.i][j];
                prev[j] = E.i;
                // 加入活结点优先队列
                MinHeapNode N;
                N.i = j;
                N.length = dist[j];
                H.push(N);
            }

        if (H.empty()) break;   // 优先队列空
        else {
            E = H.top();
            H.pop();            // 取下一扩展结点
        }
    }
}

int path(int n, int s, int t, int **c)
{
    Graph g;
    g.n = n;
    g.prev = new int [n + 1];
    g.dist = new int [n + 1];
    memset(g.prev, 0, sizeof(g.prev));

    for (int i = 0; i <= n; i++)
        g.dist[i] = inf;

    g.c = c;
    g.ShortestPaths(s);
    int result = g.dist[t];
    delete [] g.prev;
    delete [] g.dist;
    return result;
}

int main()
{
    int n;
    int edge;
    cin >> n >> edge;
    int *c[n + 1];

    for (int i = 0; i <= n; i++)
        c[i] = new int [n + 1];

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
        int dist = path(n, s, t, c);

        if (dist == inf) cout << "No path!";
        else cout << dist;

        cout << endl;
    }

    for (int i = 0; i <= n; i++)
        delete []c[i];

    return 0;
}
