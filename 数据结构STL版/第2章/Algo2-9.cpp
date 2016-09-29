//Algo2-9.cpp STL中双端队列的应用
#include "C.h"
void out(int a)
{
    cout << a << ' ';
}
void main()
{
    deque<int> q;
    int a, b;

    for (int n = 1; n <= 5; n++) {
        q.push_back(n);
        q.push_front(n * 2);
    }

    for_each(q.begin(), q.end(), out);
    cout << endl << "双端队列元素个数为" << q.size() << endl;
    a = q.front();
    b = q.back();
    cout << "队头元素=" << a << "，队尾元素=" << b << endl;
    q.pop_front();
    q.pop_back();
    for_each(q.begin(), q.end(), out);
    cout << endl;
}

