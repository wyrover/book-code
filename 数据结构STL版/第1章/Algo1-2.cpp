//Algo1-2.cpp STL中向量（顺序线性表）的应用
#include "C.h"
void out(int a)
{
    cout << a << ' ';
}
void main()
{
    vector<int> a;
    vector<int>::iterator ip;
    vector<int>::const_iterator cip;

    for (int i = 0; i < 5; i++)
        a.push_back(i);

    for_each(a.begin(), a.end(), out);
    cout << endl;
    ip = a.begin() + 2;
    a.insert(ip, 9);

    for (cip = a.begin(); cip != a.end(); cip++)
        cout << *cip << ' ';

    cout << endl << "a.size()=" << a.size();
    cout << " a.capacity=" << a.capacity();
    cout << " a.empty()=" << boolalpha << a.empty() << endl;
    a.erase(a.begin() + 4);
    for_each(a.begin(), a.end(), out);
    cout << endl;
    cout << "a.front()=" << a.front() << " a.back()=" << a.back() << " a[3]=" << a[3] << endl;
    sort(a.begin(), a.end());
//  sort(a.begin(), a.end(), greater<int>());//降序排列
    for_each(a.begin(), a.end(), out);
    cout << endl;
    reverse(a.begin(), a.end());
    for_each(a.begin(), a.end(), out);
    cout << endl;
    a.clear();
}

