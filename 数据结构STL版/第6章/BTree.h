//BTree.h B树类（BTree类）
#ifndef _BTREE_H_
#define _BTREE_H_
#include "BTNode.h"
template<typename D>class BTree
{
private:
    int m;
    BTNode *root;
    vector<D> record;
    int MinEmpt;
    void DestroyBTree(BTNode* &t)
    {
        if (t != NULL) {
            for (int i = 0; i < t->children.size(); i++)
                DestroyBTree(t->children[i]);

            delete t;
            t = NULL;
        }
    }
    int Search(BTNode* p, KeyType k)const
    {
        for (int j = 0; j < p->ki.size(); j++)
            if GE(p->ki[j].key, k)
                break;

        return j;
    }
    void Insert(BTNode* q, int i, int r, BTNode* ap)
    {
        KeyIndex ki;
        ki.key = record[r].key;
        ki.index = r;
        q->ki.insert(q->ki.begin() + i, ki);
        q->children.insert(q->children.begin() + i + 1, ap);

        if (ap != NULL)
            ap->parent = q;
    }
    void split(BTNode* q, BTNode* &ap)
    {
        ap = new BTNode;
        int i;

        for (i = m / 2 + 1; i < m; i++)
            ap->ki.push_back(q->ki[i]);

        q->ki.erase(q->ki.begin() + m / 2, q->ki.end());

        for (i = m / 2 + 1; i <= m; i++) {
            if (q->children[i] != NULL)
                q->children[i]->parent = ap;

            ap->children.push_back(q->children[i]);
        }

        q->children.erase(q->children.begin() + m / 2 + 1, q->children.end());
    }
    void NewRoot(int r, BTNode* ap)
    {
        BTNode *p = new BTNode;
        KeyIndex ki;
        ki.key = record[r].key;
        ki.index = r;
        p->parent = NULL;
        p->ki.push_back(ki);
        p->children.push_back(root);

        if (root != NULL)
            root->parent = p;

        p->children.push_back(ap);

        if (ap != NULL)
            ap->parent = p;

        root = p;
    }
    void InsertBTree(int r, BTNode* q, int i)
    {
        BTNode *ap = NULL;
        bool finished = false;

        while (q != NULL && !finished) {
            Insert(q, i, r, ap);

            if (q->ki.size() < m)
                finished = true;
            else {
                r = q->ki[m / 2].index;
                split(q, ap);
                q = q->parent;

                if (q != NULL)
                    i = Search(q, record[r].key);
            }
        }

        if (!finished)
            NewRoot(r, ap);
    }
    bool Move(BTNode* &p)
    {
        BTNode *a, *f = p->parent;
        int i;

        for (i = 0; f->children[i] != p; i++);

        if (i > 0 && f->children[i - 1]->ki.size() > (m - 1) / 2) {
            a = f->children[i - 1];
            p->ki.insert(p->ki.begin(), f->ki[i - 1]);
            f->ki[i - 1] = a->ki.back();
            a->ki.pop_back();
            p->children.insert(p->children.begin(), a->children.back());
            a->children.pop_back();

            if (p->children[0] != NULL)
                p->children[0]->parent = p;

            return true;
        } else if (i < f->children.size() - 1 && f->children[i + 1]->ki.size() > (m - 1) / 2) {
            a = f->children[i + 1];
            p->ki.push_back(f->ki[i]);
            f->ki[i] = a->ki[0];
            a->ki.erase(a->ki.begin());

            if (a->children[0] != NULL)
                a->children[0]->parent = p;

            p->children.push_back(a->children[0]);
            a->children.erase(a->children.begin());
            return true;
        }

        return false;
    }
    BTNode* Merge(BTNode* &p)
    {
        BTNode *b, *f = p->parent;
        int i, j;

        for (i = 0; f->children[i] != p; i++);

        if (i > 0) {
            b = f->children[i - 1];

            for (j = 0; j < p->children.size(); j++) {
                if (p->children[j] != NULL)
                    p->children[j]->parent = b;

                b->children.push_back(p->children[j]);
            }

            f->children.erase(f->children.begin() + i);
            b->ki.push_back(f->ki[i - 1]);
            f->ki.erase(f->ki.begin() + i - 1);

            for (j = 0; j < p->ki.size(); j++)
                b->ki.push_back(p->ki[j]);

            delete p;
            return b;
        } else {
            b = f->children[i + 1];

            for (j = 0; j < b->children.size(); j++) {
                if (b->children[j] != NULL)
                    b->children[j]->parent = p;

                p->children.push_back(b->children[j]);
            }

            f->children.erase(f->children.begin() + i + 1);
            p->ki.push_back(f->ki[i]);
            f->ki.erase(f->ki.begin() + i);

            for (j = 0; j < b->ki.size(); j++)
                p->ki.push_back(b->ki[j]);

            delete b;
            return p;
        }
    }
public:
    BTree(int M = 3)
    {
        m = M;
        root = NULL;
        MinEmpt = 0;
    }
    ~BTree()
    {
        DestroyBTree(root);
    }
    Result SearchBTree(KeyType k)const
    {
        BTNode *p = root, *q = NULL;
        bool found = false;
        int i = 0;
        Result re;

        while (p != NULL && !found) {
            i = Search(p, k);

            if (p->ki.size() > i && p->ki[i].key == k)
                found = true;
            else {
                q = p;
                p = p->children[i];
            }
        }

        if (found) {
            re.pt = p;
            re.tag = true;
        } else {
            re.pt = q;
            re.tag = false;
        }

        re.i = i;
        return re;
    }
    bool InsertRecord(D r)
    {
        Result re = SearchBTree(r.key);

        if (re.tag)
            return false;

        int i, k;

        if (MinEmpt != record.size()) {
            record[MinEmpt] = r;
            k = MinEmpt;

            for (i = MinEmpt + 1; i < record.size(); i++)
                if (record[i].key == EMPTY)
                    break;

            MinEmpt = i;
        } else {
            record.push_back(r);
            k = MinEmpt;
            MinEmpt = record.size();
        }

        InsertBTree(k, re.pt, re.i);
        return true;
    }
    void CreateFromFile(string FileName)
    {
        D r;
        ifstream fin(FileName.c_str());

        while (!fin.eof())
            if (InputFromFile(fin, r)) {
                bool f = InsertRecord(r);

                if (!f)
                    cout << "record中已有关键字为" << r.key << "的数据，无法插入。" << endl;
            }

        fin.close();
    }
    BTNode* Root()const
    {
        return root;
    }
    bool DeleteBTree(D &r, KeyType k)
    {
        int i;
        BTNode *p, *q;
        Result re = SearchBTree(k);

        if (!re.tag)
            return false;

        p = re.pt;
        i = re.i;
        r = record[p->ki[i].index];
        record[p->ki[i].index].key = EMPTY;

        if (p->ki[i].index < MinEmpt)
            MinEmpt = p->ki[i].index;

        if (p->children[i] != NULL) {
            q = p->children[i];

            while (q->children.back() != NULL)
                q = q->children.back();

            if (q->ki.size() > (m - 1) / 2) {
                p->ki[i] = q->ki.back();
                p = q;
                i = q->ki.size() - 1;
            } else {
                q = p->children[i + 1];

                while (q->children[0] != NULL)
                    q = q->children[0];

                p->ki[i] = q->ki[0];
                p = q;
                i = 0;
            }
        }

        p->ki.erase(p->ki.begin() + i);
        p->children.erase(p->children.begin() + i + 1);

        while (p->ki.size() < (m - 1) / 2 && p != root)
            if (!Move(p)) {
                p = Merge(p);

                if (p != root)
                    p = p->parent;
            }

        if (p == root && root->ki.size() == 0) {
            root = root->children[0];
            delete p;

            if (root != NULL)
                root->parent = NULL;
        }

        return true;
    }
    void Traverse(BTNode* t, void(*visit)(D))const
    {
        if (t != NULL)
            for (int i = 0; i < t->children.size(); i++) {
                if (t->children[i] != NULL)
                    Traverse(t->children[i], visit);

                if (i < t->ki.size())
                    visit(record[t->ki[i].index]);
            }
    }
    void OutRecord(void(*visit)(D), int i)
    {
        if (record[i].key != EMPTY) {
            cout << "[" << i << "]";
            visit(record[i]);
            cout << endl;
        }
    }
    void Print(void(*visit)(D))
    {
        for (int i = 0; i < record.size(); i++)
            OutRecord(visit, i);
    }
};
#endif

