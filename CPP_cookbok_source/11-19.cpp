#include <numeric>
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    int v1[] = { 1, 2, 3 };
    int v2[] = { 4, 6, 8 };
    cout << "the dot product of (1,2,3) and (4,6,8) is ";
    cout << inner_product(v1, v1 + 3, v2, 0) << endl;
}