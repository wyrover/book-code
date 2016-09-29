//SqQueueNM.h 顺序不移动队列类（出队时不移动元素，只改变头指针的值）
#ifndef _SQQUEUENM_H_
#define _SQQUEUENM_H_
#include "SqQueueCy.h"
template<typename T>class SqQueueNM: public SqQueueCy<T>
{
public:
    SqQueueNM(int k): SqQueueCy<T>(k)
    {
    }
    bool EnQueue(T e)
    {
        if (rear == queuesize)
            return false;

        *(base + rear++) = e;
        return true;
    }
    bool DeQueue(T &e)
    {
        if (front == rear)
            return false;

        e = *(base + front++);
        return true;
    }
    int QueueLength()const
    {
        return rear - front;
    }
    void QueueTraverse(void(*visit)(T&))const
    {
        for (int i = front; i < rear; i++)
            visit(base[i]);

        cout << endl;
    }
};
#endif

