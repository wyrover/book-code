//Main2-7.cpp 验证双端顺序循环队列DeQueueCy<T>类的成员函数
#include "C.h"
#include "DeQueueCy.h"
typedef int T;
#include "Func1-1.h"
void main()
{
    bool i;
    T d;
    DeQueueCy<T> Q;

    for (int k = 0; k < 5; k++) {
        assert(Q.EnQueue(k));
        assert(Q.EnQueueAtFront(k * 2));
    }

    cout << "入队10个元素后，队列的元素依次为";
    Q.QueueTraverse(print);
    Q.DeQueue(d);
    cout << "出队了队头元素，其值为" << d;
    Q.DeQueueAtRear(d);
    cout << "，出队了队尾元素，其值为" << d << endl;

    if (i = Q.GetHead(d))
        cout << "新的队头元素是" << d;

    if (i = Q.GetRear(d))
        cout << "，新的队尾元素是" << d << endl;

    cout << "现在队列的元素依次为";
    Q.QueueTraverse(print);
}

