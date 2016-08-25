#include "matrix.hpp" // recipe 11.13
#include "kmatrix.hpp" // recipe 11.14
#include <iostream>
#include <cassert>

using namespace std;

template<class M1, class M2, class M3>
void matrixMultiply(const M1& m1, const M2& m2, M3& m3)
{
    assert(m1.cols() == m2.rows());
    assert(m1.rows() == m3.rows());
    assert(m2.cols() == m3.cols());

    for (int i = m1.rows() - 1; i >= 0; --i) {
        for (int j = m2.cols() - 1; j >= 0; --j) {
            for (int k = m1.cols() - 1; k >= 0; --k) {
                m3[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
}

int main()
{
    matrix<int> m1(2, 1);
    matrix<int> m2(1, 2);
    kmatrix<int, 2, 2> m3;
    m3 = 0;
    m1[0][0] = 1;
    m1[1][0] = 2;
    m2[0][0] = 3;
    m2[0][1] = 4;
    matrixMultiply(m1, m2, m3);
    cout << "(" << m3[0][0] << ", " << m3[0][1] << ")" << endl;
    cout << "(" << m3[1][0] << ", " << m3[1][1] << ")" << endl;
}