//Main1-5.cpp 验证静态链表StLinkList<T>类的成员函数
#include "C.h"
#include "StLinkList.h"
typedef int T;
#include "Func1-1.h"
void main()
{
    StLinkList<T> L;
    T e, e0 = 3;
    bool i;
    int j, k;

    for (j = 1; j <= 5; j++)
        i = L.ListInsert(j, j);

    cout << "在L的表尾依次插入1～5后，L=";
    L.ListTraverse(print);
    cout << "L是否空？" << boolalpha << L.ListEmpty() << "，表L的长度=";
    cout << L.ListLength() << endl;
    i = L.PriorElem(e0, equal, e);

    if (i)
        cout << "元素" << e0 << "的前驱为" << e << endl;
    else
        cout << "元素" << e0 << "无前驱，";

    i = L.NextElem(e0, equal, e);

    if (i)
        cout << "元素" << e0 << "的后继为" << e << endl;
    else
        cout << "元素" << e0 << "无后继，";

    k = L.ListLength();

    for (j = k + 1; j >= k; j--) {
        i = L.ListDelete(j, e);

        if (i)
            cout << "删除第" << j << "个元素成功，其值为" << e << endl;
        else
            cout << "删除第" << j << "个元素失败（不存在此元素），";
    }

    cout << "依次输出L的元素：";
    L.ListTraverse(print);
    L.ClearList();
    cout << "清空L后，L=";
    L.ListTraverse(print);
    cout << "L是否空？" << boolalpha << L.ListEmpty() << "，表L的长度=";
    cout << L.ListLength() << endl;
}

