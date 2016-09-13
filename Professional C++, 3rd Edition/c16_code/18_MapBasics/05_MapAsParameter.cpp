#include <map>
#include <iostream>

using namespace std;

void func(const map<int, int>& m)
{
//  cout << m[1] << endl;  // Error
}

int main()
{
    map<int, int> m;
    m[1] = 11;
    m[2] = 22;
    m[3] = 33;
    func(m);
    return 0;
}
