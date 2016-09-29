//LinkList.h 单链表的类（LinkList类）
#ifndef _LINKLIST_H_
#define _LINKLIST_H_
#include "LNode.h"
template<typename T>class LinkList
{
    friend void MergeList(LinkList<T>&, LinkList<T>&);
protected:
    LNode<T> *Head;
public:
    LinkList()
    {
        Head = new LNode<T>;
        assert(Head != NULL);
        Head->next = NULL;
    }
    ~LinkList()
    {
        ClearList();
        delete Head;
    }
    void ClearList()const
    {
        LNode<T> *q, *p = Head->next;
        Head->next = NULL;

        while (p != NULL) {
            q = p->next;
            delete p;
            p = q;
        }
    }
    bool ListEmpty()const
    {
        return Head->next == NULL;
    }
    int ListLength()const
    {
        int i = 0;
        LNode<T> *p = Head->next;

        while (p != NULL) {
            i++;
            p = p->next;
        }

        return i;
    }
    bool GetElem(int i, T &e)const
    {
        int j = 1;
        LNode<T> *p = Head->next;

        while (p != NULL && j < i) {
            j++;
            p = p->next;
        }

        if (p == NULL || j > i)
            return false;

        e = p->data;
        return true;
    }
    int LocateElem(T e, bool(*eq)(T, T))const
    {
        int i = 0;
        LNode<T> *p = Head->next;

        while (p != NULL) {
            i++;

            if (eq(p->data, e))
                return i;

            p = p->next;
        }

        return 0;
    }
    bool PriorElem(T e, bool(*eq)(T, T), T &pre_e)const
    {
        LNode<T> *q, *p = Head->next;

        while (p != NULL && p->next != NULL) {
            q = p->next;

            if (eq(q->data, e)) {
                pre_e = p->data;
                return true;
            }

            p = q;
        }

        return false;
    }
    bool NextElem(T e, bool(*eq)(T, T), T &next_e)const
    {
        LNode<T> *p = Head->next;

        while (p != NULL && p->next != NULL) {
            if (eq(p->data, e)) {
                next_e = p->next->data;
                return true;
            }

            p = p->next;
        }

        return false;
    }
    bool ListInsert(int i, T e)
    {
        int j = 0;
        LNode<T> *s, *p = Head;

        while (p != NULL && j < i - 1) {
            j++;
            p = p->next;
        }

        if (p == NULL || j > i - 1)
            return false;

        s = new LNode<T>;

        if (s == NULL)
            return false;

        s->data = e;
        s->next = p->next;
        p->next = s;
        return true;
    }
    bool ListDelete(int i, T &e)const
    {
        int j = 0;
        LNode<T> *q, *p = Head;

        while (p->next != NULL && j < i - 1) {
            j++;
            p = p->next;
        }

        if (p->next == NULL || j > i - 1)
            return false;

        q = p->next;
        p->next = q->next;
        e = q->data;
        delete q;
        return true;
    }
    void ListTraverse(void(*visit)(T&))const
    {
        LNode<T> *p = Head->next;

        while (p != NULL) {
            visit(p->data);
            p = p->next;
        }

        cout << endl;
    }
};
#endif

