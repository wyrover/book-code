//Main4-3.cpp 验证二叉链表结构的二叉排序树BSTree<T>类的成员函数
#include "C.h"
#include "BiTNode.h"
#include "Func4-2.h"
#include "C4-1.h"
#include "BSTree.h"
void main()
{
    BSTree<T> t;
    BiTNode<T> *p;
    KeyType j;
    T r;
    bool k;
    ifstream fin("F4-5.txt");

    while (!fin.eof()) {
        if (InputFromFile(fin, r)) {
            k = t.Insert(r);

            if (!k) {
                cout << "二叉排序树t中已存在关键字为" << r.key << "的数据，故(" << r.key;
                cout << ", " << r.others << ")无法插入到t中。" << endl;
            }
        }
    }

    fin.close();
    cout << "中序遍历二叉排序树t：" << endl;
    t.OrderTraverse(t.Root(), In, Visit);
    cout << endl << "先序遍历二叉排序树t：" << endl;
    t.OrderTraverse(t.Root(), Pre, Visit);
    cout << endl << "请输入待删除结点的关键字的值：";
    InputKey(j);
    p = t.SearchBST(t.Root(), j);

    if (p != NULL) {
        cout << "t中存在关键字为" << j << "的结点。";
        t.BSDTree<T>::Delete(j);
        cout << "删除此结点后，中序遍历二叉排序树t：" << endl;
        t.OrderTraverse(t.Root(), In, Visit);
        cout << endl << "先序遍历二叉排序树t：" << endl;
        t.OrderTraverse(t.Root(), Pre, Visit);
        cout << endl;
    } else
        cout << "t中不存在关键字为" << j << "的结点。" << endl;
}

