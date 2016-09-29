//Algo3-3.cpp 用类实现BM算法的程序
#include "C.h"
#include "HString.h"
const int N = 256;
template<typename T>class BMMatching
{
private:
    vector<int> BC, GS, SS;
    int p;
    void buildBC(T P)
    {
        BC.assign(N, -1);

        for (int j = 0; j < p; j++)
            BC[(unsigned char)P[j]] = j;
    }
    void buildSS(T P)
    {
        SS.assign(p);
        SS[p - 1] = p;

        for (int lo = p - 1, hi = p - 1, j = lo - 1; j >= 0; j--)
            if (lo < j && SS[p - hi + j - 1] <= j - lo)
                SS[j] = SS[p - hi + j - 1];
            else {
                hi = j;
                lo = __min(lo, hi);

                while (0 <= lo && P[lo] == P[p - hi + lo - 1])
                    lo--;

                SS[j] = hi - lo;
            }
    }
    void buildGS(T P)
    {
        int i, j;
        buildSS(P);
        GS.assign(p, p);

        for (i = 0, j = p - 1; j >= 0; j--) {
            if (j + 1 == SS[j])
                while (i < p - j - 1)
                    GS[i++] = p - j - 1;
        }

        for (j = 0; j < p - 1; j++)
            GS[p - SS[j] - 1] = p - j - 1;

        cout << "GS=";

        for (i = 0; i < p; i++)
            cout << GS[i] << ' ';

        cout << endl;
    }
public:
    int match(T P, T Q, int pos, bool Bad, bool Good)
    {
        if (Bad || Good) {
            p = P.size();

            if (Bad)
                buildBC(P);

            if (Good)
                buildGS(P);

            int i = pos;

            while (Q.size() >= i + p) {
                int j = p - 1;

                while (P[j] == Q[i + j])
                    if (--j < 0)
                        break;

                if (j < 0)
                    break;
                else {
                    int B = INT_MIN, G = INT_MIN;

                    if (Bad)
                        B = __max(1, j - BC[(unsigned char)Q[i + j]]);

                    if (Good)
                        G = GS[j];

                    i += __max(G, B);
                    cout << "i=" << i << endl;
                }
            }

            return i;
        }

        return Q.size();
    }
};
void main()
{
    typedef string T;
//  typedef HString T;
    BMMatching<T> BM;
    T P = "dcabcaabc", Q = "asdrcabdcabcaabcytu";
    cout << "主串：" << Q << " 子串：" << P << endl;
    int index, pos = 1;
    bool B, G;

    for (int flag = 0; flag < 3; flag++) {
        B = false, G = false;

        switch (flag) {
        case 0:
            B = true;
            cout << "坏字符：" << endl;
            break;

        case 1:
            G = true;
            cout << "好后缀：" << endl;
            break;

        case 2:
            B = true;
            G = true;
            cout << "坏字符加好后缀：" << endl;
            break;
        }

        index = BM.match(P, Q, pos, B, G);

        if (Q.size() < index + P.size())
            cout << "由[" << pos << "]开始的主串和模式串匹配不成功。" << endl;
        else
            cout << "由[" << pos << "]开始的主串在[" << index << "]和模式串首次匹配。" << endl;
    }
}

