//DLinkList.h 双向链表的类（DLinkList类）
#ifndef _DLINKLIST_H_
#define _DLINKLIST_H_
#include "DLNode.h"
template<typename T>class DLinkList
{
private:
    DLNode<T> *Head;
    DLNode<T>* GetElemP(int i)const
    {
        int j = 0;
        DLNode<T> *p = Head;

        if (i < 0)
            return NULL;

        if (i == 0)
            return p;

        do {
            p = p->next;
            j++;
        } while (p != Head && j < i);

        if (p == Head)
            return NULL;
        else
            return p;
    }
    DLNode<T>* GetElemE(T e, bool(*eq)(T, T))const
    {
        DLNode<T> *p = Head->next;

        while (p != Head && !eq(p->data, e))
            p = p->next;

        if (p == Head)
            return NULL;
        else
            return p;
    }
public:
    DLinkList()
    {
        Head = new DLNode<T>;
        assert(Head != NULL);
        Head->next = Head->prior = Head;
    }
    ~DLinkList()
    {
        ClearList();
        delete Head;
    }
    void ClearList()const
    {
        DLNode<T> *p = Head->next;

        while (p != Head) {
            p = p->next;
            delete p->prior;
        }

        Head->next = Head->prior = Head;
    }
    bool ListEmpty()const
    {
        return Head->next == Head;
    }
    int ListLength()const
    {
        int i = 0;
        DLNode<T> *p = Head->next;

        while (p != Head) {
            i++;
            p = p->next;
        }

        return i;
    }
    bool GetElem(int i, T &e)const
    {
        DLNode<T> *p = GetElemP(i);

        if (p != NULL && i > 0) {
            e = p->data;
            return true;
        } else
            return false;
    }
    int LocateElem(T e, bool(*eq)(T, T))const
    {
        int i = 0;
        DLNode<T> *p = Head->next;

        while (p != Head) {
            i++;

            if (eq(p->data, e))
                return i;

            p = p->next;
        }

        return 0;
    }
    bool PriorElem(T e, bool(*eq)(T, T), T &pre_e)const
    {
        DLNode<T> *p = GetElemE(e, eq);

        if (p != NULL && p->prior != Head) {
            pre_e = p->prior->data;
            return true;
        }

        return false;
    }
    bool NextElem(T e, bool(*eq)(T, T), T &next_e)const
    {
        DLNode<T> *p = GetElemE(e, eq);

        if (p != NULL && p->next != Head) {
            next_e = p->next->data;
            return true;
        }

        return false;
    }
    bool ListInsert(int i, T e)
    {
        DLNode<T> *s, *p = GetElemP(i - 1);

        if (p == NULL)
            return false;

        s = new DLNode<T>;

        if (s == NULL)
            return false;

        s->data = e;
        s->prior = p;
        s->next = p->next;
        p->next->prior = s;
        p->next = s;
        return true;
    }
    bool ListDelete(int i, T &e)const
    {
        DLNode<T> *p = GetElemP(i);

        if (i <= 0 || p == NULL)
            return false;

        e = p->data;
        p->prior->next = p->next;
        p->next->prior = p->prior;
        delete p;
        return true;
    }
    void ListTraverse(void(*visit)(T&))const
    {
        DLNode<T> *p = Head->next;

        while (p != Head) {
            visit(p->data);
            p = p->next;
        }

        cout << endl;
    }
    void ListBackTraverse(void(*visit)(T&))const
    {
        DLNode<T> *p = Head->prior;

        while (p != Head) {
            visit(p->data);
            p = p->prior;
        }

        cout << endl;
    }
};
#endif

