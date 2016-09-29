//Algo1-4.cpp 归并两个有序链表的算法
#include "C.h"
#include "LinkList.h"
typedef int T;
#include "Func1-1.h"
void MergeList(LinkList<T> &La, LinkList<T> &Lb)
{
    LNode<T> *pa = La.Head->next, *pb = Lb.Head->next, *p;
    p = La.Head;

    while (pa && pb)
        if (pa->data <= pb->data) {
            p->next = pa;
            p = pa;
            pa = pa->next;
        } else {
            p->next = pb;
            p = pb;
            pb = pb->next;
        }

    p->next = pa ? pa : pb;
    Lb.Head->next = NULL;
}
void main()
{
    LinkList<T> La, Lb;

    for (int i = 1; i <= 5; i++) {
        La.ListInsert(i, i);
        Lb.ListInsert(i, i * 2);
    }

    cout << "La=";
    La.ListTraverse(print);
    cout << "Lb=";
    Lb.ListTraverse(print);
    MergeList(La, Lb);
    cout << "New La=";
    La.ListTraverse(print);
    cout << "New Lb=";
    Lb.ListTraverse(print);
}

