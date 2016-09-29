//Algo5-1.cpp MatrixGraph类对象调用深度优先和广度优先搜索遍历的程序
#include "C5-1.h"
#include "MatrixGraph.h"
#include "Func5-1.h"
#include "Func5-2.h"
void main()
{
    MatrixGraph<V, A> g;
    string s = "F5-5.txt";

    for (int i = 5; i < 7; i++) {
        s[3] = i + '0';
        g.CreateFromFile(s);
        cout << "MatrixGraph类深度优先搜索遍历图g的结果：";
        g.DFSTraverse(Visit);
        cout << "MatrixGraph类广度优先搜索遍历图g的结果：";
        g.BFSTraverse(Visit);
    }
}

