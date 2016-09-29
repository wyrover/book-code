//Algo3-2.cpp KMP和改进的KMP算法
#include "C.h"
#include "HString.h"
template<typename T>class KMP
{
private:
    vector<int> Next;
    void printNext()
    {
        for (int i = 0; i < Next.size(); i++)
            cout << Next[i] << ' ';

        cout << endl;
    }
    void buildNext(T P, bool flag)
    {
        int i = 0;
        int j = -1;

        while (i < Next.size() - 1)
            if (j < 0 || P[i] == P[j]) {
                i++, j++;

                if (flag)
                    Next[i] = j;
                else
                    Next[i] = (P[i] != P[j]) ? j : Next[j];
            } else
                j = Next[j];
    }
public:
    int Match(T P, T R, int pos, bool flag)
    {
        T Q;
        Q = R.substr(pos, R.size() - pos);
        int n = Q.size(), i = 0;
        int m = P.size(), j = 0;
        Next.assign(m, -1);
        buildNext(P, flag);

        while (j < m && i < n) {
            if (0 > j || Q[i] == P[j])
                i++, j++;
            else
                j = Next[j];
        }

        if (!flag)
            cout << "改进的";

        cout << "KMP算法的next数组：";
        printNext();
        return i - j + pos;
    }
};
void main()
{
    typedef string T;
//  typedef HString T;//主程序第2行
    T P[2] = {"aaaab", "数据"}, Q[2] = {"aaabaaaab", "算法与数据结构"};
    KMP<T> kmp;

    for (int j = 0; j < 2; j++)
        for (int i = 0; i < 2; i++) {
            int k = 2;
            int pos = kmp.Match(P[j], Q[j], k, 0 == i);
            cout << Q[j] << "从[" << k << "]起和" << P[j];

            if (Q[j].size() < pos + P[j].size())
                cout << "不匹配。pos=" << pos << endl;
            else
                cout << "在主串的[" << pos << "]处首次匹配。" << endl;
        }
}

