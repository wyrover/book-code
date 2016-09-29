//Algo5-4.cpp 构造最小生成树
#include "C5-1.h"
#include "MatrixGraph.h"
template<typename VerT, typename ArcT>class MSTree: public MatrixGraph<VerT, ArcT>
{
private:
    struct closedge {
        int adjv;
        VRType lowc;
    };
    vector<closedge> mindge;
    int minimum()
    {
        int i = 0, j, k, min;

        while (mindge[i].lowc == 0)
            i++;

        min = mindge[i].lowc;
        k = i;

        for (j = i + 1; j < vexs.size(); j++)
            if (mindge[j].lowc > 0 && mindge[j].lowc < min) {
                min = mindge[j].lowc;
                k = j;
            }

        return k;
    }
    struct side {
        int a, b;
        VRType weight;
    };
    struct cmp {
        bool operator()(const side &a, const side &b)const
        {
            return a.weight > b.weight;
        }
    };
public:
    void MiniSpanTree_PRIM(int k)
    {
        mindge.assign(vexs.size());

        if (vexs.size() > 0 && Weight && !Directed) {
            int i, j, m;

            for (j = 0; j < vexs.size(); j++) {
                mindge[j].adjv = k;
                mindge[j].lowc = vexs[k].arcs[j].adj;
            }

            cout << "普里姆算法构造的最小代价生成树的各条弧为" << endl;

            for (i = 1; i < vexs.size(); i++) {
                m = minimum();
                cout << vexs[mindge[m].adjv].vex.name << '-' << vexs[m].vex.name << endl;
                mindge[m].lowc = 0;

                for (j = 0; j < vexs.size(); j++)
                    if (vexs[m].arcs[j].adj < mindge[j].lowc) {
                        mindge[j].adjv = m;
                        mindge[j].lowc = vexs[m].arcs[j].adj;
                    }
            }
        }
    }
    void Kruskal()
    {
        vector<int> set(vexs.size());

        if (vexs.size() > 0 && Weight && !Directed) {
            priority_queue<side, vector<side>, cmp> q;
            int i, j, k;
            side se;

            for (i = 0; i < vexs.size(); i++)
                for (j = i + 1; j < vexs.size(); j++)
                    if (vexs[i].arcs[j].adj < INFINITY) {
                        se.a = i;
                        se.b = j;
                        se.weight = vexs[i].arcs[j].adj;
                        q.push(se);
                    }

            for (i = 0; i < vexs.size(); i++)
                set[i] = i;

            cout << "克鲁斯卡尔算法构造的最小代价生成树的各条弧为" << endl;
            k = 0;

            while (k < vexs.size() - 1) {
                if (!q.empty()) {
                    se = q.top();
                    q.pop();

                    if (set[se.a] != set[se.b]) {
                        cout << vexs[se.a].vex.name << '-' << vexs[se.b].vex.name << endl;
                        j = set[se.b];

                        for (i = 0; i < vexs.size(); i++)
                            if (set[i] == j)
                                set[i] = set[se.a];

                        k++;
                    }
                } else {
                    cout << "无法构成最小生成树。" << endl;
                    break;
                }
            }
        }
    }
};
#include "Func5-1.h"
#include "Func5-2.h"
void main()
{
    MSTree<V, A> m;
    m.CreateFromFile("F5-8.txt");
    m.Display();
    m.MiniSpanTree_PRIM(0);
    m.Kruskal();
}

