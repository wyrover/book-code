//AStack.h 栈抽象类的定义
#ifndef _ASTACK_H_
#define _ASTACK_H_
template<typename T>class AStack
{
public:
    void ClearStack();
    bool StackEmpty()const;
    int StackLength()const;
    bool GetTop(T &e)const;
    bool Push(T e);
    bool Pop(T &e);
    void StackTraverse(void(*visit)(T&))const;

};
#endif

