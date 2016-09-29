//Algo3-4.cpp Karp-Rabin算法
#include "C.h"
#include "HString.h"
const int R = 255;
const int M = INT_MAX / R / 2;
template<typename T>class Karp_Rabin
{
private:
    int p, Dp;
    T P, Q;
    void CountDp()
    {
        Dp = 1;

        for (int i = 1; i < p; i++)
            Dp = (R * Dp) % M;
    }
    int Digit(char S)
    {
        return int((unsigned char)S);
    }
    bool Check(int i)
    {
        for (int j = 0; j < p; j++, i++)
            if (P[j] != Q[i])
                return false;

        return true;
    }
    void Update(int &hashQ, int k)
    {
        hashQ = ((hashQ - Digit(Q[k - 1]) * Dp) % M + M) % M;
        hashQ = (hashQ * R + Digit(Q[k + p - 1])) % M;
    }
public:
    int Match(T P1, T Q1, int pos)
    {
        P = P1, Q = Q1;
        p = P.size();
        int t = Q.size();
        CountDp();
        int hashP = 0, hashQ = 0;

        for (int i = 0; i < p; i++) {
            hashP = (hashP * R + Digit(P[i])) % M;
            hashQ = (hashQ * R + Digit(Q[i + pos])) % M;
        }

        for (int k = pos; k <= t - p; k++) {
            if (hashQ == hashP) {
                cout << "k=" << k << "，指纹匹配，";

                if (Check(k)) {
                    cout << "找到" << endl;
                    return k;
                }
            }

            Update(hashQ, k + 1);
        }

        cout << "没找到" << endl;
        return k;
    }
};
void main()
{
//  typedef string T;//主程序第1行
    typedef HString T;
    T p = "数据", q = "算法与数据结构";
//  T p="cabcaabc", q="asdrcabcaabcytu";
    cout << "主串q=" << q << "，模式串p=" << p << endl;
    Karp_Rabin<T> K;
    int pos = 3;
    int indes = K.Match(p, q, pos);
    cout << "indes=" << indes << endl;

    if (q.size() >= indes + p.size())
        cout << "由[" << pos << "]开始的主串在[" << indes << "]首次与模式串匹配" << endl;
}

