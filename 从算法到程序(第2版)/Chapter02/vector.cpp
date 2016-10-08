#include <cstring>
#include <vector>
#include <iostream>
using namespace std;
int ar[10] = { 12, 45, 234, 64, 12, 35, 63, 23, 12, 55 };
char* str = "Hello World";
int main()
{
    vector <int> vec1(ar, ar + 10);
    vector < char > vec2(str, str + strlen(str));
    cout << "vec1:" << endl;
    vector<int>::iterator p;

    for (p = vec1.begin(); p != vec1.end(); ++p)
        cout << *p << " ";

    cout << '\n' << "vec2:" << endl;
    vector< char >::iterator p1;

    for (p1 = vec2.begin(); p1 != vec2.end(); ++p1)
        cout << *p1 << " ";

    cout << '\n';
    return 0;
}
