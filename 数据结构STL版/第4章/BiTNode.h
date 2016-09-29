//BiTNode.h 二叉链表结点类型结构体
#ifndef _BITNODE_H_
#define _BITNODE_H_
template<typename T>struct BiTNode {
    T data;
    BiTNode<T> *lchild, *rchild;
};
#endif

