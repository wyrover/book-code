//Algo2-8.cpp STL中队列的应用
#include "C.h"
void main()
{
    queue<int> q;

    for (int n = 1; n <= 5; n++)
        q.push(n);

    cout << "队尾元素为" << q.back() << endl;

    while (!q.empty()) {
        cout << q.front() << ' ';
        q.pop();
    }

    cout << endl;
    cout << "现在队列q中有" << q.size() << "个元素" << endl;
}

