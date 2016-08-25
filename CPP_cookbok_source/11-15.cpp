#include <valarray>
#include <iostream>

using namespace std;

int main()
{
    valarray<int> v(3);
    v[0] = 1;
    v[1] = 2;
    v[2] = 3;
    cout << v[0] << ", " << v[1] << ", " << v[2] << endl;
    v = v + v;
    cout << v[0] << ", " << v[1] << ", " << v[2] << endl;
    v /= 2;
    cout << v[0] << ", " << v[1] << ", " << v[2] << endl;
}