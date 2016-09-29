//Main4-5.cpp 验证红黑树RBTree<T>类的成员函数
#include "C.h"
#include "RBTNode.h"
#include "C4-1.h"
#define BiTNode RBTNode
#include "Func4-2.h"
#include "Func4-4.h"
#include "RBTree.h"
void main()
{
    RBTree<T> t;
    T r, r1[4];
    bool k;
    ifstream fin("F4-5.txt");

    while (!fin.eof()) {
        if (InputFromFile(fin, r)) {
            k = t.Insert(r);

            if (k) {
                cout << "插入关键字为" << r.key << "的结点后，中序遍历红黑树t：" << endl;
                t.OrderTraverse(t.Root(), In, VisitRB);
                cout << endl << "先序遍历红黑树t：" << endl;
                t.OrderTraverse(t.Root(), Pre, VisitRB);
                cout << endl;
            } else {
                cout << "红黑树t中已存在关键字为" << r.key << "的数据，故(";
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
        k = t.BSDTree<T>::Delete(r1[i].key);

        if (k) {
            cout << "删除关键字为" << r1[i].key << "的结点后，中序遍历红黑树t：" << endl;
            t.OrderTraverse(t.Root(), In, VisitRB);
            cout << endl << "先序遍历红黑树t：" << endl;
            t.OrderTraverse(t.Root(), Pre, VisitRB);
            cout << endl;
        } else
            cout << "红黑树t中不存在关键字为" << r1[i].key << "的数据" << endl;
    }
}

