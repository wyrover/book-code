//Bank1.h 模拟银行排队机事件的类（Bank1类）
#ifndef _BANK1_H_
#define _BANK1_H_
#include "BankQueue.h"
class Bank1: public BankQueue
{
private:
    LinkQueue<QElemType> q;
    vector<bool> chk;
    vector<QElemType> customer;
    int ChuangKou()
    {
        for (int i = 0; i < Qn; i++)
            if (chk[i])
                return i;

        return Qn;
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
        q.EnQueue(f);
        i = ChuangKou();

        if (i < Qn) {
            q.DeQueue(customer[i]);
            e.OccurTime = en.OccurTime + customer[i].Duration;
            e.NType = i;
            ev.push(e);
            chk[i] = false;
        }
    }
    void CustomerDeparture()
    {
        Event e;
        int i;
        i = en.NType;
        chk[i] = true;
        TotalTime += en.OccurTime - customer[i].ArrivalTime;

        if (!q.QueueEmpty()) {
            q.DeQueue(customer[i]);
            chk[i] = false;
            e.OccurTime = en.OccurTime + customer[i].Duration;
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
        cout << "窗口数：" << Qn << "，营业时长：" << CloseTime << "分钟，客户总数：";
        Initialize();
        chk.assign(Qn, true);
        customer.assign(Qn);
        Bank_Simulation();
    }
};
#endif

