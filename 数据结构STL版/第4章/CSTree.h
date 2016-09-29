//CSTree.h 孩子-兄弟二叉链表结构的树或森林类（CSTree类）
#ifndef _CSTREE_H_
#define _CSTREE_H_
#include "CSNode.h"
template<typename VerT, typename ArcT>class DFST;
template<typename T>class CSTree
{
    friend DFST<struct V, struct A>;
private:
    CSNode<T> *root;
    void ClearTree(CSNode<T>* &t)
    {
        if (t != NULL) {
            ClearTree(t->firstchild);
            ClearTree(t->nextsibling);
            delete t;
            t = NULL;
        }
    }
public:
    CSTree()
    {
        root = NULL;
    }
    ~CSTree()
    {
        ClearTree(root);
    }
    void CreateTreeFromFile(string FileName)
    {
        ifstream fin(FileName.c_str());
        CSNode<T> *p;
        queue<CSNode<T>*> q;
        int i, m;
        fin >> m;
        root = new CSNode<T>;
        fin >> root->data;
        q.push(root);
        p = root;

        for (i = 1; i < m; i++) {
            p->nextsibling = new CSNode<T>;
            p = p->nextsibling;
            fin >> p->data;
            q.push(p);
        }

        p->nextsibling = NULL;

        while (!q.empty()) {
            p = q.front();
            q.pop();
            fin >> m;

            if (m > 0) {
                p = p->firstchild = new CSNode<T>;
                fin >> p->data;
                q.push(p);

                for (i = 1; i < m; i++) {
                    p->nextsibling = new CSNode<T>;
                    p = p->nextsibling;
                    fin >> p->data;
                    q.push(p);
                }

                p->nextsibling = NULL;
            } else
                p->firstchild = NULL;
        }

        fin.close();
    }
    bool TreeEmpty()const
    {
        return root == NULL;
    }
    int TreeDepth(CSNode<T> *t)const
    {
        CSNode<T> *p;
        int depth, max = 0;

        if (t == NULL)
            return 0;

        for (p = t->firstchild; p != NULL; p = p->nextsibling) {
            depth = TreeDepth(p);

            if (depth > max)
                max = depth;
        }

        return max + 1;
    }
    CSNode<T>* Point(T s)const
    {
        CSNode<T> *p;
        queue<CSNode<T>*> q;

        if (root != NULL) {
            q.push(root);

            while (!q.empty()) {
                p = q.front();
                q.pop();

                if (p->data == s)
                    return p;

                if (p->firstchild != NULL)
                    q.push(p->firstchild);

                if (p->nextsibling != NULL)
                    q.push(p->nextsibling);
            }
        }

        return NULL;
    }
    T Value(CSNode<T> *p)const
    {
        if (p != NULL)
            return p->data;
        else
            return Nil;
    }
    void PreOrderTraverse(CSNode<T>* t, void(*visit)(CSNode<T>&))const
    {
        if (t != NULL) {
            visit(*t);
            PreOrderTraverse(t->firstchild, visit);
            PreOrderTraverse(t->nextsibling, visit);
        }
    }
    void PostOrderTraverse(CSNode<T>* t, void(*visit)(CSNode<T>&))const
    {
        if (t != NULL) {
            PostOrderTraverse(t->firstchild, visit);
            visit(*t);
            PostOrderTraverse(t->nextsibling, visit);
        }
    }
    void LevelOrderTraverse(void(*visit)(CSNode<T>&))const
    {
        CSNode<T> *p = root;
        queue<CSNode<T>*> q;

        if (p != NULL) {
            while (p != NULL) {
                visit(*p);
                q.push(p);
                p = p->nextsibling;
            }

            while (!q.empty()) {
                p = q.front();
                q.pop();

                if (p->firstchild != NULL) {
                    p = p->firstchild;
                    visit(*p);
                    q.push(p);

                    while (p->nextsibling != NULL) {
                        p = p->nextsibling;
                        visit(*p);
                        q.push(p);
                    }
                }
            }
        }
    }
    CSNode<T>* Root()const
    {
        return root;
    }
    void Assign(CSNode<T>* p, T value)const
    {
        if (p != NULL)
            p->data = value;
    }
    T Parent(T e)const
    {
        CSNode<T> *t, *p = root;
        queue<CSNode<T>*> q;

        if (p != NULL) {
            while (p != NULL)
                if (Value(p) == e)
                    return Nil;
                else {
                    q.push(p);
                    p = p->nextsibling;
                }

            while (!q.empty()) {
                p = q.front();
                q.pop();

                if (p->firstchild != NULL) {
                    if (p->firstchild->data == e)
                        return p->data;

                    t = p;
                    p = p->firstchild;
                    q.push(p);

                    while (p->nextsibling != NULL) {
                        p = p->nextsibling;

                        if (p->data == e)
                            return t->data;

                        q.push(p);
                    }
                }
            }
        }

        return Nil;
    }
    T LeftChild(T e)const
    {
        CSNode<T> *f = Point(e);

        if (f != NULL && f->firstchild)
            return f->firstchild->data;
        else
            return Nil;
    }
    T RightSibling(T e)const
    {
        CSNode<T> *f = Point(e);

        if (f != NULL && f->nextsibling)
            return f->nextsibling->data;
        else
            return Nil;
    }
    bool InsertChild(CSNode<T> *p, int i, CSTree<T> &c)const
    {
        int j;
        CSNode<T> *q, *s = c.Root();
        c.root = NULL;

        if (root != NULL) {
            if (i == 1) {
                s->nextsibling = p->firstchild;
                p->firstchild = s;
            } else {
                q = p->firstchild;
                j = 2;

                while (q != NULL && j < i) {
                    q = q->nextsibling;
                    j++;
                }

                if (j == i) {
                    s->nextsibling = q->nextsibling;
                    q->nextsibling = s;
                } else
                    return false;
            }

            return true;
        } else
            return false;
    }
    bool DeleteChild(CSNode<T> *p, int i)
    {
        CSNode<T> *b, *q;
        int j;

        if (i == 1) {
            b = p->firstchild;
            p->firstchild = b->nextsibling;
            b->nextsibling = NULL;
            ClearTree(b);
        } else {
            q = p->firstchild;
            j = 2;

            while (q != NULL && j < i) {
                q = q->nextsibling;
                j++;
            }

            if (j == i) {
                b = q->nextsibling;
                q->nextsibling = b->nextsibling;
                b->nextsibling = NULL;
                ClearTree(b);
            } else
                return false;
        }

        return true;
    }
};
#endif

