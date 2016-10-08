#include <queue>
#include <iostream>
using namespace std;

int main()
{
    queue <int> q;
    q.push(1);
    q.push(2);
    q.push(3);
    q.push(9);
    cout << q.size() << endl;   //返回队例元素数量
    cout << q.empty() << endl;  //判断队列是否为空
    cout << q.front() << endl;  //读取队首元素
    cout << q.back() << endl;   //读取队尾元素

    //所有元素出列，即删除所有元素
    while (!q.empty()) {
        cout << q.front() << " ";
        q.pop();                //删除队首元素
    }

    return 0;
}

