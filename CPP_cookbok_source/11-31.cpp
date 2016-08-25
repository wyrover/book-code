#include "kmatrix.hpp"

#include <iostream>

using namespace std;

template<class Iter_T>
void outputRowOrColumn(Iter_T iter, int n)
{
    for (int i = 0; i < n; ++i) {
        cout << iter[i] << " ";
    }

    cout << endl;
}

template<class Matrix_T>
void initializeMatrix(Matrix_T& m)
{
    int k = 0;

    for (int i = 0; i < m.rows(); ++i) {
        for (int j = 0; j < m.cols(); ++j) {
            m[i][j] = k++;
        }
    }
}

template<class Matrix_T>
void outputMatrix(Matrix_T& m)
{
    for (int i = 0; i < m.rows(); ++i) {
        cout << "Row " << i << " = ";
        outputRowOrColumn(m.row(i), m.cols());
    }

    for (int i = 0; i < m.cols(); ++i) {
        cout << "Column " << i << " = ";
        outputRowOrColumn(m.col(i), m.rows());
    }
}

int main()
{
    kmatrix<int, 2, 4> m;
    initializeMatrix(m);
    m *= 2;
    outputMatrix(m);
}