//Graph.h 图基类的定义
#ifndef _GRAPH_H_
#define _GRAPH_H_
template<typename VerT, typename ArcT>class Graph
{
protected:
    vector<VerTNode<VerT, ArcT> > vexs;

    int ArcNum;
    bool Weight;
    bool Directed;
private:
    void DFS(int i, void(*visit)(VerT), vector<bool> &Visited)
    {
        Visited[i] = true;
        visit(GetVex(i));

        for (int j = FirstAdjVex(i); j >= 0; j = NextAdjVex(i, j))
            if (!Visited[j])
                DFS(j, visit, Visited);
    }
public:
    Graph()
    {
        ArcNum = 0;
    }
    virtual int FirstAdjVex(int i)const = 0;

    virtual int NextAdjVex(int i, int j)const = 0;

    void DFSTraverse(void(*visit)(VerT))
    {
        int i;
        vector<bool> Visited(vexs.size(), false);

        for (i = 0; i < vexs.size(); i++)
            if (!Visited[i])
                DFS(i, visit, Visited);

        cout << endl;
    }
    void BFSTraverse(void(*visit)(VerT))
    {
        int i, j, k;
        vector<bool> Visited(vexs.size(), false);
        queue<int> q;

        for (i = 0; i < vexs.size(); i++)
            if (!Visited[i]) {
                Visited[i] = true;
                visit(GetVex(i));
                q.push(i);

                while (!q.empty()) {
                    j = q.front();
                    q.pop();

                    for (k = FirstAdjVex(j); k >= 0; k = NextAdjVex(j, k))
                        if (!Visited[k]) {
                            Visited[k] = true;
                            visit(GetVex(k));
                            q.push(k);
                        }
                }
            }

        cout << endl;
    }
    int GetVexNumOrArcNum(bool f)const
    {
        if (f)
            return vexs.size();
        else
            return ArcNum;
    }
    bool GetDirectedOrWeight(bool f)const
    {
        if (f)
            return Directed;
        else
            return Weight;
    }
    VerT GetVex(int i)const
    {
        if (i >= vexs.size() || i < 0)
            exit(1);

        return vexs[i].vex;
    }
    int LocateVex(VerT u)const
    {
        for (int i = 0; i < vexs.size(); i++)
            if EQ(u, vexs[i].vex)
                return i;

        return -1;
    }
    bool PutVex(VerT v, VerT value)
    {
        int k = LocateVex(v);

        if (k < 0)
            return false;

        vexs[k].vex = value;
        return true;
    }
    void CreateFromFile(string FileName);
    void InsertVex(VerT v);
    bool DeleteVex(VerT v);
    bool InsertArc(VerT v, VerT w);
    bool DeleteArc(VerT v, VerT w);
    void Display()const;
};
#endif

