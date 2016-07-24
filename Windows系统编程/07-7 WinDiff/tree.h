
/*
 * TREE.H
 *
 * NOTE: include gutils.h before this.
 */

/* handle for a tree */
typedef struct tree FAR * TREE;

/* keys in these trees are DWORDs */
typedef DWORD TREEKEY;

/* A place-holder understood only by tree_search and
 * tree_addafter
 */
typedef struct treeitem FAR * TREEITEM;

/* pointer to one of these place holders */
typedef TREEITEM FAR * PTREEITEM;


TREE APIENTRY tree_create(HANDLE hHeap);
void APIENTRY tree_delete(TREE tree);
LPVOID APIENTRY tree_update(TREE tree, TREEKEY key, LPVOID value, UINT length);
LPVOID APIENTRY tree_find(TREE tree, TREEKEY key);
LPVOID APIENTRY tree_search(TREE tree, TREEKEY key, PTREEITEM place);
LPVOID APIENTRY tree_addafter(TREE tree, PTREEITEM place, TREEKEY key, LPVOID value,
                        UINT length);
TREE APIENTRY ctree_create(HANDLE hHeap);
void APIENTRY ctree_delete(TREE tree);
LPVOID APIENTRY ctree_update(TREE tree, TREEKEY key, LPVOID value, UINT length);
long APIENTRY ctree_getcount(TREE tree, TREEKEY key);
LPVOID APIENTRY ctree_find(TREE tree, TREEKEY key);
