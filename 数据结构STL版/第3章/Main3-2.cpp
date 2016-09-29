//Main3-2.cpp 验证多维数组MuArray类的成员函数
#include "C.h"
#include "MuArray.h"
void main()
{
    int i, j, k, dim = 3, bound1 = 3, bound2 = 4, bound3 = 2;
    int e;
    MuArray<int> A;
    A.Array(dim, bound1, bound2, bound3);
    cout << bound1 << "页" << bound2 << "行" << bound3 << "列矩阵元素如下：" << endl;

    for (i = 0; i < bound1; i++) {
        for (j = 0; j < bound2; j++) {
            for (k = 0; k < bound3; k++) {
                A.Assign(i * 100 + j * 10 + k, i, j, k);
                A.Value(e, dim, i, j, k);
                cout << "A[" << i << "][" << j << "][" << k << "]=" << setw(3) << e << "  ";
            }

            cout << endl;
        }

        cout << endl;
    }
}

