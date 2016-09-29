//SqQueueCy.h 顺序循环队列类（SqQueueCy类）
#ifndef _SQQUEUECY_H_
#define _SQQUEUECY_H_
#include "AQueue.h"
template<typename T>class SqQueueCy: public AQueue<T>
{
protected:
    T *base;
    int front;
    int rear;
    int queuesize;
public:
    SqQueueCy(int k = 1)
    {
        base = new T[k];
        assert(base != NULL);
        front = rear = 0;
        queuesize = k;
    }
    ~SqQueueCy()
    {
        delete[]base;
    }
    void ClearQueue()
    {
        front = rear = 0;
    }
    bool QueueEmpty()const
    {
        return front == rear;
    }
    int QueueLength()const
    {
        return (rear - front + queuesize) % queuesize;
    }
    bool GetHead(T &e)const
    {
        if (front == rear)
            return false;

        e = *(base + front);
        return true;
    }
    bool EnQueue(T e)
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

        *(base + rear) = e;
        rear = ++rear % queuesize;
        return true;
    }
    bool DeQueue(T &e)
    {
        if (front == rear)
            return false;

        e = *(base + front);
        front = ++front % queuesize;
        return true;
    }
    void QueueTraverse(void(*visit)(T&))const
    {
        int i = front;

        while (i != rear) {
            visit(base[i]);
            i = ++i % queuesize;
        }

        cout << endl;
    }
};
#endif

