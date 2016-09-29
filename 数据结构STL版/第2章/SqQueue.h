//SqQueue.h 顺序队列类（队头元素固定在[0]单元）
#ifndef _SQQUEUE_H_
#define _SQQUEUE_H_
template<typename T>class SqQueue
{
private:
    T *base;
    int rear;
    int queuesize;
public:
    SqQueue(int k = 1)
    {
        base = new T[k];
        assert(base != NULL);
        rear = 0;
        queuesize = k;
    }
    ~SqQueue()
    {
        delete[]base;
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
        if (rear == 0)
            return false;

        e = *base;

        for (int i = 1; i < rear; i++)
            *(base + i - 1) = *(base + i);

        rear--;
        return true;
    }
    void QueueTraverse(void(*visit)(T&))const
    {
        for (int i = 0; i < rear; i++)
            visit(base[i]);

        cout << endl;
    }
};
#endif

