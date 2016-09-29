//Main3-3.cpp 验证稀疏矩阵RLSMatrix类的成员函数
#include "C.h"
#include "RLSMatrix.h"
void main()
{
    bool f;
    RLSMatrix<int> A, B, C, D;
    A.MakeMatrixFromFile("F3-1.txt");
    B.MakeMatrixFromFile("F3-2.txt");
    C.MakeMatrixFromFile("F3-3.txt");
    cout << "矩阵A=" << endl;
    A.PrintSMatrix();
    cout << "矩阵B=" << endl;
    B.PrintSMatrix();
    cout << "矩阵C=" << endl;
    C.PrintSMatrix();
    D.CopySMatrix(A);
    cout << "由矩阵A复制的矩阵D=" << endl;
    D.PrintSMatrix();
    D.ClearSMatrix();
    f = D.AddSMatrix(A, C);

    if (f) {
        cout << "矩阵D=A+C=" << endl;
        D.PrintSMatrix();
    }

    B.ClearSMatrix();
    f = B.SubtSMatrix(D, A);

    if (f) {
        cout << "矩阵B=D-A=" << endl;
        B.PrintSMatrix();
    }

    D.ClearSMatrix();
    D.TransposeSMatrix(C);
    cout << "矩阵D=C的转置=" << endl;
    D.PrintSMatrix();
    A.ClearSMatrix();
    f = A.MultSMatrix(C, D);

    if (f) {
        cout << "矩阵A=C×D=" << endl;
        A.PrintSMatrix();
    }
}

