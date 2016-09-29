//DeQueueCy.h 双端顺序循环队列类（DeQueueCy类）
#ifndef _DEQUEUECY_H_
#define _DEQUEUECY_H_
#include "SqQueueCy.h"
template<typename T>class DeQueueCy: public SqQueueCy<T>
{
public:
    bool GetRear(T &e)const
    {
        if (front == rear)
            return false;

        e = *(base + (rear - 1 + queuesize) % queuesize);
        return true;
    }
    bool EnQueueAtFront(T e)
    {
        T *newbase;
        int i;

        if ((rear + 1) % queuesize == front) {
            newbase = new T[queuesize * 2];

            if (newbase == NULL)
                return false;

            for (i = 0; i < queuesize - 1; i++)
                *(newbase + i) = *(base + (front + i) % queuesize);

            delete[]base;
            base = newbase;
            front = 0;
            rear = queuesize - 1;
            queuesize *= 2;
        }

        front = (front - 1 + queuesize) % queuesize;
        *(base + front) = e;
        return true;
    }
    bool DeQueueAtRear(T &e)
    {
        if (front == rear)
            return false;

        rear = (rear - 1 + queuesize) % queuesize;
        e = *(base + rear);
        return true;
    }
};
#endif

