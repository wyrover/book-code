//AdjListGraph.h 图的邻接表类（AdjListGraph类）
#ifndef _ADJLISTGRAPH_H_
#define _ADJLISTGRAPH_H_
#include "C.h"
template<typename ArcT>struct ArcTNode {
    int adjvex;
    ArcT arc;
};
template<typename VerT, typename ArcT>struct VerTNode {
    VerT vex;
    list<ArcTNode<ArcT> > arcs;
};
#include "Graph.h"
template<typename VerT, typename ArcT>class AdjListGraph: public Graph<VerT, ArcT>
{
public:
    void CreateFromFile(string FileName)
    {
        int i, j, k;
        VerT v1, v2;
        ArcTNode<ArcT> e;
        VerTNode<VerT, ArcT> v;
        ifstream fin(FileName.c_str());
        fin >> boolalpha >> Directed;
        fin >> boolalpha >> Weight;
        fin >> j;
        vexs.assign(j);

        for (i = 0; i < j; i++)
            InputVertexFromFile(fin, vexs[i].vex);

        fin >> ArcNum;

        for (k = 0; k < ArcNum; k++) {
            InputVertexNameFromFile(fin, v1);
            InputVertexNameFromFile(fin, v2);
            i = LocateVex(v1);
            j = LocateVex(v2);
            e.adjvex = j;

            if (Weight)
                fin >> e.arc.weight;

            InputArcFromFile(fin, e.arc);
            vexs[i].arcs.push_front(e);

            if (!Directed) {
                e.adjvex = i;
                vexs[j].arcs.push_front(e);
            }
        }

        fin.close();
    }
    void InsertVex(VerT v)
    {
        vexs.resize(vexs.size() + 1);
        vexs[vexs.size() - 1].vex = v;
    }
    bool DeleteVex(VerT v)
    {
        int i, k;
        list<ArcTNode<ArcT> >::iterator p;
        k = LocateVex(v);

        if (k < 0)
            return false;

        for (i = 0; i < vexs.size(); i++) {
            for (p = vexs[i].arcs.begin(); p != vexs[i].arcs.end(); p++)
                if (p->adjvex == k) {
                    vexs[i].arcs.erase(p);
                    ArcNum--;
                    break;
                }
        }

        if (Directed)
            ArcNum -= vexs[k].arcs.size();

        vexs.erase(vexs.begin() + k);

        for (i = 0; i < vexs.size(); i++) {
            for (p = vexs[i].arcs.begin(); p != vexs[i].arcs.end(); p++)
                if (p->adjvex > k)
                    p->adjvex--;
        }

        return true;
    }
    bool InsertArc(VerT v, VerT w)
    {
        ArcTNode<ArcT> e;
        int i, j;
        i = LocateVex(v);
        j = LocateVex(w);

        if (i < 0 || j < 0)
            return false;

        ArcNum++;
        e.adjvex = j;
        InputArc(Directed, Weight, v, w, e.arc);
        vexs[i].arcs.push_front(e);

        if (!Directed) {
            e.adjvex = i;
            vexs[j].arcs.push_front(e);
        }

        return true;
    }
    bool DeleteArc(VerT v, VerT w)
    {
        list<ArcTNode<ArcT> >::iterator p;
        int i, j;
        i = LocateVex(v);
        j = LocateVex(w);

        if (i < 0 || j < 0 || i == j)
            return false;

        for (p = vexs[i].arcs.begin(); p != vexs[i].arcs.end(); p++)
            if (p->adjvex == j) {
                vexs[i].arcs.erase(p);
                ArcNum--;
                break;
            }

        if (!Directed)
            for (p = vexs[j].arcs.begin(); p != vexs[j].arcs.end(); p++)
                if (p->adjvex == i) {
                    vexs[j].arcs.erase(p);
                    break;
                }

        return true;
    }
    int FirstAdjVex(int i)const
    {
        list<ArcTNode<ArcT> >::const_iterator p = vexs[i].arcs.begin();

        if (p != vexs[i].arcs.end())
            return p->adjvex;
        else
            return -1;
    }
    int NextAdjVex(int i, int j)const
    {
        list<ArcTNode<ArcT> >::const_iterator p;

        for (p = vexs[i].arcs.begin(); p != vexs[i].arcs.end(); p++)
            if (p->adjvex == j) {
                p++;
                break;
            }

        if (p == vexs[i].arcs.end())
            return -1;
        else
            return p->adjvex;
    }
    void Display()const
    {
        int i;
        list<ArcTNode<ArcT> >::const_iterator p;
        string s1 = "无向", s2 = "无权图";

        if (Directed)
            s1 = "有向";

        if (Weight)
            s2 = "带权图";

        cout << vexs.size() << "个顶点" << ArcNum << "条弧的" << s1 << s2 << "。顶点依次是：";

        for (i = 0; i < vexs.size(); i++)
            Visit(vexs[i].vex);

        cout << endl << ArcNum << "条弧：" << endl;

        for (i = 0; i < vexs.size(); i++) {
            for (p = vexs[i].arcs.begin(); p != vexs[i].arcs.end(); p++)
                if (Directed || i < p->adjvex)
                    OutputArc(Directed, Weight, vexs[i].vex, vexs[p->adjvex].vex, p->arc);

            cout << endl;
        }
    }
};
#endif

