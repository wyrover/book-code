//Algo2-1.cpp STL中栈的应用
#include "C.h"
void main()
{
    stack<int> st;

    for (int n = 1; n <= 5; n++)
        st.push(n);

    cout << "栈中有" << st.size() << "个元素" << endl;

    while (!st.empty()) {
        cout << st.top() << ' ';
        st.pop();
    }

    cout << endl;
}

