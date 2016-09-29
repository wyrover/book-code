//LNode.h 单链表结点类型结构体
#ifndef _LNODE_H_
#define _LNODE_H_
template<typename T>struct LNode {
    T data;
    LNode<T> *next;
};
#endif

