#include <stdio.h>
#include <stdlib.h>
#include "bitree.h"
#include "traverse.h"
static void print_preorder(const BiTreeNode *node)
{
    if (!bitree_is_eob(node)) {
        fprintf(stdout, "Node=%03d\n", *(int *)bitree_data(node));

        if (!bitree_is_eob(bitree_left(node)))
            print_preorder(bitree_left(node));

        if (!bitree_is_eob(bitree_right(node)))
            print_preorder(bitree_right(node));
    }

    return;
}

static void print_inorder(const BiTreeNode *node)
{
    if (!bitree_is_eob(node)) {
        if (!bitree_is_eob(bitree_left(node)))
            print_inorder(bitree_left(node));

        fprintf(stdout, "Node=%03d\n", *(int *)bitree_data(node));

        if (!bitree_is_eob(bitree_right(node)))
            print_inorder(bitree_right(node));
    }

    return;
}

static void print_postorder(const BiTreeNode *node)
{
    if (!bitree_is_eob(node)) {
        if (!bitree_is_eob(bitree_left(node)))
            print_postorder(bitree_left(node));

        if (!bitree_is_eob(bitree_right(node)))
            print_postorder(bitree_right(node));

        fprintf(stdout, "Node=%03d\n", *(int *)bitree_data(node));
    }

    return;
}

static int insert_int(BiTree *tree, int i)
{
    BiTreeNode         *node,
                       *prev;
    int                direction,
                       *data;
    node = tree->root;
    direction = 0;

    while (!bitree_is_eob(node)) {
        prev = node;

        if (i == *(int *)bitree_data(node)) {
            return -1;
        } else if (i < * (int *)bitree_data(node)) {
            node = bitree_left(node);
            direction = 1;
        } else {
            node = bitree_right(node);
            direction = 2;
        }
    }

    if ((data = (int *)malloc(sizeof(int))) == NULL)
        return -1;

    *data = i;

    if (direction == 0)//empty tree
        return bitree_ins_left(tree, NULL, data);

    if (direction == 1)
        return bitree_ins_left(tree, prev, data);

    if (direction == 2)
        return bitree_ins_right(tree, prev, data);

    return -1;
}

static BiTreeNode *search_int(BiTree *tree, int i)
{
    BiTreeNode         *node;
    node = bitree_root(tree);

    while (!bitree_is_eob(node)) {
        if (i == *(int *)bitree_data(node)) {
            return node;
        } else if (i < * (int *)bitree_data(node)) {
            node = bitree_left(node);
        } else {
            node = bitree_right(node);
        }
    }

    return NULL;
}

int main(int argc, char **argv)
{
    BiTree             tree, tree1, tree2;
    BiTreeNode         *node;
    int                i;
    bitree_init(&tree, free);
    fprintf(stdout, "Inserting some nodes\n");

    if (insert_int(&tree, 20) != 0)
        return 1;

    if (insert_int(&tree, 10) != 0)
        return 1;

    if (insert_int(&tree, 30) != 0)
        return 1;

    if (insert_int(&tree, 15) != 0)
        return 1;

    if (insert_int(&tree, 25) != 0)
        return 1;

    if (insert_int(&tree, 70) != 0)
        return 1;

    if (insert_int(&tree, 80) != 0)
        return 1;

    if (insert_int(&tree, 23) != 0)
        return 1;

    if (insert_int(&tree, 26) != 0)
        return 1;

    if (insert_int(&tree, 5) != 0)
        return 1;

    fprintf(stdout, "Tree size is %d\n", bitree_size(&tree));
    fprintf(stdout, "(Preorder traversal)\n");
    print_preorder(bitree_root(&tree));
    i = 30;

    if ((node = search_int(&tree, i)) == NULL) {
        fprintf(stdout, "Could not find %03d\n", i);
    } else {
        fprintf(stdout, "Found %03d...Removing the left tree below it\n", i);
        bitree_rem_left(&tree, node);
        fprintf(stdout, "Tree size is %d\n", bitree_size(&tree));
        fprintf(stdout, "(Preorder traversal)\n");
        print_preorder(bitree_root(&tree));
    }

    i = 99;

    if ((node = search_int(&tree, i)) == NULL) {
        fprintf(stdout, "Could not find %03d\n", i);
    } else {
        fprintf(stdout, "Found %03d...Removing the right tree below it\n", i);
        bitree_rem_right(&tree, node);
        fprintf(stdout, "Tree size is %d\n", bitree_size(&tree));
        fprintf(stdout, "(Preorder traversal)\n");
        print_preorder(bitree_root(&tree));
    }

    i = 20;

    if ((node = search_int(&tree, i)) == NULL) {
        fprintf(stdout, "Could not find %03d\n", i);
    } else {
        fprintf(stdout, "Found %03d...Removing the right tree below it\n", i);
        bitree_rem_right(&tree, node);
        fprintf(stdout, "Tree size is %d\n", bitree_size(&tree));
        fprintf(stdout, "(Preorder traversal)\n");
        print_preorder(bitree_root(&tree));
    }

    i = bitree_is_leaf(bitree_root(&tree));
    fprintf(stdout, "Testing bitree_is_leaf...Value=%d (0=OK)\n", i);
    i = bitree_is_leaf(bitree_left((bitree_root(&tree))));
    fprintf(stdout, "Testing bitree_is_leaf...Value=%d (0=OK)\n", i);
    i = bitree_is_leaf(bitree_left(bitree_left((bitree_root(&tree)))));
    fprintf(stdout, "Testing bitree_is_leaf...Value=%d (1=OK)\n", i);
    i = bitree_is_leaf(bitree_right(bitree_left((bitree_root(&tree)))));
    fprintf(stdout, "Testing bitree_is_leaf...Value=%d (1=OK)\n", i);
    fprintf(stdout, "Inserting some nodes\n");

    if (insert_int(&tree, 55) != 0)
        return 1;

    if (insert_int(&tree, 44) != 0)
        return 1;

    if (insert_int(&tree, 77) != 0)
        return 1;

    if (insert_int(&tree, 11) != 0)
        return 1;

    fprintf(stdout, "Tree size is %d\n", bitree_size(&tree));
    fprintf(stdout, "(Preorder traversal)\n");
    print_preorder(bitree_root(&tree));
    fprintf(stdout, "(Inorder traversal)\n");
    print_inorder(bitree_root(&tree));
    fprintf(stdout, "(Postorder traversal)\n");
    print_postorder(bitree_root(&tree));
    //fprintf(stdout, "Destroying the tree\n");
    //bitree_destroy(&tree);
    printf("==================================\n");
    printf("Merge two tree!\n");
    bitree_init(&tree1, free);

    if (insert_int(&tree1, 6) != 0)
        return 1;

    if (insert_int(&tree1, 9) != 0)
        return 1;

    if (insert_int(&tree1, 4) != 0)
        return 1;

    if (insert_int(&tree1, 5) != 0)
        return 1;

    if (insert_int(&tree1, 2) != 0)
        return 1;

    int *p = (int *)malloc(sizeof(int));
    *p = 35;
    bitree_merge(&tree2, &tree, &tree1, p);
    fprintf(stdout, "(Preorder traversal)\n");
    print_preorder(bitree_root(&tree2));
    printf("==================================\n");
    fprintf(stdout, "Destroying the tree\n");
    bitree_destroy(&tree2);
    return 0;
}
