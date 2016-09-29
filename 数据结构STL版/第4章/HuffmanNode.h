//HuffmanNode.h 赫夫曼树结点类型结构体
#ifndef _HUFFMANNODE_H_
#define _HUFFMANNODE_H_
template<typename T>struct HTNode {
    T weight;
    int parent, lchild, rchild;
};
#endif

