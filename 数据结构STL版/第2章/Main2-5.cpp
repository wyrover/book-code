//Main2-5.cpp 验证顺序队列SqQueue<T>类的成员函数
#include "C.h"
#include "SqQueue.h"
typedef int T;
#include "Func1-1.h"
void main()
{
    int k;
    T d;
    SqQueue<T> Q(5);

    for (k = 0; k < 5; k++)
        Q.EnQueue(k);

    cout << "入队5个元素后，队列的元素依次为";
    Q.QueueTraverse(print);

    for (k = 0; k < 2; k++) {
        Q.DeQueue(d);
        cout << "出队了队头元素，其值为" << d << endl;
    }

    cout << "当前，队列的元素依次为";
    Q.QueueTraverse(print);
}

