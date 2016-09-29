//SpTree.h 伸展树的类（SpTree类）
#ifndef _SPTREE_H_
#define _SPTREE_H_
#include "ChangeTree.h"
template<typename T>class SpTree: public ChangeTree<T>
{
private:
    void Zig(BiTNode<T>* p)
    {
        if (p->data.key < root->data.key) {
            root->lchild = p->rchild;
            p->rchild = root;
        } else {
            root->rchild = p->lchild;
            p->lchild = root;
        }

        root = p;
    }
    void ZigZig(BiTNode<T>* p, BiTNode<T>* pp, BiTNode<T>* pg)
    {
        BiTNode<T> *r = Parent(pg);

        if (pp->data.key < pg->data.key) {
            if (p->data.key < pp->data.key) {
                pp->lchild = p->rchild;
                pg->lchild = pp->rchild;
                pp->rchild = pg;
                p->rchild = pp;
            } else {
                pp->rchild = p->lchild;
                pg->lchild = p->rchild;
                p->lchild = pp;
                p->rchild = pg;
            }
        } else {
            if (p->data.key > pp->data.key) {
                pp->rchild = p->lchild;
                pg->rchild = pp->lchild;
                pp->lchild = pg;
                p->lchild = pp;
            } else {
                pp->lchild = p->rchild;
                pg->rchild = p->lchild;
                p->rchild = pp;
                p->lchild = pg;
            }
        }

        if (r == NULL)
            root = p;
        else if (r->data.key > p->data.key)
            r->lchild = p;
        else
            r->rchild = p;
    }
    void Splay(BiTNode<T>* p)
    {
        BiTNode<T> *pg, *pp;

        if (p != root && p != NULL) {
            pp = Parent(p);
            pg = Parent(pp);

            if (pg == NULL)
                Zig(p);
            else {
                ZigZig(p, pp, pg);
                Splay(p);
            }
        }
    }
    void Delete(BiTNode<T>* &p)
    {
        BiTNode<T> *s = Parent(p), *q = p;

        if (p->rchild == NULL) {
            p = p->lchild;
            delete q;
        } else if (p->lchild == NULL) {
            p = p->rchild;
            delete q;
        } else {
            s = p->lchild;

            while (s->rchild != NULL) {
                q = s;
                s = s->rchild;
            }

            p->data = s->data;

            if (q != p)
                q->rchild = s->lchild;
            else
                q->lchild = s->lchild;

            delete s;
            s = q;
        }

        Splay(s);
    }
public:
    bool Insert(T e)
    {
        BiTNode<T> *p, *s;

        if (!BSTree<T>::SearchBST(root, e.key, NULL, p)) {
            s = new BiTNode<T>;
            s->data = e;
            s->lchild = s->rchild = NULL;

            if (p == NULL)
                root = s;
            else if LT(e.key, p->data.key) {
                p->lchild = s;
                Splay(p->lchild);
            } else {
                p->rchild = s;
                Splay(p->rchild);
            }

            return true;
        } else
            return false;
    }
    bool Search(KeyType key)
    {
        BiTNode<T> *p;
        bool f = BSTree<T>::SearchBST(root, key, NULL, p);
        Splay(p);
        return f;
    }
};
#endif

