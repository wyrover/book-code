#ifndef BITREE_H
#define BITREE_H
#include<stdlib.h>

typedef struct BiTreeNode_ {
    void *data;
    struct BiTreeNode_ *left;
    struct BiTreeNode_ *right;
} BiTreeNode;

typedef struct BiTree_ {
    int size;
    int (*compare)(const void *key1, const void *key2);
    void (*destroy)(void *data);
    BiTreeNode *root;
} BiTree;

void bitree_init(BiTree *tree, void (*destroy)(void *data));
void bitree_destroy(BiTree *tree);
int bitree_ins_left(BiTree *tree, BiTreeNode *node, const void *data);
int bitree_ins_right(BiTree *tree, BiTreeNode *node, const void *data);
void bitree_rem_left(BiTree *tree, BiTreeNode *node);
void bitree_rem_right(BiTree *tree, BiTreeNode *node);
int bitree_merge(BiTree *merge, BiTree *left, BiTree *right, const void *data);

#define bitree_size(tree) ((tree)->size)
#define bitree_root(tree) ((tree)->root)
#define bitree_is_eob(node) ((node)==NULL)
#define bitree_is_leaf(node) ((node)->left==NULL&&(node)->right==NULL)
#define bitree_data(node) ((node)->data)
#define bitree_left(node) ((node)->left)
#define bitree_right(node) ((node)->right)

#endif
