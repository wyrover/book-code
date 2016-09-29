//Algo2-10.cpp STL中优先队列的应用
#include "C.h"
#include "PosType.h"
class cmp
{
public:
    bool operator()(const PosType a, const PosType b)const
    {
        return a.row > b.row;
//      return a.row<b.row;//按PosType的row域降序排序
    }
};
void main()
{
    PosType p[] = {{3, 4}, {5, 6}, {4, 1}}, q;
    priority_queue<PosType, vector<PosType>, cmp> pri_queue;
//  priority_queue<PosType, deque<PosType>, cmp> pri_queue;//容器为双端队列

    for (int i = 0; i < 3; i++)
        pri_queue.push(p[i]);

    cout << "优先队列元素数=" << pri_queue.size() << endl;

    while (!pri_queue.empty()) {
        q = pri_queue.top();
        cout << '(' << q.row << ", " << q.col << ") ";
        pri_queue.pop();
    }

    cout << endl;
}

