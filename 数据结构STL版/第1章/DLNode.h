//DLNode.h 双向链表结点类型结构体
#ifndef _DLNODE_H_
#define _DLNODE_H_
template<typename T>struct DLNode {
    T data;
    DLNode<T> *prior, *next;
};
#endif

