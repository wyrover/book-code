//Main4-6.cpp 验证伸展树SpTree<T>类的成员函数
#include "C.h"
#include "BiTNode.h"
#include "Func4-2.h"
#include "C4-1.h"
#include "SpTree.h"
void main()
{
    SpTree<T> t;
    T r;
    bool k;
    ifstream fin("F4-6.txt");

    while (!fin.eof()) {
        if (InputFromFile(fin, r)) {
            k = t.Insert(r);

            if (k) {
                cout << "插入关键字为" << r.key << "的结点后，中序遍历伸展树t：" << endl;
                t.OrderTraverse(t.Root(), In, Visit);
                cout << endl << "先序遍历伸展树t：" << endl;
                t.OrderTraverse(t.Root(), Pre, Visit);
                cout << endl;
            } else {
                cout << "伸展树t中已存在关键字为" << r.key << "的数据，故(";
                cout << r.key << ", " << r.others << ")无法插入到t中。" << endl;
            }
        }
    }

    fin.close();

    for (int i = 1; i <= 2; i++) {
        cout << "请输入待查找结点的关键字的值：";
        InputKey(r.key);
        k = t.Search(r.key);

        if (k)
            cout << "找到关键字为" << r.key << "的结点后，中序遍历伸展树t：" << endl;
        else
            cout << "没找到关键字为" << r.key << "的结点后，中序遍历伸展树t：" << endl;

        t.OrderTraverse(t.Root(), In, Visit);
        cout << endl << "先序遍历伸展树t：" << endl;
        t.OrderTraverse(t.Root(), Pre, Visit);
        cout << endl;
    }

    cout << "请输入待删除结点的关键字的值：";
    InputKey(r.key);
    k = t.BSDTree<T>::Delete(r.key);
    cout << "删除关键字为" << r.key << "的结点后，中序遍历伸展树t：" << endl;
    t.OrderTraverse(t.Root(), In, Visit);
    cout << endl << "先序遍历伸展树t：" << endl;
    t.OrderTraverse(t.Root(), Pre, Visit);
    cout << endl;
}

