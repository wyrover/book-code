#include <map>
#include <iostream>

using namespace std;

// This function will not compile
//void func(const map<int, int>& m)
//{
//  cout << m[1] << endl;  // Error
//}

// C++11 version
void func(const map<int, int>& m)
{
    auto iter = m.find(1);

    if (iter != end(m))
        cout << iter->second << endl;
}

/*
// Pre-C++11 version
void func(const map<int, int>& m)
{
    map<int, int>::const_iterator iter = m.find(1);
    if (iter != end(m))
        cout << iter->second << endl;
}
*/

int main()
{
    map<int, int> m;
    m[1] = 11;
    m[2] = 22;
    m[3] = 33;
    func(m);
    return 0;
}
