//Algo6-3.cpp 验证BTree类的成员函数
#include "C.h"
#include "C4-1.h"
typedef int KeyType;
const int EMPTY = 0;
#include "BTree.h"
#include "Func6-3.h"
void main()
{
    BTree<Record> t(3);
    const int j = 4;
    Result u[j];
    KeyType k;
    t.CreateFromFile("F6-4.txt");
    t.Print(Out);
    cout << "按关键字的顺序遍历B树t.root：" << endl;
    t.Traverse(t.Root(), Out);
    cout << endl;

    for (int i = 0; i < j; i++) {
        cout << "请输入待查找数据的关键字：";
        InputKey(k);
        u[i] = t.SearchBTree(k);
        cout << "u[" << i << "].i=" << u[i].i << " u[" << i << "].tag=" << boolalpha << u[i].tag << endl;

        if (u[i].tag)
            t.OutRecord(Out, u[i].pt->ki[u[i].i].index);
        else
            cout << "未找到关键字为" << k << "的数据。" << endl;
    }

    cout << "请输入待删除数据的关键字：";
    InputKey(k);
    Record r;
    bool f = t.DeleteBTree(r, k);

    if (f)
        Out(r);

    cout << endl << "按关键字的顺序遍历删除后的B树t.root：" << endl;
    t.Traverse(t.Root(), Out);
    cout << endl;
    t.Print(Out);
}

