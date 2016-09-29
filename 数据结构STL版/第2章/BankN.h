//BankN.h 模拟银行多队列排队事件的类（BankN类）
#ifndef _BANKN_H_
#define _BANKN_H_
#include "BankQueue.h"
class BankN: public BankQueue
{
private:
    LinkQueue<QElemType> *q;
    int Minimum()
    {
        int minlen, k = 0;
        minlen = q[0].QueueLength();

        for (int i = 1; i < Qn; i++)
            if (q[i].QueueLength() < minlen) {
                minlen = q[i].QueueLength();
                k = i;
            }

        return k;
    }
    void CustomerArrived()
    {
        Event e;
        QElemType f;
        int DurationTime, IntervalTime, i;
        CustomerNum++;
        Random(DurationTime, IntervalTime);
        e.OccurTime = en.OccurTime + IntervalTime;
        e.NType = Qn;

        if (e.OccurTime < CloseTime)
            ev.push(e);

        f.ArrivalTime = en.OccurTime;
        f.Duration = DurationTime;
        i = Minimum();
        q[i].EnQueue(f);

        if (q[i].QueueLength() == 1) {
            e.OccurTime = f.ArrivalTime + f.Duration;
            e.NType = i;
            ev.push(e);
        }
    }
    void CustomerDeparture()
    {
        Event e;
        QElemType f;
        int i;
        i = en.NType;
        q[i].DeQueue(f);
        TotalTime += en.OccurTime - f.ArrivalTime;

        if (!q[i].QueueEmpty()) {
            q[i].GetHead(f);
            e.OccurTime = en.OccurTime + f.Duration;
            e.NType = i;
            ev.push(e);
        }
    }
public:
    void Bank(int n = 2, int k = 480, int i = 8, int j = 20)
    {
        Qn = n;
        CloseTime = k;
        Khjg = i;
        Blsj = j;
        cout << "两相邻到达的客户的时间间隔：0～" << Khjg;
        cout << "分钟，每个客户办理业务的时间：1～" << Blsj << "分钟" << endl;
        cout << "队列数：" << Qn << "，营业时长：" << CloseTime << "分钟，客户总数：";
        Initialize();
        q = new LinkQueue<QElemType>[Qn];
        Bank_Simulation();
    }
    ~BankN()
    {
        delete[]q;
    }
};
#endif

