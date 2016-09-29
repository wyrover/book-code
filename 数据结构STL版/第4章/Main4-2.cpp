//Main4-2.cpp 验证三指针链表结构的二叉树PBiTree<T>类的成员函数
#include "C.h"
#include "PBiTNode.h"
#include "PBiTree.h"
typedef int T;
T Nil = 0;
#include "Func4-1.h"
void Visit1(PBiTNode<T> &c)
{
    cout << '(' << c.data;

    if (c.parent != NULL)
        cout << ", " << c.parent->data;

    cout << ')';
}
void main()
{
    bool f, k;
    string LR;
    PBiTree<T> t, c;
    PBiTNode<T> *p, *q, *r, *s;
    ifstream fin("F4-3.txt");
    t.CreateBiTreeFromFile(fin);
    fin.close();
    cout << "由文件F4-3.txt创建二叉树t后，先序递归遍历二叉树t：";
    t.OrderTraverse(t.Root(), Pre, Visit);
    cout << endl << "中序递归遍历深度为" << t.BiTreeDepth(t.Root()) << "的二叉树t：";
    t.OrderTraverse(t.Root(), In, Visit);
    fin.open("F4-4.txt");
    c.CreateBiTreeFromFile(fin);
    fin.close();
    cout << endl << "创建右子树为空的二叉树c后，先序递归遍历二叉树c：";
    c.OrderTraverse(c.Root(), Pre, Visit);
    cout << endl << "中序递归遍历二叉树c：";
    c.OrderTraverse(c.Root(), In, Visit);
    p = t.Root();
    t.Child(p, q, r);
    f = t.InsertChild(q, true, c);

    if (f) {
        cout << endl << "先序递归遍历插入后的二叉树t：";
        t.OrderTraverse(t.Root(), Pre, Visit);
        cout << endl << "中序递归遍历插入后的二叉树t：";
        t.OrderTraverse(t.Root(), In, Visit);
    }

    p = t.Root();
    t.Child(p, q, r);
    t.Child(q, p, r);
    p = t.Parent(r);
    cout << endl;
    cout << "树t根结点的左孩子的右孩子是" << t.Value(r) << "，其双亲是" << t.Value(p);
    f = t.Sibling(r, s, k);

    if (f) {
        if (!k)
            LR = "左";
        else
            LR = "右";

        cout << "，其" << LR << "兄弟是" << t.Value(s) << endl;
    }

    cout << "树t每个结点的双亲：" << endl;
    t.OrderTraverse(t.Root(), Pre, Visit1);
    cout << endl;
}

