//Algo1-5.cpp STL中表（链表）的应用
#include "C.h"
void main()
{
    list<int> a;
    list<int>::iterator ip;
    int e;

    for (int i = 0; i < 5; i++)
        a.push_back(i);

    a.push_front(1);
    ip = a.begin();
    advance(ip, 2);
    a.insert(ip, 9);

    for (ip = a.begin(); ip != a.end(); ip++)
        cout << *ip << ' ';

    cout << endl << "a.size()=" << a.size();
    ip = a.begin();
    advance(ip, 4);
    e = *ip;
    cout << " e=" << e << endl;
    a.erase(ip);
    a.remove(1);

    for (ip = a.begin(); ip != a.end(); ip++)
        cout << *ip << ' ';

    cout << endl << "a.empty()=" << boolalpha << a.empty();
    cout << " a.front()=" << a.front() << " a.back()=" << a.back() << endl;
    a.sort();

//  a.sort(greater<int>());//降序
    for (ip = a.begin(); ip != a.end(); ip++)
        cout << *ip << ' ';

    cout << endl;
    a.pop_front();
    a.pop_back();

    for (ip = a.begin(); ip != a.end(); ip++)
        cout << *ip << ' ';

    cout << endl;
    a.clear();
}

