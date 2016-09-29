//Main5-4.cpp 验证用于自定义顶点和弧结构体的邻接表类的成员函数
#include "C5-1.h"
#include "AdjListGraph.h"
#include "Func5-3.h"
#include "Func5-6.h"
void main()
{
    int i, j, k, n;
    string s = "F5-3.txt";
    AdjListGraph<V, A> g[2];
    V v1, v2;

    for (i = 0; i < 2; i++) {
        s[3] = i + '3';
        g[i].CreateFromFile(s);
        g[i].Display();
        cout << "插入新顶点，请输入新顶点的值：";
        InputVertex(v1);
        g[i].InsertVex(v1);
        cout << "插入与新顶点有关的弧，请输入弧数：";
        cin >> n;

        for (k = 0; k < n; k++) {
            cout << "请输入第" << k + 1 << "条弧另一顶点的名称：";
            cin >> v2.name;

            if (g[i].GetDirectedOrWeight(true)) {
                cout << "请输入第" << k + 1 << "条弧另一顶点的方向（0：弧头 1：弧尾）：";
                cin >> j;

                if (j)
                    g[i].InsertArc(v2, v1);
                else
                    g[i].InsertArc(v1, v2);
            } else
                g[i].InsertArc(v1, v2);
        }

        g[i].Display();
        cout << "删除顶点及相关的弧，请输入待删除顶点的名称：";
        cin >> v1.name;
        g[i].DeleteVex(v1);
        g[i].Display();

        if (i == 1) {
            cout << "修改顶点的值，请输入待修改顶点名称：";
            cin >> v1.name;
            cout << "请输入新值：";
            InputVertex(v2);
            g[i].PutVex(v1, v2);
            cout << "删除一条弧，请输入待删除弧的弧尾 弧头：";
            cin >> v1.name >> v2.name;
            g[i].DeleteArc(v1, v2);
            g[i].Display();
        }
    }
}

