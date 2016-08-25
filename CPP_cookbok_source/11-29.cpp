#include "matrix.hpp"

#include <iostream>

using namespace std;

int main()
{
    matrix<int> m(2, 2);
    m = 0;
    m[0][0] = 1;
    m[1][1] = 1;
    m *= 2;
    cout << "(" << m[0][0] << "," << m[0][1] << ")" << endl;
    cout << "(" << m[1][0] << "," << m[1][1] << ")" << endl;
}