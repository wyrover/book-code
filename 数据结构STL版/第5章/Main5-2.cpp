//Main5-2.cpp 另一种顶点和弧结构体调用邻接矩阵类的成员函数
#include "C5-1.h"
#include "MatrixGraph.h"
#include "Func5-3.h"
#include "Func5-4.h"
void main()
{
    int i, j, k, n;
    string s = "F5-1.txt";
    MatrixGraph<V, A> g;
    V v1, v2;

    for (i = 3; i < 5; i++) {
        s[3] = i + '0';
        g.CreateFromFile(s);
        g.Display();
        cout << "插入新顶点，请输入新顶点的值：";
        InputVertex(v1);
        g.InsertVex(v1);
        cout << "插入与新顶点有关的弧，请输入弧数：";
        cin >> n;

        for (k = 0; k < n; k++) {
            cout << "请输入第" << k + 1 << "条弧另一顶点的名称：";
            cin >> v2.name;

            if (g.GetDirectedOrWeight(true)) {
                cout << "请输入第" << k + 1 << "条弧另一顶点的方向（0：弧头 1：弧尾）：";
                cin >> j;

                if (j)
                    g.InsertArc(v2, v1);
                else
                    g.InsertArc(v1, v2);
            } else
                g.InsertArc(v1, v2);
        }

        g.Display();
        cout << "删除顶点及相关的弧，请输入待删除顶点的名称：";
        cin >> v1.name;
        g.DeleteVex(v1);
        g.Display();

        if (i == 4) {
            cout << "修改顶点的值，请输入待修改顶点名称：";
            cin >> v1.name;
            cout << "请输入新值：";
            InputVertex(v2);
            g.PutVex(v1, v2);
            cout << "删除一条弧，请输入待删除弧的弧尾 弧头：";
            cin >> v1.name >> v2.name;
            g.DeleteArc(v1, v2);
            g.Display();
        }
    }
}

