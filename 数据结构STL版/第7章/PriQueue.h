//PriQueue.h 优先队列类
#ifndef _PRIQUEUE_H_
#define _PRIQUEUE_H_
#include "Heap.h"
template<typename D>class PriQueue: public Heap<D>
{
public:
    void EnQueue(D e, bool flag)
    {
        elem.push_back(e);
        HeapAdjustUp(elem.size() - 1, flag);
    }
    bool DeQueue(D &e, bool flag)
    {
        if (elem.size() == 0)
            return false;

        e = elem[0];
        elem[0] = elem[elem.size() - 1];
        elem.erase(elem.end() - 1);
        HeapAdjustDown(0, elem.size() - 1, flag);
        return true;
    }
    bool QueueEmpty()const
    {
        return elem.size() == 0;
    }
    bool GetHead(D &e)const
    {
        if (elem.size() != 0)
            e = elem[0];

        return elem.size() != 0;
    }
};
#endif

