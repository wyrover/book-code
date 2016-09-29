//ChangeTree.h 二叉排序树转换操作的类（ChangeTree类）
#ifndef _CHANGETREE_H_
#define _CHANGETREE_H_
#include "BSTree.h"
template<typename T>class ChangeTree: public BSTree<T>
{
protected:
    void LL_Rotate(BiTNode<T>* &p)
    {
        BiTNode<T> *lc = p->lchild;
        p->lchild = lc->rchild;
        lc->rchild = p;
        p = lc;
    }
    void RR_Rotate(BiTNode<T>* &p)
    {
        BiTNode<T> *rc = p->rchild;
        p->rchild = rc->lchild;
        rc->lchild = p;
        p = rc;
    }
    void LR_Rotate(BiTNode<T>* &p)
    {
        BiTNode<T> *lc = p->lchild;
        p->lchild = lc->rchild->rchild;
        lc->rchild->rchild = p;
        p = lc->rchild;
        lc->rchild = p->lchild;
        p->lchild = lc;
    }
    void RL_Rotate(BiTNode<T>* &p)
    {
        BiTNode<T> *rc = p->rchild;
        p->rchild = rc->lchild->lchild;
        rc->lchild->lchild = p;
        p = rc->lchild;
        rc->lchild = p->rchild;
        p->rchild = rc;
    }
    BiTNode<T>* Parent(BiTNode<T>* p)const
    {
        if (root == p)
            return NULL;

        BiTNode<T> *q = root;

        while (q != NULL)
            if (q->data.key > p->data.key)
                if (q->lchild == p)
                    return q;
                else
                    q = q->lchild;
            else if (q->rchild == p)
                return q;
            else
                q = q->rchild;

        return NULL;
    }
};
#endif

