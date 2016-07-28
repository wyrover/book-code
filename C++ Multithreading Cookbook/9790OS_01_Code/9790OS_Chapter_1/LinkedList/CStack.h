#ifndef __STACK__
#define __STACK__

#include "CList.h"

template<class T>
class CStack : CList<T>
{
public:
    CStack() : CList<T>() { }
    CStack(T* tElement) : CList<T>(tElement) { }
    virtual ~CStack() { }
    virtual void Push(T* tElement)
    {
        Insert(tElement);
    }
    virtual T* Pop()
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
    CStack(const CStack<T>& cStack);
    CStack<T>& operator = (const CStack<T>& cStack);
};

#endif
