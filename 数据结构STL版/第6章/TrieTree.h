//TrieTree.h Trie树类（TrieTree类）
#ifndef _TRIETREE_H_
#define _TRIETREE_H_
const int LENGTH = 26;
#include "TrieNode.h"
template<typename D>class TrieTree
{
private:
    TrieNode *root;
    vector<D> record;
    int MinEmpt;
    int ord(char c)const
    {
        if (c == Empty)
            return -1;
        else
            return c - 'A';
    }
    void DestroyTrie(TrieNode* &p)
    {
        if (p != NULL) {
            for (int i = 0; i < LENGTH; i++)
                if (p->ptr[i] != NULL)
                    DestroyTrie(p->ptr[i]);

            delete p;
            p = NULL;
        }
    }
public:
    TrieTree()
    {
        root = NULL;
        MinEmpt = 0;
    }
    ~TrieTree()
    {
        DestroyTrie(root);
    }
    void CreateFromFile(string FileName)
    {
        D r;
        ifstream fin(FileName.c_str());

        while (!fin.eof())
            if (InputFromFile(fin, r)) {
                bool f = InsertTrie(r);

                if (!f)
                    cout << "record中已有关键字为" << r.key << "的数据，无法插入。" << endl;
            }

        fin.close();
    }
    int SearchTrie(string Key)const
    {
        const TrieNode *p = root;

        for (int i = 0; p != NULL && i < Key.size(); i++)
            p = p->ptr[ord(Key[i])];

        if (p != NULL)
            return p->index;
        else
            return -1;
    }
    bool InsertTrie(D r)
    {
        TrieNode *p, *q;
        int i, k;
        int rc = SearchTrie(r.key);

        if (rc != -1)
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

        if (root == NULL) {
            p = root = new TrieNode;

            for (i = 0; i < r.key.size(); i++) {
                p->ptr.assign(LENGTH, NULL);
                p->index = -1;
                p = p->ptr[ord(r.key[i])] = new TrieNode;
            }

            p->ptr.assign(LENGTH, NULL);
            p->index = k;
        } else {
            p = root;

            for (i = 0; p != NULL && i < r.key.size(); i++) {
                q = p;
                p = p->ptr[ord(r.key[i])];
            }

            if (p == NULL) {
                p = q->ptr[ord(r.key[i - 1])] = new TrieNode;

                for (; i < r.key.size(); i++) {
                    p->ptr.assign(LENGTH, NULL);
                    p->index = -1;
                    p = p->ptr[ord(r.key[i])] = new TrieNode;
                }

                p->ptr.assign(LENGTH, NULL);
            }

            p->index = k;
        }

        return true;
    }
    struct orderptr {
        TrieNode *ptr;
        int order;
    };
    bool DeleteTrie(string Key, D &R)
    {
        TrieNode *p = root;
        stack<orderptr> S;
        orderptr op;
        int i, j, count;
        int rc = SearchTrie(Key);

        if (rc == -1)
            return false;

        for (i = 0; i < Key.size(); i++) {
            for (j = 0, count = 0; j < LENGTH, count < 2; j++)
                if (p->ptr[j] != NULL)
                    count++;

            if (count == 2)
                while (!S.empty())
                    S.pop();
            else {
                op.ptr = p;
                op.order = ord(Key[i]);
                S.push(op);
            }

            p = p->ptr[ord(Key[i])];
        }

        R = record[p->index];
        record[p->index].key[0] = Empty;

        if (p->index < MinEmpt)
            MinEmpt = p->index;

        p->index = -1;

        while (!S.empty()) {
            op = S.top();
            S.pop();

            if (op.ptr->ptr[op.order]->index == -1) {
                delete op.ptr->ptr[op.order];
                op.ptr->ptr[op.order] = NULL;
            }
        }

        if (root == NULL) {
            delete root;
            root = NULL;
        }

        return true;
    }
    TrieNode* Root()const
    {
        return root;
    }
    void TraverseTrie(TrieNode* p, void(*visit)(D))const
    {
        if (p != NULL) {
            if (p->index != -1)
                visit(record[p->index]);

            for (int i = 0; i < LENGTH; i++)
                if (p->ptr[i] != NULL)
                    TraverseTrie(p->ptr[i], visit);
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

