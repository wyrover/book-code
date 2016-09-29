//DLTNode.h 双链键树结点的存储结构
#ifndef _DLTNODE_H_
#define _DLTNODE_H_
enum NodeKind {LEAF, BRANCH};
struct DLTNode {
    char symbol;
    NodeKind kind;
    union {
        int index;
        DLTNode *first;
    };
    DLTNode *next;
    DLTNode()
    {
        kind = BRANCH;
        first = next = NULL;
    }
};
#endif

