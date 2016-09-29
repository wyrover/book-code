//Main4-4.cpp 验证平衡二叉树AVLTree<T>类的成员函数
#include "C.h"
#include "AVLTNode.h"
#include "C4-1.h"
#include "C4-2.h"
#define BiTNode AVLTNode
#include "Func4-2.h"
#include "Func4-3.h"
#include "AVLTree.h"
void main()
{
    AVLTree<T> t;
    T r, r1[4];
    bool k;
    ifstream fin("F4-5.txt");

    while (!fin.eof()) {
        if (InputFromFile(fin, r)) {
            k = t.Insert(r);

            if (k) {
                cout << "插入关键字为" << r.key << "的结点后，中序遍历平衡二叉树t：" << endl;
                t.OrderTraverse(t.Root(), In, Visit);
                cout << endl << "先序遍历平衡二叉树t：" << endl;
                t.OrderTraverse(t.Root(), Pre, Visit);
                cout << endl;
            } else {
                cout << "平衡二叉树t中已存在关键字为" << r.key << "的数据，故(";
                cout << r.key << ", " << r.others << ")无法插入到t中。" << endl;
            }
        }
    }

    fin.close();
    r1[0].key = 70;
    r1[1].key = 90;
    r1[2].key = 99;
    r1[3].key = 50;

    for (int i = 0; i < 4; i++) {
        k = t.Delete(r1[i]);

        if (k) {
            cout << "删除结点(" << r1[i].key << ", " << r1[i].others << ")后，中序遍历";
            cout << "（关键字，平衡因子）平衡二叉树t：" << endl;
            t.OrderTraverse(t.Root(), In, VisitBf);
            cout << endl << "先序遍历（关键字，平衡因子）平衡二叉树t：" << endl;
            t.OrderTraverse(t.Root(), Pre, VisitBf);
            cout << endl;
        } else
            cout << "平衡二叉树t中不存在关键字为" << r1[i].key << "的数据" << endl;
    }
}

