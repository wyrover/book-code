//Algo5-8.cpp 迪杰斯特拉算法，从某个源点到其余各顶点的最短路径
#include "C5-1.h"
#include "MatrixGraph.h"
template<typename VerT, typename ArcT>class ShortestPath: public MatrixGraph<VerT, ArcT>
{
private:
    vector<VRType> D;
    vector<vector<bool> >P;
    vector<bool> final;
    void Path(int v0, int v)
    {
        vector<bool> path = P[v];
        VRType min;
        int i, j = 0, k, m;

        for (i = 0; i < vexs.size(); i++)
            if (path[i])
                j++;

        path[v0] = false;

        if (j != 0)
            j--;

        k = v0;

        while (j > 0) {
            min = INFINITY;
            cout << vexs[k].vex.name << "→";

            for (i = 0; i < vexs.size(); i++)
                if (path[i] && vexs[k].arcs[i].adj < min)
                    min = vexs[k].arcs[i].adj, m = i;

            k = m;
            path[k] = false;
            j--;
        }

        if (k != v0)
            cout << vexs[k].vex.name << endl;
    }
public:
    void ShortestPath_DIJ(int v0)
    {
        int v, w, i, j;
        VRType min;
        final.assign(vexs.size(), false);
        D.assign(vexs.size(), 0);
        P.assign(vexs.size());

        for (v = 0; v < vexs.size(); v++) {
            D[v] = vexs[v0].arcs[v].adj;
            P[v].assign(vexs.size(), false);

            if (D[v] < INFINITY)
                P[v][v0] = P[v][v] = true;
        }

        final[v0] = true;

        for (i = 1; i < vexs.size(); i++) {
            min = INFINITY;

            for (w = 0; w < vexs.size(); w++)
                if (!final[w] && D[w] < min) {
                    v = w;
                    min = D[w];
                }

            final[v] = true;

            for (w = 0; w < vexs.size(); w++)
                if (!final[w] && min < INFINITY && vexs[v].arcs[w].adj <
                    INFINITY && (min + vexs[v].arcs[w].adj < D[w])) {
                    D[w] = min + vexs[v].arcs[w].adj;

                    for (j = 0; j < vexs.size(); j++)
                        P[w][j] = P[v][j];

                    P[w][w] = true;
                }
        }

        cout << "  i   最短路径向量P：" << endl;

        for (i = 0; i < vexs.size(); i++) {
            cout << setw(3) << i;

            for (j = 0; j < vexs.size(); j++)
                cout << setw(3) << P[i][j];

            cout << endl;
        }

        cout << vexs[v0].vex.name << "到各顶点的最短路径依次经过的顶点为" << endl;

        for (i = 0; i < vexs.size(); i++)
            Path(v0, i);

        cout << vexs[v0].vex.name << "到各顶点的最短路径长度为" << endl;

        for (i = 0; i < vexs.size(); i++)
            if (i != v0) {
                cout << vexs[v0].vex.name << "→" << vexs[i].vex.name << "：";

                if (D[i] < INFINITY)
                    cout << D[i] << endl;
                else
                    cout << "∞" << endl;
            }
    }
};
#include "Func5-1.h"
#include "Func5-2.h"
void main()
{
    ShortestPath<V, A> m;
    m.CreateFromFile("F5-12.txt");
    m.Display();
    m.ShortestPath_DIJ(0);
}

