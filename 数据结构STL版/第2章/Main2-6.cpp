//Main2-6.cpp 验证顺序不移动队列SqQueueNM<T>类的成员函数
#include "C.h"
#include "SqQueueNM.h"
typedef int T;
#include "Func1-1.h"
void main()
{
    bool i;
    int k;
    T d;
    SqQueueNM<T> Q(5);

    for (k = 0; k < 6; k++)
        Q.EnQueue(k);

    cout << "入队" << Q.QueueLength() << "个元素后，队列的元素依次为";
    Q.QueueTraverse(print);

    for (k = 0; k < 2; k++) {
        Q.DeQueue(d);
        cout << "出队了队头元素，其值为" << d << endl;
    }

    i = Q.EnQueue(100);

    if (!i)
        cout << "队列已满，无法插入元素。" << endl;

    cout << "当前，队列的元素依次为";
    Q.QueueTraverse(print);
}

