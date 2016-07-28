#include<stdlib.h>
#include<string.h>
#include"bitree.h"

void bitree_init(BiTree *tree, void (*destroy)(void *data))
{
    tree->size = 0;
    tree->destroy = destroy;
    tree->root = NULL;
    return ;
}

void bitree_destroy(BiTree *tree)
{
    bitree_rem_left(tree, NULL);
    memset(tree, 0, sizeof(BiTree));
    return ;
}

int bitree_ins_left(BiTree *tree, BiTreeNode *node, const void *data)
{
    BiTreeNode *new_node, **position;

    if (node == NULL) {
        if (bitree_size(tree) > 0)
            return -1;

        position = &tree->root;
    } else {
        if (bitree_left(node) != NULL)
            return -1;

        position = &node->left;
    }

    if ((new_node = (BiTreeNode *)malloc(sizeof(BiTreeNode))) == NULL)
        return -1;

    new_node->data = (void *)data;
    new_node->left = NULL;
    new_node->right = NULL;
    *position = new_node;
    tree->size++;
    return 0;
}

int bitree_ins_right(BiTree *tree, BiTreeNode *node, const void *data)
{
    BiTreeNode *new_node, **position;

    if (node == NULL) {
        if (bitree_size(tree) > 0)
            return -1;

        position = &tree->root;
    } else {
        if (bitree_right(node) != NULL)
            return -1;

        position = &node->right;
    }

    if ((new_node = (BiTreeNode *)malloc(sizeof(BiTreeNode))) == NULL)
        return -1;

    new_node->data = (void *)data;
    new_node->left = NULL;
    new_node->right = NULL;
    *position = new_node;
    tree->size++;
    return 0;
}

void bitree_rem_left(BiTree *tree, BiTreeNode *node)
{
    BiTreeNode **position;

    if (bitree_size(tree) == 0)
        return;

    if (node == NULL)
        position = &tree->root;
    else
        position = &node->left;

    if (*position != NULL) {
        bitree_rem_left(tree, *position);
        bitree_rem_right(tree, *position);

        if (tree->destroy != NULL) {
            tree->destroy((*position)->data);
        }

        free(*position);
        *position = NULL;
        tree->size--;
    }

    return ;
}

void bitree_rem_right(BiTree *tree, BiTreeNode *node)
{
    BiTreeNode **position;

    if (bitree_size(tree) == 0)
        return ;

    if (node == NULL)
        position = &tree->root;
    else
        position = &node->right;

    if (*position != NULL) {
        bitree_rem_left(tree, *position);
        bitree_rem_right(tree, *position);

        if (tree->destroy != NULL) {
            tree->destroy((*position)->data);
        }

        free(*position);
        *position = NULL;
        tree->size--;
    }

    return ;
}

int bitree_merge(BiTree *merge, BiTree *left, BiTree *right, const void *data)
{
    bitree_init(merge, left->destroy);

    if (bitree_ins_left(merge, NULL, data) != 0) {
        bitree_destroy(merge);
        return -1;
    }

    bitree_root(merge)->left = bitree_root(left);
    bitree_root(merge)->right = bitree_root(right);
    merge->size = merge->size + bitree_size(left) + bitree_size(right);
    left->root = NULL;
    left->size = 0;
    right->root = NULL;
    right->size = 0;
    return 0;
}
