//BSDTree.h 二叉排序树的删除类（BSDTree类）
#ifndef _BSDTREE_H_
#define _BSDTREE_H_
#include "BiTree.h"
template<typename T>class BSDTree: public BiTree<T>
{
protected:
    virtual void Delete(BiTNode<T>* &p) = 0;
    bool DeleteBST(BiTNode<T>* &p, KeyType key)
    {
        if (p == NULL)
            return false;
        else {
            if EQ(key, p->data.key) {
                Delete(p);
                return true;
            } else if LT(key, p->data.key)
                return DeleteBST(p->lchild, key);
            else
                return DeleteBST(p->rchild, key);
        }
    }
public:
    bool Delete(KeyType key)
    {
        return DeleteBST(root, key);
    }
};
#endif

