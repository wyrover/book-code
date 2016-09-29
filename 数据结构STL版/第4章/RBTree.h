//RBTree.h ºìºÚÊ÷µÄÀà£¨RBTreeÀà£©
#ifndef _RBTREE_H_
#define _RBTREE_H_
#include "ChangeTree.h"
template<typename T>class RBTree: public ChangeTree<T>
{
private:
    RBTNode<T>* Uncle(RBTNode<T>* p)const
    {
        RBTNode<T> *g, *l, *r, *a = Parent(p);

        if (a != NULL) {
            g = Parent(a);
            Child(g, l, r);

            if (l == a)
                return r;
            else
                return l;
        }

        return NULL;
    }
    BiTNode<T>* Grandfather(BiTNode<T>* p)const
    {
        BiTNode<T> *a = Parent(p);

        if (a != NULL)
            a = Parent(a);

        return a;
    }
    void AdjustDoubleRed(RBTNode<T>* &s, RBTNode<T>* &p)
    {
        RBTNode<T> *u, *g, *gp;
        int flag;
        u = Uncle(s);
        g = Grandfather(s);

        if (g == root)
            flag = 0;
        else {
            gp = Parent(g);

            if (g->data.key < gp->data.key)
                flag = 1;
            else
                flag = 2;
        }

        if (u == NULL || u->RB == Black) {
            if (g->data.key > p->data.key)
                if (p->data.key > s->data.key)
                    LL_Rotate(g);
                else
                    LR_Rotate(g);
            else if (p->data.key < s->data.key)
                RR_Rotate(g);
            else
                RL_Rotate(g);

            g->RB = Black;
            g->lchild->RB = g->rchild->RB = Red;

            switch (flag) {
            case 0:
                root = g;
                break;

            case 1:
                gp->lchild = g;
                break;

            case 2:
                gp->rchild = g;
            }
        } else {
            p->RB = u->RB = Black;

            if (flag > 0) {
                g->RB = Red;
                u = Parent(g);

                if (u->RB == Red)
                    AdjustDoubleRed(g, u);
            }
        }
    }
    void AdjustDoubleBlack(RBTNode<T>* &pa, bool lr)
    {
        RBTNode<T> *gp;
        int flag;

        if (pa == root)
            flag = 0;
        else {
            gp = Parent(pa);

            if (pa->data.key < gp->data.key)
                flag = 1;
            else
                flag = 2;
        }

        if (lr) {
            if (pa->lchild != NULL && pa->lchild->RB == Red)
                pa->lchild->RB = Black;
            else {
                if (pa->rchild != NULL && pa->rchild->RB == Black) {
                    if (pa->rchild->rchild != NULL && pa->rchild->rchild->RB == Red) {
                        pa->rchild->RB = pa->RB;
                        pa->RB = pa->rchild->rchild->RB = Black;
                        RR_Rotate(pa);

                        switch (flag) {
                        case 0:
                            root = pa;
                            break;

                        case 1:
                            gp->lchild = pa;
                            break;

                        case 2:
                            gp->rchild = pa;
                        }
                    } else if (pa->rchild->lchild != NULL && pa->rchild->lchild->RB == Red) {
                        pa->rchild->lchild->RB = pa->RB;
                        pa->RB = pa->rchild->RB = Black;
                        RL_Rotate(pa);

                        switch (flag) {
                        case 0:
                            root = pa;
                            break;

                        case 1:
                            gp->lchild = pa;
                            break;

                        case 2:
                            gp->rchild = pa;
                        }
                    } else {
                        pa->rchild->RB = Red;

                        if (pa->RB == Red)
                            pa->RB = Black;
                        else {
                            switch (flag) {
                            case 1:
                                AdjustDoubleBlack(gp, true);
                                break;

                            case 2:
                                AdjustDoubleBlack(gp, false);
                            }
                        }
                    }
                } else if (pa->rchild != NULL && pa->rchild->RB == Red) {
                    pa->RB = Red;
                    pa->rchild->RB = Black;
                    RR_Rotate(pa);

                    switch (flag) {
                    case 0:
                        root = pa;
                        break;

                    case 1:
                        gp->lchild = pa;
                        break;

                    case 2:
                        gp->rchild = pa;
                    }

                    AdjustDoubleBlack(pa->lchild, true);
                }
            }
        } else {
            if (pa->rchild != NULL && pa->rchild->RB == Red)
                pa->rchild->RB = Black;
            else {
                if (pa->lchild != NULL && pa->lchild->RB == Black) {
                    if (pa->lchild->lchild != NULL && pa->lchild->lchild->RB == Red) {
                        pa->lchild->RB = pa->RB;
                        pa->RB = pa->lchild->lchild->RB = Black;
                        LL_Rotate(pa);

                        switch (flag) {
                        case 0:
                            root = pa;
                            break;

                        case 1:
                            gp->lchild = pa;
                            break;

                        case 2:
                            gp->rchild = pa;
                        }
                    } else if (pa->lchild->rchild != NULL && pa->lchild->rchild->RB == Red) {
                        pa->lchild->rchild->RB = pa->RB;
                        pa->RB = pa->lchild->RB = Black;
                        LR_Rotate(pa);

                        switch (flag) {
                        case 0:
                            root = pa;
                            break;

                        case 1:
                            gp->lchild = pa;
                            break;

                        case 2:
                            gp->rchild = pa;
                        }
                    } else {
                        pa->lchild->RB = Red;

                        if (pa->RB == Red)
                            pa->RB = Black;
                        else
                            switch (flag) {
                            case 1:
                                AdjustDoubleBlack(gp, true);
                                break;

                            case 2:
                                AdjustDoubleBlack(gp, false);
                            }
                    }
                } else if (pa->lchild != NULL && pa->lchild->RB == Red) {
                    pa->RB = Red;
                    pa->lchild->RB = Black;
                    LL_Rotate(pa);

                    switch (flag) {
                    case 0:
                        root = pa;
                        break;

                    case 1:
                        gp->lchild = pa;
                        break;

                    case 2:
                        gp->rchild = pa;
                    }

                    AdjustDoubleBlack(pa->rchild, false);
                }
            }
        }
    }
    void Delete(RBTNode<T>* &p)
    {
        RBTNode<T> *pa, *s, *q = p;
        bool lr, deleflag = false;
        bool rootflag = (p == root);

        if (p->rchild == NULL || p->lchild == NULL) {
            if (p->RB == Black && p != root) {
                deleflag = true;
                pa = Parent(p);

                if (p->data.key < pa->data.key)
                    lr = true;
                else
                    lr = false;
            }

            if (p->rchild == NULL)
                p = p->lchild;
            else
                p = p->rchild;

            if (rootflag && p != NULL)
                p->RB = Black;

            delete q;
        } else {
            s = p->lchild;

            while (s->rchild != NULL) {
                q = s;
                s = s->rchild;
            }

            if (s->RB == Black) {
                deleflag = true;
                pa = Parent(s);

                if (s->data.key < pa->data.key)
                    lr = true;
                else
                    lr = false;
            }

            p->data = s->data;

            if (q != p)
                q->rchild = s->lchild;
            else
                q->lchild = s->lchild;

            delete s;
        }

        if (deleflag)
            AdjustDoubleBlack(pa, lr);
    }
public:
    bool Insert(T e)
    {
        RBTNode<T> *p, *s;

        if (!SearchBST(root, e.key, NULL, p)) {
            s = new RBTNode<T>;
            s->data = e;
            s->lchild = s->rchild = NULL;

            if (p == NULL) {
                root = s;
                s->RB = Black;
            } else {
                if LT(e.key, p->data.key)
                    p->lchild = s;
                else
                    p->rchild = s;

                s->RB = Red;

                if (p->RB == Red)
                    AdjustDoubleRed(s, p);
            }

            return true;
        } else
            return false;
    }
};
#endif

