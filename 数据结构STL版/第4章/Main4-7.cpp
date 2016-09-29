//Main4-7.cpp 验证孩子-兄弟二叉链表结构的树或森林CSTree<T>类的成员函数
#include "C.h"
#include "CSTree.h"
#define BiTNode CSNode
typedef char T;
T Nil = '#';
#include "Func4-1.h"
void main()
{
    int i;
    CSTree<T> t, p;
    cout << "树t空否？" << boolalpha << t.TreeEmpty() << "。树的深度为" << t.TreeDepth(t.Root());
    CSNode<T> *q;
    T e, e1;
    string FileName = "F4-7.txt";
    t.CreateTreeFromFile(FileName);
    cout << endl << "由文件" << FileName << "创建树t后，树t空否？" << boolalpha << t.TreeEmpty();
    cout << endl << "树根为" << t.Value(t.Root()) << "，树的深度为";
    cout << t.TreeDepth(t.Root()) << endl << "层序遍历树t：";
    t.LevelOrderTraverse(Visit);
    cout << endl << "请输入待修改的结点的值 新值：";
    cin >> e >> e1;
    t.Assign(t.Point(e), e1);
    cout << "层序遍历修改后的树t：";
    t.LevelOrderTraverse(Visit);
    cout << endl << e1 << "的双亲是" << t.Parent(e1) << "，长子是" << t.LeftChild(e1);
    cout << "，下一个兄弟是" << t.RightSibling(e1) << endl;
    FileName = "F4-8.txt";
    p.CreateTreeFromFile(FileName);
    cout << "由文件" << FileName << "创建树p后，层序遍历树p：" << endl;
    p.LevelOrderTraverse(Visit);
    cout << endl << "将树p插到树t中，请输入t中p的双亲结点 子树序号：";
    cin >> e >> i;
    q = t.Point(e);
    t.InsertChild(q, i, p);
    cout << "层序遍历插入后的树t：";
    t.LevelOrderTraverse(Visit);
    cout << endl << "先根遍历插入后的树t：";
    t.PreOrderTraverse(t.Root(), Visit);
    cout << endl << "后根遍历插入后的树t：";
    t.PostOrderTraverse(t.Root(), Visit);
    cout << endl << "层序遍历插入后的树p：";
    p.LevelOrderTraverse(Visit);
    cout << endl << "删除树t中结点e的第i棵子树，请输入e i：";
    cin >> e >> i;
    q = t.Point(e);
    t.DeleteChild(q, i);
    cout << "层序遍历删除后的树t：" << endl;
    t.LevelOrderTraverse(Visit);
    cout << endl;
}

