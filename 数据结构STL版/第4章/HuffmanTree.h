//HuffmanTree.h 赫夫曼树类（HuffmanTree类）
#ifndef _HUFFMANTREE_H_
#define _HUFFMANTREE_H_
#include "HuffmanNode.h"
template<typename T>class HuffmanTree
{
private:
    vector<HTNode<T> >HT;
    int N;
    bool Make;
    void Select(int i, T &j1, T &j2)const
    {
        int j, m;

        for (j = 0; j < i, HT[j].parent != -1; j++);

        j1 = j;

        for (j = j1 + 1; j < i, HT[j].parent != -1; j++);

        if (HT[j].weight < HT[j1].weight) {
            j2 = j1;
            j1 = j;
        } else
            j2 = j;

        for (m = j + 1; m < i; m++)
            if (HT[m].parent == -1 && HT[m].weight < HT[j1].weight) {
                j2 = j1;
                j1 = m;
            } else if (HT[m].parent == -1 && HT[m].weight < HT[j2].weight)
                j2 = m;
    }
public:
    HuffmanTree()
    {
        Make = false;
    }
    void CreateHT(string FileName)
    {
        int i, s1, s2;
        ifstream fin(FileName.c_str());
        fin >> N;

        if (N <= 1)
            return;

        HT.assign(2 * N - 1);
        cout << N << "个权值依次是：";

        for (i = 0; i < N; i++) {
            fin >> HT[i].weight;
            cout << HT[i].weight << ' ';
            HT[i].parent = -1;
            HT[i].lchild = -1;
            HT[i].rchild = -1;
        }

        cout << endl;
        fin.close();

        for (i = N; i < HT.size(); i++)
            HT[i].parent = -1;

        for (i = N; i < HT.size(); i++) {
            Select(i, s1, s2);
            HT[s1].parent = HT[s2].parent = i;
            HT[i].lchild = s1;
            HT[i].rchild = s2;
            HT[i].weight = HT[s1].weight + HT[s2].weight;
        }

        Make = true;
    }
    void HuffmanCodingLeaf()const
    {
        if (Make) {
            string *HC = new string[N];
            assert(HC != NULL);
            cout << "从叶子到根逆向求得的赫夫曼编码为：" << endl;

            for (int i = 0; i < N; i++) {
                for (int c = i, p = HT[i].parent; p >= 0; c = p, p = HT[p].parent)
                    if (c == HT[p].lchild)
                        HC[i].insert(0, "0");
                    else
                        HC[i].insert(0, "1");

                cout << HC[i] << endl;
            }

            delete[]HC;
        }
    }
    void HuffmanCodingRoot()const
    {
        if (Make) {
            string str = "", *HC = new string[N];
            assert(HC != NULL);
            vector<int> s(HT.size(), 0);
            int c = HT.size() - 1;

            while (c > -1) {
                if (s[c] == 0) {
                    s[c] = 1;

                    if (HT[c].lchild > -1) {
                        c = HT[c].lchild;
                        str += '0';
                    } else {
                        HC[c] = str;
                        c = HT[c].parent;
                        str.erase(str.size() - 1, 1);
                    }
                } else if (s[c] == 1) {
                    s[c] = 2;
                    c = HT[c].rchild;
                    str += '1';
                } else {
                    c = HT[c].parent;

                    if (c > -1)
                        str.erase(str.size() - 1, 1);
                }
            }

            cout << "无栈非递归从根到叶子求得的赫夫曼编码为：" << endl;

            for (int i = 0; i < N; i++)
                cout << HC[i] << endl;

            delete[]HC;
        }
    }
};
#endif

