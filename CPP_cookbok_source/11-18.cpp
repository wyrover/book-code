#include "kvector.hpp"

#include <algorithm>
#include <numeric>
#include <iostream>

using namespace std;

int main()
{
    kvector<int, 4> v = { 1, 2, 3, 4 };
    cout << "sum = " << accumulate(v.begin(), v.end(), 0) << endl;
    v *= 3;
    cout << "sum = " << accumulate(v.begin(), v.end(), 0) << endl;
    v += 1;
    cout << "sum = " << accumulate(v.begin(), v.end(), 0) << endl;
}