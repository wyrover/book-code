//Algo5-5.cpp 求连通图关节点的算法
#include "C5-1.h"
#include "AdjListGraph.h"
template<typename VerT, typename ArcT>class Articul:
    public AdjListGraph<VerT, ArcT>
{
private:
    int count, lowcount;
    vector<int> Visited, low, lowOrder;
    void DFSArticul(int v0)
    {
        int min, w;
        list<ArcTNode<ArcT> >::const_iterator p;
        Visited[v0] = min = ++count;

        for (p = vexs[v0].arcs.begin(); p != vexs[v0].arcs.end(); p++) {
            w = p->adjvex;

            if (Visited[w] == 0) {
                DFSArticul(w);

                if (low[w] < min)
                    min = low[w];
                else if (low[w] >= Visited[v0])
                    cout << v0 << ' ' << vexs[v0].vex.name << endl;
            } else if (Visited[w] < min)
                min = Visited[w];
        }

        low[v0] = min;
        lowOrder[v0] = lowcount++;
    }
public:
    void FindArticul()
    {
        Visited.assign(vexs.size());
        low.assign(vexs.size());
        lowOrder.assign(vexs.size());
        int i, v;
        list<ArcTNode<ArcT> >::const_iterator p;
        count = 1;
        lowcount = 1;
        Visited[0] = count;

        for (i = 1; i < GetVexNumOrArcNum(true); i++)
            Visited[i] = 0;

        p = vexs[0].arcs.begin();
        v = p->adjvex;
        DFSArticul(v);

        if (count < vexs.size()) {
            cout << "0 " << vexs[0].vex.name << endl;
            p++;

            while (p != vexs[0].arcs.end()) {
                v = p->adjvex;

                if (Visited[v] == 0)
                    DFSArticul(v);

                p++;
            }
        }

        cout << " i name Visited low lowOrder" << endl;
        cout << setw(2) << 0 << setw(4) << vexs[0].vex.name;
        cout << setw(6) << Visited[0] << endl;

        for (i = 1; i < vexs.size(); i++)
            cout << setw(2) << i << setw(4) << vexs[i].vex.name << setw(6)
                 << Visited[i] << setw(6) << low[i] << setw(7)
                 << lowOrder[i] << endl;
    }
};
#include "Func5-1.h"
#include "Func5-5.h"
void main()
{
    Articul<V, A> a;
    a.CreateFromFile("F5-9.txt");
    a.Display();
    cout << "输出关节点：" << endl;
    a.FindArticul();
}

