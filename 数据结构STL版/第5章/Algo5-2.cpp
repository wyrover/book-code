//Algo5-2.cpp AdjListGraph.h类对象调用深度优先和广度优先搜索遍历的程序
#include "C5-1.h"
#include "AdjListGraph.h"
#include "Func5-1.h"
#include "Func5-5.h"
void main()
{
    AdjListGraph<V, A> g[2];
    string s = "F5-5.txt";

    for (int i = 0; i < 2; i++) {
        s[3] = i + '5';
        g[i].CreateFromFile(s);
        cout << "AdjListGraph类深度优先搜索遍历图的结果：";
        g[i].DFSTraverse(Visit);
        cout << "AdjListGraph类广度优先搜索遍历图的结果：";
        g[i].BFSTraverse(Visit);
    }
}

