//Algo7-7.cpp 验证优先队列类的成员函数
#include "C.h"
#include "C4-1.h"
typedef int KeyType;
#include "PriQueue.h"
#include "Func7-1.h"
void main()
{
    PriQueue<ID> pq;
    bool flag = true;
    ID e;
    ifstream fin("F7-1.txt");

    while (!fin.eof()) {
        if (InputFromFile(fin, e))
            pq.EnQueue(e, flag);
    }

    fin.close();
    pq.GetHead(e);
    cout << "队头元素是";
    Visit(e);
    cout << "，依次出队优先队列的数据：" << endl;

    while (!pq.QueueEmpty()) {
        pq.DeQueue(e, flag);
        Visit(e);
    }

    cout << endl;
}

