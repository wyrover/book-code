//Main1-4.cpp 验证双向循环链表DLinkList<T>类的成员函数
#include "C.h"
#include "DLinkList.h"
typedef int T;
#include "Func1-1.h"
void main()
{
    DLinkList<T> L;
    int i, n = 4;
    bool j;
    T e;

    for (i = 1; i <= 5; i++)
        L.ListInsert(i, i);

    cout << "在L的表尾依次插入1～5后，L=";
    L.ListTraverse(print);
    j = L.GetElem(2, e);

    if (j)
        cout << "链表的第2个元素值为" << e << endl;
    else
        cout << "不存在第2个元素" << endl;

    i = L.LocateElem(n, equal);

    if (i)
        cout << "等于" << n << "的元素是第" << i << "个" << endl;
    else
        cout << "没有等于" << n << "的元素" << endl;

    j = L.PriorElem(n, equal, e);

    if (j)
        cout << n << "的前驱是" << e << endl;
    else
        cout << "不存在" << n << "的前驱" << endl;

    j = L.NextElem(n, equal, e);

    if (j)
        cout << n << "的后继是" << e << endl;
    else
        cout << "不存在" << n << "的后继" << endl;

    L.ListDelete(2, e);
    cout << "删除第2个结点，值为" << e << "，逆序输出其余结点：";
    L.ListBackTraverse(print);
    cout << "链表的元素个数为" << L.ListLength() << "，";
    cout << "链表是否空？" << boolalpha << L.ListEmpty() << endl;
    L.ClearList();
    cout << "清空后，链表是否空？" << boolalpha << L.ListEmpty() << endl;
}

