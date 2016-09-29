//RBTNode.h 红黑树的结点类型结构体
#ifndef _RBTNODE_H_
#define _RBTNODE_H_
enum Color {Red, Black};
template<typename T>struct RBTNode {
    T data;
    Color RB;
    RBTNode<T> *lchild, *rchild;
};
#endif

