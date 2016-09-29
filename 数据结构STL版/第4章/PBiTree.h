//PBiTree.h 三指针链表结构的二叉树类（PBiTree类）
#ifndef _PBITREE_H_
#define _PBITREE_H_
#define BiTNode PBiTNode
#include "BiTree.h"
template<typename T>class PBiTree: public BiTree<T>
{
    static void VisitParent(BiTNode<T> &c)
    {
        if (c.lchild != NULL)
            c.lchild->parent = &c;

        if (c.rchild != NULL)
            c.rchild->parent = &c;
    }
public:
    void CreateBiTreeFromFile(ifstream &f)
    {
        BiTree<T>::CreateBiTreeFromFile(f);
        OrderTraverse(root, Pre, VisitParent);
        root->parent = NULL;
    }
    bool InsertChild(PBiTNode<T>* &p, bool LR, PBiTree<T> &c)
    {
        BiTree<T>::InsertChild(p, LR, c);

        if (p != NULL) {
            if (!LR) {
                p->lchild->parent = p;

                if (p->lchild->rchild)
                    p->lchild->rchild->parent = p->lchild;
            } else {
                p->rchild->parent = p;

                if (p->rchild->rchild)
                    p->rchild->rchild->parent = p->rchild;
            }

            return true;
        }

        return false;
    }
    BiTNode<T>* Parent(BiTNode<T>* p)const
    {
        return p->parent;
    }
    bool Sibling(BiTNode<T>* p, BiTNode<T>* &sib, bool &LR)const
    {
        if (p->parent == NULL)
            return false;

        if (p->parent->lchild == p) {
            sib = p->parent->rchild;
            LR = true;
        } else {
            sib = p->parent->lchild;
            LR = false;
        }

        return sib != NULL;
    }
};
#endif

