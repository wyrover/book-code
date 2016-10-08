#include <list>
#include <iostream>
using namespace std;

int main()
{
    list<int> mylist(5, 100);   //5个100
    mylist.push_front(-13);
    mylist.push_back(300);      //表尾插入
    list<int> :: iterator it = mylist.begin();
    //删除元素
    mylist.erase(it);

    //遍历输出
    for (it = mylist.begin(); it != mylist.end(); ++it)
        cout << " " << *it;

    cout << endl;
    mylist.clear();
    return 0;
}

