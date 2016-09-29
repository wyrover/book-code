//AVLTree.h 平衡二叉树的类（AVLTree类）
#ifndef _AVLTREE_H_
#define _AVLTREE_H_
#include "ChangeTree.h"
template<typename T>class AVLTree: public ChangeTree<T>
{
private:
    void LeftBalance(AVLTNode<T>* &p)
    {
        AVLTNode<T> *lc, *rd;
        lc = p->lchild;

        switch (lc->bf) {
        case LH:
            p->bf = lc->bf = EH;
            LL_Rotate(p);
            break;

        case RH:
            rd = lc->rchild;

            switch (rd->bf) {
            case LH:
                p->bf = RH;
                lc->bf = EH;
                break;

            case EH:
                p->bf = lc->bf = EH;
                break;

            case RH:
                p->bf = EH;
                lc->bf = LH;
            }

            rd->bf = EH;
            LR_Rotate(p);
        }
    }
    void RightBalance(AVLTNode<T>* &p)
    {
        AVLTNode<T> *rc, *ld;
        rc = p->rchild;

        switch (rc->bf) {
        case RH:
            p->bf = rc->bf = EH;
            RR_Rotate(p);
            break;

        case LH:
            ld = rc->lchild;

            switch (ld->bf) {
            case RH:
                p->bf = LH;
                rc->bf = EH;
                break;

            case EH:
                p->bf = rc->bf = EH;
                break;

            case LH:
                p->bf = EH;
                rc->bf = RH;
            }

            ld->bf = EH;
            RL_Rotate(p);
        }
    }
    bool InsertAVL(AVLTNode<T>* &p, T e, bool &taller)
    {
        if (p == NULL) {
            p = new AVLTNode<T>;
            p->data = e;
            p->lchild = p->rchild = NULL;
            p->bf = EH;
            taller = true;
        } else {
            if EQ(e.key, p->data.key)
                return false;
            else if LT(e.key, p->data.key) {
                if (!InsertAVL(p->lchild, e, taller))
                    return false;

                if (taller)
                    switch (p->bf) {
                    case LH:
                        LeftBalance(p);
                        taller = false;
                        break;

                    case EH:
                        p->bf = LH;
                        taller = true;
                        break;

                    case RH:
                        p->bf = EH;
                        taller = false;
                    }
            } else {
                if (!InsertAVL(p->rchild, e, taller))
                    return false;

                if (taller)
                    switch (p->bf) {
                    case LH:
                        p->bf = EH;
                        taller = false;
                        break;

                    case EH:
                        p->bf = RH;
                        taller = true;
                        break;

                    case RH:
                        RightBalance(p);
                        taller = false;
                    }
            }
        }

        return true;
    }
    bool DeleteAVL(AVLTNode<T>* &p, T &e, bool &lower)
    {
        AVLTNode<T> *rc, *lc;
        T e1;

        if (p == NULL)
            return false;
        else {
            if (EQ(e.key, p->data.key)) {
                e = p->data;
                rc = p;

                if (p->lchild != NULL && p->rchild != NULL) {
                    if (p->bf == RH) {
                        lc = p->rchild;

                        while (lc->lchild != NULL)
                            lc = lc->lchild;
                    } else {
                        lc = p->lchild;

                        while (lc->rchild != NULL)
                            lc = lc->rchild;
                    }

                    e1 = lc->data;
                    DeleteAVL(p, e1, lower);
                    rc->data = e1;
                } else {
                    if (p->rchild == NULL)
                        p = p->lchild;
                    else
                        p = p->rchild;

                    delete rc;
                    lower = true;
                }
            } else if (LT(e.key, p->data.key)) {
                if (!DeleteAVL(p->lchild, e, lower))
                    return false;

                if (lower) {
                    switch (p->bf) {
                    case EH:
                        p->bf = RH;
                        lower = false;
                        break;

                    case LH:
                        p->bf = EH;
                        lower = true;
                        break;

                    case RH:
                        D_LeftBalance(p, lower);
                    }
                }
            } else {
                if (!DeleteAVL(p->rchild, e, lower))
                    return false;

                if (lower) {
                    switch (p->bf) {
                    case EH:
                        p->bf = LH;
                        lower = false;
                        break;

                    case RH:
                        p->bf = EH;
                        lower = true;
                        break;

                    case LH:
                        D_RightBalance(p, lower);
                    }
                }
            }

            return true;
        }
    }
    void D_LeftBalance(AVLTNode<T>* &p, bool &lower)
    {
        AVLTNode<T> *ld, *rc = p->rchild;

        switch (rc->bf) {
        case EH:
            rc->bf = LH;
            p->bf = RH;
            RR_Rotate(p);
            lower = false;
            break;

        case RH:
            p->bf = rc->bf = EH;
            RR_Rotate(p);
            lower = true;
            break;

        case LH:
            ld = rc->lchild;

            switch (ld->bf) {
            case EH:
                p->bf = rc->bf = EH;
                break;

            case LH:
                p->bf = EH;
                rc->bf = RH;
                break;

            case RH:
                p->bf = LH;
                rc->bf = EH;
            }

            ld->bf = EH;
            RL_Rotate(p);
            lower = true;
        }
    }
    void D_RightBalance(AVLTNode<T>* &p, bool &lower)
    {
        AVLTNode<T> *rd, *lc = p->lchild;

        switch (lc->bf) {
        case EH:
            lc->bf = RH;
            p->bf = LH;
            LL_Rotate(p);
            lower = false;
            break;

        case LH:
            p->bf = lc->bf = EH;
            LL_Rotate(p);
            lower = true;
            break;

        case RH:
            rd = lc->rchild;

            switch (rd->bf) {
            case EH:
                p->bf = lc->bf = EH;
                break;

            case RH:
                p->bf = EH;
                lc->bf = LH;
                break;

            case LH:
                p->bf = RH;
                lc->bf = EH;
            }

            rd->bf = EH;
            LR_Rotate(p);
            lower = true;
        }
    }
public:
    bool Insert(T e)
    {
        bool taller;
        return InsertAVL(root, e, taller);
    }
    bool Delete(T &e)
    {
        bool lower;
        return DeleteAVL(root, e, lower);
    }
    bool DeleteChild(AVLTNode<T>* &p, int LR)
    {
        return false;
    }
};
#endif

