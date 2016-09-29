//Algo8-3.cpp 用优先队列实现置换-选择排序类
#include "C.h"
typedef int KeyType;
template<typename D>class Replace_Select
{
public:
    void MakeSmallFile(const string FileName, int N)
    {
        priority_queue<WA<D>, vector<WA<D> >, cmp<D> > pri_queue;
        string s = "F00.txt";
        ofstream fout;
        ifstream fin(FileName.c_str());
        WA<D> wa, q;
        int k = 0;
        wa.num = k;

        for (int i = 0; i < N; i++)
            if (InputFromFile(fin, wa.data))
                pri_queue.push(wa);

        cout << s << "的内容为";
        fout.open(s.c_str());

        while (!pri_queue.empty()) {
            q = pri_queue.top();

            if (q.num > k) {
                fout.close();
                k++;
                cout << endl;
                s[2] = k % 10 + '0';
                s[1] = k / 10 % 10 + '0';
                cout << s << "的内容为";
                fout.open(s.c_str());
            }

            Visit(q.data);
            OutputToFile(fout, q.data);
            pri_queue.pop();

            if (!fin.eof())
                if (InputFromFile(fin, wa.data)) {
                    if (wa.data.key < q.data.key)
                        wa.num = q.num + 1;
                    else
                        wa.num = q.num;

                    pri_queue.push(wa);
                }
        }

        cout << endl;
        fout.close();
        fin.close();
        cout << "共产生" << k + 1 << "个有序小文件" << endl;
    }
};
template<typename D>struct WA {
    D data;
    int num;
};
template<typename D>class cmp
{
public:
    bool operator()(const WA<D> a, const WA<D> b)const
    {
        return a.num > b.num || a.num == b.num && a.data.key > b.data.key;
    }
};
#include "Func7-1.h"
#include "Func8-1.h"
void main()
{
    Replace_Select<ID> rs;
    rs.MakeSmallFile("F8-1.txt", 6);
}

