//LinkQueue.h 链队列的类（LinkQueue类）
#ifndef _LINKQUEUE_H_
#define _LINKQUEUE_H_
#include "AQueue.h"
#include "LinkList.h"
template<typename T>class LinkQueue:
    public AQueue<T>, public LinkList<T>
{
private:
    LNode<T> *rear;
public:
    LinkQueue()
    {
        rear = Head;
    }
    void ClearQueue()
    {
        ClearList();
        rear = Head;
    }
    bool QueueEmpty()const
    {
        return ListEmpty();
    }
    int QueueLength()const
    {
        return ListLength();
    }
    bool GetHead(T &e)const
    {
        return GetElem(1, e);
    }
    bool EnQueue(T e)
    {
        LNode<T> *p;
        p = new LNode<T>;

        if (p == NULL)
            return false;

        p->data = e;
        p->next = NULL;
        rear->next = p;
        rear = p;
        return true;
    }
    bool DeQueue(T &e)
    {
        bool flag = ListDelete(1, e);

        if (Head->next == NULL)
            rear = Head;

        return flag;
    }
    void QueueTraverse(void(*visit)(T&))const
    {
        ListTraverse(visit);
    }
};
#endif

