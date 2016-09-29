//StLinkList.h 静态链表的类（StLinkList类）
#ifndef _STLINKLIST_H_
#define _STLINKLIST_H_
const int MAX_SIZE = 10;
template<typename T>class StLinkList
{
    struct component {
        T data;
        int link;
    };
private:
    component SL[MAX_SIZE];
    int NEW()
    {
        int i = SL[MAX_SIZE - 1].link;

        if (i)
            SL[MAX_SIZE - 1].link = SL[i].link;

        return i;
    }
    void DELETE(int k)
    {
        SL[k].link = SL[MAX_SIZE - 1].link;
        SL[MAX_SIZE - 1].link = k;
    }
public:
    StLinkList()
    {
        int i;
        SL[0].link = 0;
        SL[MAX_SIZE - 1].link = 1;

        for (i = 1; i < MAX_SIZE - 2; i++)
            SL[i].link = i + 1;

        SL[MAX_SIZE - 2].link = 0;
    }
    void ClearList()
    {
        int j, i = SL[MAX_SIZE - 1].link;

        while (i) {
            j = i;
            i = SL[i].link;
        }

        SL[j].link = SL[0].link;
        SL[0].link = 0;
    }
    bool ListEmpty()const
    {
        return SL[0].link == 0;
    }
    int ListLength()const
    {
        int j = 0, i = SL[0].link;

        while (i) {
            i = SL[i].link;
            j++;
        }

        return j;
    }
    bool PriorElem(T e, bool(*eq)(T, T), T &pre_e)const
    {
        int j, i = SL[0].link;

        do {
            j = i;
            i = SL[i].link;
        } while (i && !eq(SL[i].data, e));

        if (i) {
            pre_e = SL[j].data;
            return true;
        }

        return false;
    }
    bool NextElem(T e, bool(*eq)(T, T), T &next_e)const
    {
        int i = SL[0].link;

        while (i) {
            if (eq(SL[i].data, e) && SL[i].link) {
                next_e = SL[SL[i].link].data;
                return true;
            }

            i = SL[i].link;
        }

        return false;
    }
    bool ListInsert(int i, T e)
    {
        int m, k = 0;

        for (m = 1; m < i; m++) {
            k = SL[k].link;

            if (k == 0)
                break;
        }

        if (m < i)
            return false;
        else {
            m = NEW();

            if (m) {
                SL[m].data = e;
                SL[m].link = SL[k].link;
                SL[k].link = m;
                return true;
            }

            return false;
        }
    }
    bool ListDelete(int i, T &e)
    {
        int m, k = 0;

        for (m = 1; m < i; m++) {
            k = SL[k].link;

            if (k == 0)
                break;
        }

        if (m < i || SL[k].link == 0)
            return false;
        else {
            m = SL[k].link;
            SL[k].link = SL[m].link;
            e = SL[m].data;
            DELETE(m);
            return true;
        }
    }
    void ListTraverse(void(*visit)(T&))
    {
        int i = SL[0].link;

        while (i) {
            visit(SL[i].data);
            i = SL[i].link;
        }

        cout << endl;
    }
};
#endif

