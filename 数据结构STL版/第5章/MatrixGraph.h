//MatrixGraph.h 图的邻接矩阵类（MatrixGraph类）
#ifndef _MATRIXGRAPH_H_
#define _MATRIXGRAPH_H_
#include "C.h"
template<typename VerT, typename ArcT>struct VerTNode {
    VerT vex;
    vector<ArcT> arcs;
};
#include "Graph.h"
template<typename VerT, typename ArcT>class MatrixGraph: public Graph<VerT, ArcT>
{
public:
    void CreateFromFile(string FileName)
    {
        int i, j, k;
        VerT v1, v2;
        ArcT arc;
        ifstream fin(FileName.c_str());
        fin >> boolalpha >> Directed;
        fin >> boolalpha >> Weight;

        if (Weight)
            arc.adj = INFINITY;
        else
            arc.adj = 0;

        fin >> j;
        vexs.assign(j);

        for (i = 0; i < vexs.size(); i++) {
            InputVertexFromFile(fin, vexs[i].vex);
            vexs[i].arcs.assign(vexs.size(), arc);
        }

        if (Weight)
            for (i = 0; i < vexs.size(); i++)
                vexs[i].arcs[i].adj = 0;
        else
            arc.adj = 1;

        fin >> ArcNum;

        for (k = 0; k < ArcNum; k++) {
            InputVertexNameFromFile(fin, v1);
            InputVertexNameFromFile(fin, v2);

            if (Weight)
                fin >> arc.adj;

            i = LocateVex(v1);
            j = LocateVex(v2);
            vexs[i].arcs[j].adj = arc.adj;
            InputArcFromFile(fin, vexs[i].arcs[j]);

            if (!Directed)
                vexs[j].arcs[i] = vexs[i].arcs[j];
        }

        fin.close();
    }
    void InsertVex(VerT v)
    {
        int i;
        ArcT arc;

        if (Weight)
            arc.adj = INFINITY;
        else
            arc.adj = 0;

        for (i = 0; i < vexs.size(); i++)
            vexs[i].arcs.resize(vexs.size() + 1, arc);

        vexs.resize(vexs.size() + 1);
        vexs[vexs.size() - 1].vex = v;
        vexs[vexs.size() - 1].arcs.resize(vexs.size(), arc);
        vexs[vexs.size() - 1].arcs[vexs.size() - 1].adj = 0;
    }
    bool DeleteVex(VerT v)
    {
        int i, k;
        k = LocateVex(v);

        if (k < 0)
            return false;

        for (i = 0; i < vexs.size(); i++) {
            if (vexs[k].arcs[i].adj > 0 && vexs[k].arcs[i].adj != INFINITY)
                ArcNum--;

            if (Directed)
                if (vexs[i].arcs[k].adj > 0 && vexs[i].arcs[k].adj != INFINITY)
                    ArcNum--;
        }

        vexs.erase(vexs.begin() + k);

        for (i = 0; i < vexs.size(); i++)
            vexs[i].arcs.erase(vexs[i].arcs.begin() + k);

        return true;
    }
    bool InsertArc(VerT v, VerT w)
    {
        int i, j;
        i = LocateVex(v);
        j = LocateVex(w);

        if (i < 0 || j < 0)
            return false;

        ArcNum++;

        if (Weight) {
            cout << "请输入此弧的权值：";
            cin >> vexs[i].arcs[j].adj;
        } else
            vexs[i].arcs[j].adj = 1;

        InputArc(vexs[i].arcs[j]);

        if (!Directed)
            vexs[j].arcs[i] = vexs[i].arcs[j];

        return true;
    }
    bool DeleteArc(VerT v, VerT w)
    {
        int i, k;
        VRType j = 0;

        if (Weight)
            j = INFINITY;

        i = LocateVex(v);
        k = LocateVex(w);

        if (i < 0 || k < 0 || i == k)
            return false;

        if (vexs[i].arcs[k].adj != j) {
            vexs[i].arcs[k].adj = j;

            if (!Directed)
                vexs[k].arcs[i].adj = j;

            ArcNum--;
        }

        return true;
    }
    int FirstAdjVex(int i)const
    {
        VRType j = 0;

        if (Weight)
            j = INFINITY;

        for (int k = 0; k < vexs.size(); k++)
            if (k != i && vexs[i].arcs[k].adj != j)
                return k;

        return -1;
    }
    int NextAdjVex(int i, int j)const
    {
        VRType m = 0;

        if (Weight)
            m = INFINITY;

        if (vexs[i].arcs[j].adj != m)
            for (int k = j + 1; k < vexs.size(); k++)
                if (i != k && vexs[i].arcs[k].adj != m)
                    return k;

        return -1;
    }
    void Display()const
    {
        int i, j;
        VRType m = 0;

        if (Weight)
            m = INFINITY;

        string s1 = "无向", s2 = "无权图";

        if (Directed)
            s1 = "有向";

        if (Weight)
            s2 = "带权图";

        cout << vexs.size() << "个顶点" << ArcNum << "条弧的" << s1 << s2 << "。顶点依次是：";

        for (i = 0; i < vexs.size(); i++)
            Visit(vexs[i].vex);

        cout << endl << "arcs.adj：" << endl;

        for (i = 0; i < vexs.size(); i++) {
            for (j = 0; j < vexs.size(); j++)
                if (vexs[i].arcs[j].adj == INFINITY)
                    cout << setw(4) << "∞";
                else
                    cout << setw(4) << vexs[i].arcs[j].adj;

            cout << endl;
        }

        for (i = 0; i < vexs.size(); i++) {
            if (Directed) {
                for (j = 0; j < vexs.size(); j++)
                    if (i != j && vexs[i].arcs[j].adj != m)
                        OutputArc(Directed, vexs[i].vex, vexs[j].vex, vexs[i].arcs[j]);
            } else
                for (j = i + 1; j < vexs.size(); j++)
                    if (vexs[i].arcs[j].adj != m)
                        OutputArc(Directed, vexs[i].vex, vexs[j].vex, vexs[i].arcs[j]);
        }
    }
};
#endif

