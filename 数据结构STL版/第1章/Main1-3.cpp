//Main1-3.cpp 验证单链表LinkList<T>类的成员函数
#include "C.h"
#include "LinkList.h"
typedef int T;
#include "Func1-1.h"
void main()
{
    bool i;
    int j, k;
    T e, e0;
    LinkList<T> L;

    for (j = 1; j <= 5; j++)
        i = L.ListInsert(1, j);

    cout << "在L的表头依次插入1～5后，L=";
    L.ListTraverse(print);
    cout << "L是否空？" << boolalpha << L.ListEmpty() << "，表L的长度=";
    cout << L.ListLength() << endl;

    for (j = 0; j <= 1; j++) {
        k = L.LocateElem(j, equal);

        if (k)
            cout << "值为" << j << "的元素是第" << k << "个元素" << endl;
        else
            cout << "没有值为" << j << "的元素，";
    }

    for (j = 1; j <= 2; j++) {
        L.GetElem(j, e0);
        i = L.PriorElem(e0, equal, e);

        if (i)
            cout << "元素" << e0 << "的前驱为" << e << endl;
        else
            cout << "元素" << e0 << "无前驱，";
    }

    for (j = L.ListLength(); j >= L.ListLength() - 1; j--) {
        L.GetElem(j, e0);
        i = L.NextElem(e0, equal, e);

        if (i)
            cout << "元素" << e0 << "的后继为" << e << endl;
        else
            cout << "元素" << e0 << "无后继，";
    }

    k = L.ListLength();

    for (j = k + 1; j >= k; j--) {
        i = L.ListDelete(j, e);

        if (i)
            cout << "删除第" << j << "个元素成功，其值为" << e << endl;
        else
            cout << "删除第" << j << "个元素失败（不存在此元素），";
    }

    L.ClearList();
    cout << "清空表L后，L是否空？" << boolalpha << L.ListEmpty() << "，表L的长度=";
    cout << L.ListLength() << endl;
}

