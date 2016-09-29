//BTNode.h B树的3种结构体
#ifndef _BTNODE_H_
#define _BTNODE_H_
struct KeyIndex {
    KeyType key;
    int index;
};
struct BTNode {
    BTNode *parent;
    vector<BTNode*>children;
    vector<KeyIndex> ki;
};
struct Result {
    BTNode *pt;
    bool tag;
    int i;
};
#endif

