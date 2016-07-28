#ifndef __QUEUE__
#define __QUEUE__

#include "CList.h"

template<class T>
class CQueue : CList<T>
{
public:
    CQueue() : CList<T>() { }
    CQueue(T* tElement) : CList<T>(tElement) { }
    virtual ~CQueue() { }
    virtual void Enqueue(T* tElement)
    {
        Append(Head(), tElement);
    }
    virtual T* Dequeue()
    {
        T* tElement = GetFirst();
        Remove(tElement);
        return tElement;
    }
    virtual T* Peek()
    {
        return GetFirst();
    }
    CList<T>::Count;
protected:
    CQueue(const CQueue<T>& cQueue);
    CQueue<T>& operator = (const CQueue<T>& cQueue);
};

#endif
