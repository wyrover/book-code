//Main4-1.cpp 验证二叉链表结构的二叉树BiTree<T>类的成员函数
#include "C.h"
#include "BiTNode.h"
#include "BiTree.h"
typedef char T;
T Nil = '#';
#include "Func4-1.h"
void main()
{
    BiTree<T> t, c;
    BiTNode<T> *p, *q, *r, *s;
    T e;
    bool f, k;
    string LR;
    cout << "树t空否？" << boolalpha << t.BiTreeEmpty() << "。树t的深度=";
    cout << t.BiTreeDepth(t.Root()) << endl;
    ifstream fin("F4-1.txt");
    t.CreateBiTreeFromFile(fin);
    fin.close();
    cout << "由文件F4-1.txt创建二叉树t后，树t空否？" << boolalpha << t.BiTreeEmpty();
    cout << "。树t的深度=" << t.BiTreeDepth(t.Root()) << endl;
    cout << "先序递归遍历二叉树t：";
    t.OrderTraverse(t.Root(), Pre, Visit);
    cout << endl << "中序递归遍历二叉树t：";
    t.OrderTraverse(t.Root(), In, Visit);
    p = t.Root();
    cout << endl << "t的根结点=" << t.Value(p) << "。给根结点赋新值，请输入新值：";
    Input(e);
    t.Assign(p, e);
    t.Child(p, q, r);
    s = t.Parent(r);
    cout << "树t根结点的右孩子是" << t.Value(r) << "，其双亲是" << t.Value(s);
    f = t.Sibling(r, s, k);

    if (f) {
        if (!k)
            LR = "左";
        else
            LR = "右";

        cout << "，其" << LR << "兄弟是" << t.Value(s) << endl;
    }

    fin.open("F4-2.txt");
    c.CreateBiTreeFromFile(fin);
    fin.close();
    cout << "创建右子树为空的二叉树c后，中序非递归遍历二叉树c：";
    c.InOrderTraverse(Visit);
    cout << "后序递归遍历二叉树c：";
    c.OrderTraverse(c.Root(), Post, Visit);
    f = t.InsertChild(q, true, c);

    if (f) {
        cout << endl << "先序非递归遍历插入后的二叉树t：";
        t.PreOrderTraverse(Visit);
        cout << endl;
        cout << "中序非递归遍历插入后的二叉树t：";
        t.InOrderTraverse(Visit);
        cout << "先序递归遍历插入后的二叉树c：";
        c.OrderTraverse(c.Root(), Pre, Visit);
    }

    f = t.DeleteChild(q, true);

    if (f) {
        cout << endl << "后序非递归遍历删除后的二叉树t：";
        t.PostOrderTraverse(Visit);
        cout << endl << "中序递归遍历删除后的二叉树t：";
        t.OrderTraverse(t.Root(), In, Visit);
        cout << endl << "层序遍历删除后的二叉树t：";
        t.LevelOrderTraverse(Visit);
    }
}

