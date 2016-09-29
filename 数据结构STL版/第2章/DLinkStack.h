//DLinkStack.h 用双向链表实现链栈的类（DLinkStack类）
#ifndef _DLINKSTACK_H_
#define _DLINKSTACK_H_
#include "AStack.h"
#include "DLinkList.h"
template<typename T>class DLinkStack: public AStack<T>, public DLinkList<T>
{
public:
    void ClearStack()const
    {
        ClearList();
    }
    bool StackEmpty()const
    {
        return ListEmpty();
    }
    int StackLength()const
    {
        return ListLength();
    }
    bool GetTop(T &e)const
    {
        return GetElem(1, e);
    }
    bool Push(T e)
    {
        return ListInsert(1, e);
    }
    bool Pop(T &e)const
    {
        return ListDelete(1, e);
    }
    void StackTraverse(void(*visit)(T&))const
    {
        ListBackTraverse(visit);
    }
};
#endif

