//Algo2-2.cpp 将十进制转换为二～十六进制
#include "C.h"
#include "SqStack.h"
void Conversion(int m, int n)
{
    SqStack<int> S(5);
    int e;
    char c;

    if (m >= 0 && n >= 2 && n <= 16) {
        while (m) {
            S.Push(m % n);
            m = m / n;
        }

        while (!S.StackEmpty()) {
            S.Pop(e);

            if (e <= 9)
                cout << e;
            else {
                c = e + 55;
                cout << c;
            }
        }

        cout << endl;
    }
}
void main()
{
    int m, n;
    cout << "将十进制整数m转换为n进制数，请输入：m（≥0） n（2～16）：";
    cin >> m >> n;
    Conversion(m, n);
}

