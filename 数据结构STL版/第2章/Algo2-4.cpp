//Algo2-4.cpp 用递归函数求解汉诺塔问题
#include "C.h"
class HANOI
{
private:
    static int c;
    inline void move(int x, int z, int n)const
    {
        cout << "第" << ++c << "步：将" << n << "号盘从" << x << "座移到" << z << "座" << endl;
    }
public:
    void hanoi(int n, int x, int y, int z)const
    {
        if (n == 1)
            move(x, z, 1);
        else {
            hanoi(n - 1, x, z, y);
            move(x, z, n);
            hanoi(n - 1, y, x, z);
        }
    }
};
int HANOI::c = 0;
void main()
{
    HANOI H;
    int n;
    cout << "3个塔座为1、2、3，圆盘最初在1座，借助2座移到3座。请输入圆盘数：";
    cin >> n;
    H.hanoi(n, 1, 2, 3);
}

