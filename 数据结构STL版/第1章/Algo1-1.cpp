//Algo1-1.cpp 归并两个有序的顺序表得到新的有序顺序表的算法
#include "C.h"
#include "SqList.h"
typedef int T;
#include "Func1-1.h"
void MergeList(const SqList<T> &La, const SqList<T> &Lb, SqList<T> &Lc)
{
    T *pa, *pa_last, *pb, *pb_last, *pc;
    assert(Lc.listsize >= La.length + Lb.length);
    pa = La.elem;
    pb = Lb.elem;
    pc = Lc.elem;
    pa_last = La.elem + La.length - 1;
    pb_last = Lb.elem + Lb.length - 1;

    while (pa <= pa_last && pb <= pb_last) {
        if (*pa <= *pb)
            *pc++ = *pa++;
        else
            *pc++ = *pb++;
    }

    while (pa <= pa_last)
        *pc++ = *pa++;

    while (pb <= pb_last)
        *pc++ = *pb++;

    Lc.length = La.length + Lb.length;
}
void main()
{
    SqList<T> La(5), Lb(3), Lc(8);

    for (int j = 0; j < 5; j++)
        La.ListInsert(j, j + 1);

    cout << "La=";
    La.ListTraverse(print);

    for (j = 0; j < 3; j++)
        Lb.ListInsert(j, 2 * (j + 1));

    cout << "Lb=";
    Lb.ListTraverse(print);
    MergeList(La, Lb, Lc);
    cout << "Lc=";
    Lc.ListTraverse(print);
}

