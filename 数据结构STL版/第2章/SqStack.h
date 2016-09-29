//SqStack.h À≥–Ú’ªµƒ¿‡£®SqStack¿‡£©
#ifndef _SQSTACK_H_
#define _SQSTACK_H_
#include "AStack.h"
template<typename T>class SqStack: public AStack<T>
{
private:
    T *base, *top;
    int stacksize;
public:
    SqStack(int k = 1)
    {
        base = new T[k];
        assert(base != NULL);
        top = base;
        stacksize = k;
    }
    ~SqStack()
    {
        delete[]base;
    }
    void ClearStack()
    {
        top = base;
    }
    bool StackEmpty()const
    {
        return top == base;
    }
    int StackLength()const
    {
        return top - base;
    }
    bool GetTop(T &e)const
    {
        if (top > base) {
            e = *(top - 1);
            return true;
        } else
            return false;
    }
    bool Push(T e)
    {
        T *newbase;

        if (top - base == stacksize) {
            newbase = new T[stacksize * 2];

            if (newbase == NULL)
                return false;

            for (int j = 0; j < top - base; j++)
                *(newbase + j) = *(base + j);

            delete[]base;
            base = newbase;
            top = base + stacksize;
            stacksize *= 2;
        }

        *top++ = e;
        return true;
    }
    bool Pop(T &e)
    {
        if (top == base)
            return false;

        e = *--top;
        return true;
    }
    void StackTraverse(void(*visit)(T&))const
    {
        T *p = base;

        while (p < top)
            visit(*(p++));

        cout << endl;
    }
};
#endif

