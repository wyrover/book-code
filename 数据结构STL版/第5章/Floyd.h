//Floyd.h 弗洛伊德最短路径类（Floyd类）
#ifndef _FLORD_H_
#define _FLORD_H_
#pragma warning(disable:4786)
//避免由于“vector<vector<vector<bool> > >P;”语句出现的警告
#include "MatrixGraph.h"
template<typename VerT, typename ArcT>class Floyd: public MatrixGraph<VerT, ArcT>
{
private:
    vector<vector<VRType> > D;
    vector<vector<vector<bool> > >P;
    void DisplayPath(vector<int>pa)const
    {
        cout << setw(3);

        for (int i = 0; i < pa.size() - 1; i++)
            cout << vexs[pa[i]].vex.name << "→";

        cout << vexs[pa[pa.size() - 1]].vex.name << "："
             << D[pa[0]][pa[pa.size() - 1]] << endl;
    }
public:
    void ShortestPath_FLOYD()
    {
        int u, v, w, i;
        P.assign(vexs.size());
        D.assign(vexs.size());

        for (v = 0; v < vexs.size(); v++) {
            D[v].assign(vexs.size());
            P[v].assign(vexs.size());

            for (w = 0; w < vexs.size(); w++) {
                D[v][w] = vexs[v].arcs[w].adj;
                P[v][w].assign(vexs.size(), false);

                if (D[v][w] < INFINITY)
                    P[v][w][v] = P[v][w][w] = true;
            }
        }

        for (u = 0; u < vexs.size(); u++)
            for (v = 0; v < vexs.size(); v++) {
                if (u == v)
                    continue;

                for (w = 0; w < vexs.size(); w++) {
                    if (w == u || w == v)
                        continue;

                    if (D[v][u] < INFINITY && D[u][w] < INFINITY && D[v][u] + D[u][w] < D[v][w]) {
                        D[v][w] = D[v][u] + D[u][w];

                        for (i = 0; i < vexs.size(); i++)
                            P[v][w][i] = P[v][u][i] || P[u][w][i];
                    }
                }
            }
    }
    void Path(int i, int j)const
    {
        vector<bool> p = P[i][j];
        int k, n, m = i;
        VRType min;
        vector<int> pa;

        if (D[i][j] < INFINITY) {
            pa.push_back(i);
            p[i] = false;

            while (m != j) {
                min = INFINITY;

                for (k = 0; k < vexs.size(); k++)
                    if (vexs[m].arcs[k].adj < min && p[k]) {
                        min = vexs[m].arcs[k].adj;
                        n = k;
                    }

                pa.push_back(n);
                p[n] = false;
                m = n;
            }

            DisplayPath(pa);
        } else
            cout << vexs[i].vex.name << "到" << vexs[j].vex.name << "没有路径可通。" << endl;
    }
    void DisplayD()const
    {
        cout << "D：" << endl;

        for (int i = 0; i < vexs.size(); i++) {
            for (int j = 0; j < vexs.size(); j++)
                if (D[i][j] == INFINITY)
                    cout << setw(3) << "∞";
                else
                    cout << setw(3) << D[i][j];

            cout << endl;
        }
    }
    void DisplayP(int i, int j)const
    {
        cout << "P[" << i << "][" << j << "]=";

        for (int k = 0; k < vexs.size(); k++)
            cout << setw(2) << P[i][j][k];
    }
};
#endif

