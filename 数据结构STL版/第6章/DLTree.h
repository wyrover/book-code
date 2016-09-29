//DLTree.h 双链键树类（DLTree类）
#ifndef _DLTREE_H_
#define _DLTREE_H_
#include "DLTNode.h"
template<typename D>class DLTree
{
private:
    DLTNode *dst;
    int MinEmpt;
    vector<D> record;
    void DestroyDLTree(DLTNode* &p)
    {
        if (p != NULL) {
            if (p->kind == BRANCH)
                DestroyDLTree(p->first);

            DestroyDLTree(p->next);
            delete p;
            p = NULL;
        }
    }
public:
    DLTree()
    {
        dst = new DLTNode;
        MinEmpt = 0;
    }
    ~DLTree()
    {
        DestroyDLTree(dst);
    }
    void CreateFromFile(string FileName)
    {
        D r;
        ifstream fin(FileName.c_str());

        while (!fin.eof())
            if (InputFromFile(fin, r)) {
                bool f = InsertDLTree(r);

                if (!f)
                    cout << "record中已有关键字为" << r.key << "的数据，无法插入。" << endl;
            }

        fin.close();
    }
    int SearchDLTree(string Key)const
    {
        DLTNode *p = dst->first;
        int i = 0;

        while (p != NULL && i < Key.size()) {
            while (p != NULL && p->symbol < Key[i])
                p = p->next;

            if (p != NULL && p->symbol == Key[i]) {
                p = p->first;
                i++;
            } else
                return -1;
        }

        if (p != NULL && p->kind == LEAF)
            return p->index;
        else
            return -1;
    }
    bool InsertDLTree(D r)
    {
        DLTNode *q = dst, *p = dst->first, *ap;
        int i, k;

        if (SearchDLTree(r.key) != -1)
            return false;

        if (MinEmpt != record.size()) {
            record[MinEmpt] = r;
            k = MinEmpt;

            for (i = MinEmpt + 1; i < record.size(); i++)
                if (record[i].key[0] == Empty)
                    break;

            MinEmpt = i;
        } else {
            record.push_back(r);
            k = MinEmpt;
            MinEmpt = record.size();
        }

        i = 0;

        while (p != NULL && i < r.key.size()) {
            while (p != NULL && p->symbol < r.key[i]) {
                q = p;
                p = p->next;
            }

            if (p != NULL && p->symbol == r.key[i]) {
                q = p;
                p = p->first;
                i++;
            } else
                break;
        }

        ap = new DLTNode;

        if (q->first == p)
            q->first = ap;
        else
            q->next = ap;

        if (i == r.key.size()) {
            ap->next = p;
            ap->symbol = Empty;
            ap->kind = LEAF;
            ap->index = k;
        } else {
            ap->next = p;
            ap->symbol = r.key[i];
            p = ap;
            ap = new DLTNode;

            for (i++; i < r.key.size(); i++) {
                p->first = ap;
                ap->symbol = r.key[i];
                p = ap;
                ap = new DLTNode;
            }

            p->first = ap;
            ap->symbol = Empty;
            ap->kind = LEAF;
            ap->index = k;
        }

        return true;
    }
    struct flagptr {
        DLTNode *ptr;
        bool flag;
    };
    bool DeleteDLTree(string Key, D &r)
    {
        DLTNode *p = dst, *q = NULL;
        stack<flagptr> S;
        flagptr fp;
        int rc = SearchDLTree(Key);

        if (rc == -1)
            return false;

        fp.ptr = p;
        fp.flag = false;
        S.push(fp);

        for (int i = 0; i < Key.size(); i++) {
            p = p->first;

            while (p->symbol < Key[i]) {
                fp.ptr = p;
                fp.flag = true;
                S.push(fp);
                p = p->next;
            }

            fp.ptr = p;
            fp.flag = false;
            S.push(fp);
        }

        p = p->first;
        r = record[p->index];
        record[p->index].key[0] = Empty;

        if (p->index < MinEmpt)
            MinEmpt = p->index;

        while (!S.empty() && q == NULL) {
            q = p->next;
            fp = S.top();
            S.pop();

            if (fp.flag)
                fp.ptr->next = q;
            else
                fp.ptr->first = q;

            delete p;
            p = fp.ptr;

            if (p->first != NULL)
                break;
        }

        return true;
    }
    DLTNode* Root()const
    {
        return dst;
    }
    void TraverseDLTree(DLTNode* p, void(*visit)(D))const
    {
        if (p != NULL) {
            if (p->kind == BRANCH)
                TraverseDLTree(p->first, visit);
            else
                visit(record[p->index]);

            TraverseDLTree(p->next, visit);
        }
    }
    void OutRecord(void(*visit)(D), int i)
    {
        visit(record[i]);
    }
    void Print(void(*visit)(D))
    {
        for (int i = 0; i < record.size(); i++)
            if (record[i].key[0] != Empty) {
                cout << "[" << i << "]";
                visit(record[i]);
                cout << endl;
            }
    }
};
#endif

