//AQueue.h 队列抽象类的定义
#ifndef _AQUEUE_H_
#define _AQUEUE_H_
template<typename T>class AQueue
{
public:
    void ClearQueue();
    bool QueueEmpty()const;
    int QueueLength()const;
    bool GetHead(T &e)const;
    bool EnQueue(T e);
    bool DeQueue(T &e);
    void QueueTraverse(void(*visit)(T&))const;

};
#endif

