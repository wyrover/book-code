//Main4-8.cpp 用CSTree<T>类构造森林
#include "C.h"
#include "CSTree.h"
#define BiTNode CSNode
typedef char T;
#include "Func4-1.h"
void main()
{
    CSTree<T>t;
    string FileName = "F4-9.txt";
    t.CreateTreeFromFile(FileName);
    cout << "由文件" << FileName << "创建森林t后，森林的深度（所有树中深度最大的树的深度）为";
    cout << t.TreeDepth(t.Root()) << endl << "层序遍历森林t：";
    t.LevelOrderTraverse(Visit);
    cout << endl << "先根遍历森林t：";
    t.PreOrderTraverse(t.Root(), Visit);
    cout << endl << "后根遍历森林t：";
    t.PostOrderTraverse(t.Root(), Visit);
    cout << endl;
}

