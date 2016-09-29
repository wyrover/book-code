//CSNode.h 孩子-兄弟二叉链表结点类型结构体
#ifndef _CSNODE_H_
#define _CSNODE_H_
template<typename T>struct CSNode {
    T data;
    CSNode<T> *firstchild, *nextsibling;

};
#endif

