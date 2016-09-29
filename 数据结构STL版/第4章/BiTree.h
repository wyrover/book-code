//BiTree.h 二叉链表结构的二叉树类（BiTree类）
#ifndef _BITREE_H_
#define _BITREE_H_
enum Tags {Left, Right};
enum style {Pre, In, Post};
template<typename T>struct StackElem {
    BiTNode<T> *p;
    Tags flag;
};
template<typename T>class BiTree
{
protected:
    BiTNode<T> *root;
private:
    void DestroyBiTree(BiTNode<T>* &t)
    {
        if (t != NULL) {
            DestroyBiTree(t->lchild);
            DestroyBiTree(t->rchild);
            delete t;
            t = NULL;
        }
    }
public:
    BiTree()
    {
        root = NULL;
    }
    ~BiTree()
    {
        DestroyBiTree(root);
    }
    void CreateBiTreeFromFile(ifstream &f)
    {
        T e;
        InputFromFile(f, e);

        if (e == Nil)
            return;

        root = new BiTNode<T>;
        assert(root != NULL);
        root->data = e;
        BiTree<T> son;
        son.CreateBiTreeFromFile(f);
        root->lchild = son.root;
        son.root = NULL;
        son.CreateBiTreeFromFile(f);
        root->rchild = son.root;
        son.root = NULL;
    }
    bool BiTreeEmpty()const
    {
        return root == NULL;
    }
    int BiTreeDepth(BiTNode<T>* t)const
    {
        int i, j;

        if (t == NULL)
            return 0;

        i = BiTreeDepth(t->lchild);
        j = BiTreeDepth(t->rchild);
        return i > j ? i + 1 : j + 1;
    }
    void PreOrderTraverse(void(*visit)(BiTNode<T>&))const
    {
        stack<BiTNode<T>*> s;
        BiTNode<T> *t = root;
        s.push(NULL);

        while (t != NULL) {
            visit(*t);

            if (t->rchild != NULL)
                s.push(t->rchild);

            if (t->lchild != NULL)
                t = t->lchild;
            else {
                t = s.top();
                s.pop();
            }
        }
    }
    void InOrderTraverse(void(*visit)(BiTNode<T>&))const
    {
        stack<BiTNode<T>*> s;
        BiTNode<T> *t = root;

        while (t != NULL || !s.empty()) {
            if (t != NULL) {
                s.push(t);
                t = t->lchild;
            } else {
                t = s.top();
                s.pop();
                visit(*t);
                t = t->rchild;
            }
        }

        cout << endl;
    }
    void PostOrderTraverse(void(*visit)(BiTNode<T>&))const
    {
        StackElem<T> se;
        stack<StackElem<T> > s;
        BiTNode<T> *t = root;

        if (t == NULL)
            return;

        while (!s.empty() || t != NULL) {
            while (t != NULL) {
                se.p = t;
                se.flag = Left;
                s.push(se);
                t = t->lchild;
            }

            se = s.top();
            s.pop();
            t = se.p;

            if (se.flag == Left) {
                se.flag = Right;
                s.push(se);
                t = t->rchild;
            } else {
                visit(*t);
                t = NULL;
            }
        }
    }
    void LevelOrderTraverse(void(*visit)(BiTNode<T>&))const
    {
        queue<BiTNode<T>*> q;
        BiTNode<T> *a, *t = root;

        if (t != NULL) {
            q.push(t);

            while (!q.empty()) {
                a = q.front();
                q.pop();
                visit(*a);

                if (a->lchild != NULL)
                    q.push(a->lchild);

                if (a->rchild != NULL)
                    q.push(a->rchild);
            }
        }

        cout << endl;
    }
    void OrderTraverse(BiTNode<T>* t, style mode, void(*visit)(BiTNode<T>&))const
    {
        if (t != NULL) {
            if (mode == Pre)
                visit(*t);

            OrderTraverse(t->lchild, mode, visit);

            if (mode == In)
                visit(*t);

            OrderTraverse(t->rchild, mode, visit);

            if (mode == Post)
                visit(*t);
        }
    }
    BiTNode<T>* Root()
    {
        return root;
    }
    bool InsertChild(BiTNode<T>* &p, bool LR, BiTree<T> &c)
    {
        BiTNode<T> *q = c.root;
        c.root = NULL;

        if (p != NULL) {
            if (!LR) {
                q->rchild = p->lchild;
                p->lchild = q;
            } else {
                q->rchild = p->rchild;
                p->rchild = q;
            }

            return true;
        }

        return false;
    }
    bool DeleteChild(BiTNode<T>* &p, bool LR)
    {
        if (p != NULL) {
            if (!LR)
                DestroyBiTree(p->lchild);
            else
                DestroyBiTree(p->rchild);

            return true;
        } else
            return false;
    }
    T Value(BiTNode<T>* p)const
    {
        return p->data;
    }
    void Assign(BiTNode<T>* p, T value)const
    {
        p->data = value;
    }
    BiTNode<T>* Parent(BiTNode<T>* p)const
    {
        queue<BiTNode<T>*> q;
        BiTNode<T> *a;

        if (root != NULL) {
            q.push(root);

            while (!q.empty()) {
                a = q.front();
                q.pop();

                if (a->lchild != NULL && a->lchild == p || a->rchild != NULL && a->rchild == p)
                    return a;
                else {
                    if (a->lchild != NULL)
                        q.push(a->lchild);

                    if (a->rchild != NULL)
                        q.push(a->rchild);
                }
            }
        }

        return NULL;
    }
    void Child(BiTNode<T>* p, BiTNode<T>* &left, BiTNode<T>* &right)const
    {
        left = p->lchild;
        right = p->rchild;
    }
    bool Sibling(BiTNode<T>* p, BiTNode<T>* &sib, bool &LR)const
    {
        BiTNode<T> *q = Parent(p);

        if (q == NULL)
            return false;

        if (q->lchild == p) {
            sib = q->rchild;
            LR = true;
        } else {
            sib = q->lchild;
            LR = false;
        }

        return sib != NULL;
    }
};
#endif

