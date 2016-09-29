//Topo.h 拓扑类（Topo类）
#ifndef _TOPO_H_
#define _TOPO_H_
#include "AdjListGraph.h"
template<typename VerT, typename ArcT>class Topo: public AdjListGraph<VerT, ArcT>
{
private:
    vector<int> indegree;
    vector<int> ve;
    vector<int> vl;
    void FindInDegree()
    {
        int i;
        list<ArcTNode<ArcT> >::const_iterator p;
        indegree.assign(vexs.size(), 0);

        for (i = 0; i < vexs.size(); i++)
            for (p = vexs[i].arcs.begin(); p != vexs[i].arcs.end(); p++)
                indegree[p->adjvex]++;
    }
    bool TopologicalOrder(stack<int> &T)
    {
        int i, k, count = 0;
        stack<int> S;
        list<ArcTNode<ArcT> >::const_iterator p;
        FindInDegree();
        cout << "一个拓扑序列：";

        for (i = 0; i < vexs.size(); i++)
            if (indegree[i] == 0)
                S.push(i);

        ve.assign(vexs.size(), 0);

        while (!S.empty()) {
            i = S.top();
            S.pop();
            Visit(vexs[i].vex);
            T.push(i);
            count++;

            for (p = vexs[i].arcs.begin(); p != vexs[i].arcs.end(); p++) {
                k = p->adjvex;

                if (--indegree[k] == 0)
                    S.push(k);

                if (ve[i] + p->arc.weight > ve[k])
                    ve[k] = ve[i] + p->arc.weight;
            }
        }

        if (count < vexs.size()) {
            cout << "此有向带权图有回路" << endl;
            return false;
        } else
            return true;
    }
public:
    bool TopologicalSort()
    {
        int i, k, count = 0;
        stack<int> S;
        list<ArcTNode<ArcT> >::const_iterator p;
        FindInDegree();

        for (i = 0; i < vexs.size(); i++)
            if (!indegree[i])
                S.push(i);

        while (!S.empty()) {
            i = S.top();
            S.pop();
            cout << vexs[i].vex.name << " ";
            count++;

            for (p = vexs[i].arcs.begin(); p != vexs[i].arcs.end(); p++) {
                k = p->adjvex;

                if (!(--indegree[k]))
                    S.push(k);
            }
        }

        if (count < vexs.size()) {
            cout << "此有向图有回路" << endl;
            return false;
        } else {
            cout << "为一个拓扑序列。" << endl;
            return true;
        }
    }
    bool CriticalPath()
    {
        stack<int> S;
        int i, j;
        list<ArcTNode<ArcT> >::const_iterator p;
        bool f = TopologicalOrder(S);

        if (!f)
            return false;

        j = S.top();
        vl.assign(vexs.size(), ve[j]);

        while (!S.empty()) {
            j = S.top();
            S.pop();

            for (p = vexs[j].arcs.begin(); p != vexs[j].arcs.end(); p++) {
                i = p->adjvex;

                if (vl[i] - p->arc.weight < vl[j])
                    vl[j] = vl[i] - p->arc.weight;
            }
        }

        cout << endl << "j   i  权值  j最早发生时间  i最迟发生时间" << endl;

        for (j = 0; j < vexs.size(); j++)
            for (p = vexs[j].arcs.begin(); p != vexs[j].arcs.end(); p++) {
                i = p->adjvex;
                OutputArc(Directed, Weight, vexs[j].vex, vexs[i].vex, p->arc);
                cout << setw(10) << ve[j] << setw(12) << vl[i];

                if (ve[j] + p->arc.weight == vl[i])
                    cout << " 关键活动";

                cout << endl;
            }

        return true;
    }
};
#endif

