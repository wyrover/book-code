//AVLTNode.h 平衡二叉树的结点类型结构体
#ifndef _AVLTNODE_H_
#define _AVLTNODE_H_
template<typename T>struct AVLTNode {
    T data;
    int bf;
    AVLTNode<T> *lchild, *rchild;
};
#endif

