//BSTree.h 二叉排序树的类（BSTree类）
#ifndef _BSTREE_H_
#define _BSTREE_H_
#include "BSDTree.h"
template<typename T>class BSTree: public BSDTree<T>
{
private:
    void Delete(BiTNode<T>* &p)
    {
        BiTNode<T> *s, *q = p;

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
        }
    }
protected:
    bool SearchBST(BiTNode<T>* &p, KeyType key, BiTNode<T>* f, BiTNode<T>* &q)const
    {
        if (p == NULL) {
            q = f;
            return false;
        } else if EQ(key, p->data.key) {
            q = p;
            return true;
        } else if LT(key, p->data.key)
            return SearchBST(p->lchild, key, p, q);
        else
            return SearchBST(p->rchild, key, p, q);
    }
public:
    BiTNode<T>* SearchBST(BiTNode<T>* p, KeyType key)const
    {
        if (p == NULL || EQ(key, p->data.key))
            return p;
        else if LT(key, p->data.key)
            return SearchBST(p->lchild, key);
        else
            return SearchBST(p->rchild, key);
    }
    bool Insert(T e)
    {
        BiTNode<T> *p, *s;

        if (!SearchBST(root, e.key, NULL, p)) {
            s = new BiTNode<T>;
            s->data = e;
            s->lchild = s->rchild = NULL;

            if (p == NULL)
                root = s;
            else if LT(e.key, p->data.key)
                p->lchild = s;
            else
                p->rchild = s;

            return true;
        } else
            return false;
    }
    void CreateBiTreeFromFile(ifstream &f)
    {
    }
    bool InsertChild(BiTNode<T>* &p, bool LR, BiTree<T> &c)
    {
        return false;
    }
    void Assign(BiTNode<T>* p, T value)
    {
    }
};
#endif

