//Algo7-9.cpp 用STL实现字符串的基数排序
#include "C.h"
const int RADIX = 5;
template<typename D>class RadixSort
{
private:
    vector<D> barrel[RADIX + 1];
    vector<D> v;
    int ord(char c)
    {
        return c - 96;
    }
    int Keylength()
    {
        vector<D>::const_iterator it;
        it = max_element(v.begin(), v.end(), com);
        return it->key.size();
    }
    void Distribute(int i)
    {
        int j, k;

        for (k = 0; k < v.size(); k++) {
            if (v[k].key.length() <= i)
                j = 0;
            else
                j = ord(v[k].key[i]);

            barrel[j].push_back(v[k]);
        }

        v.clear();
        PrintBarrel();
    }
    void Collect()
    {
        for (int j = 0; j <= RADIX; j++) {
            v.insert(v.end(), barrel[j].begin(), barrel[j].end());
            barrel[j].clear();
        }

        Print();
    }
    void PrintBarrel()
    {
        vector<D>::const_iterator it;

        for (int i = 0; i <= RADIX; i++) {
            cout << "barrel[" << i << "]：" << endl;

            for (it = barrel[i].begin(); it != barrel[i].end(); it++)
                Visit(*it);
        }
    }
public:
    void CreatFromFile(string  FileName)
    {
        D d;
        ifstream fin(FileName.c_str());

        while (!fin.eof()) {
            if (InputFromFile(fin, d))
                v.push_back(d);
        }

        fin.close();
    }
    void Sort()
    {
        for (int i = Keylength() - 1; i >= 0; i--) {
            cout << "对key[" << i << "]的分配：" << endl;
            Distribute(i);
            cout << "对key[" << i << "]的收集：" << endl;
            Collect();
        }
    }
    void Print()
    {
        for (vector<D>::const_iterator it = v.begin(); it != v.end(); it++)
            Visit(*it);
    }
};
#include "Func7-2.h"
bool com(Rec a, Rec b)
{
    return a.key.size() < b.key.size();
}
void main()
{
    RadixSort<Rec> lr;
    lr.CreatFromFile("F7-2.txt");
    cout << "排序前lr.v中的元素：" << endl;
    lr.Print();
    lr.Sort();
    cout << "排序后lr.v中的元素：" << endl;
    lr.Print();
}

