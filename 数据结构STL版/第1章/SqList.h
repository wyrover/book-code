//SqList.h 顺序表的类（SqList类）
#ifndef _SQLIST_H_
#define _SQLIST_H_
template<typename T>class SqList
{
    friend void MergeList(const SqList<T>&, const SqList<T>&, SqList<T>&);

private:
    T *elem;
    int length;
    int listsize;
public:
    SqList(int k = 1)
    {
        elem = new T[k];
        assert(elem != NULL);
        length = 0;
        listsize = k;
    }
    SqList(const SqList &L)
    {
        length = L.length;
        listsize = L.listsize;
        elem = new T[listsize];
        assert(elem != NULL);

        for (int i = 0; i < length; i++)
            elem[i] = L.elem[i];
    }
    SqList& operator=(const SqList &L)
    {
        if (this != &L) {
            length = L.length;
            listsize = L.listsize;

            if (elem != NULL)
                delete elem;

            elem = new T[listsize];
            assert(elem != NULL);

            for (int i = 0; i < length; i++)
                elem[i] = L.elem[i];
        }

        return *this;
    }
    ~SqList()
    {
        delete[]elem;
    }
    void ClearList()
    {
        length = 0;
    }
    bool ListEmpty()const
    {
        return length == 0;
    }
    int ListLength()const
    {
        return length;
    }
    bool GetElem(int i, T &e)const
    {
        if (i < 0 || i >= length)
            return false;

        e = *(elem + i);
        return true;
    }
    int LocateElem(T e, bool(*eq)(T, T))const
    {
        int i = 0;

        while (i < length && !eq(*(elem + i), e))
            i++;

        if (i < length)
            return i;
        else
            return -1;
    }
    bool PriorElem(T e, bool(*eq)(T, T), T &pre_e)const
    {
        int i = LocateElem(e, eq);

        if (i <= 0)
            return false;
        else {
            pre_e = *(elem + i - 1);
            return true;
        }
    }
    bool NextElem(T e, bool(*eq)(T, T), T &next_e)const
    {
        int i = LocateElem(e, eq);

        if (i == -1 || i == length - 1)
            return false;
        else {
            next_e = *(elem + i + 1);
            return true;
        }
    }
    bool ListInsert(int i, T e)
    {
        T *newbase, *q, *p;

        if (i < 0 || i > length)
            return false;

        if (length == listsize) {
            newbase = new T[listsize * 2];
            assert(newbase != NULL);

            for (int j = 0; j < length; j++)
                *(newbase + j) = *(elem + j);

            delete[]elem;
            elem = newbase;
            listsize *= 2;
        }

        q = elem + i;

        for (p = elem + length - 1; p >= q; p--)
            *(p + 1) = *p;

        *q = e;
        length++;
        return true;
    }
    bool ListDelete(int i, T &e)
    {
        T *p, *q;

        if (i < 0 || i >= length)
            return false;

        p = elem + i;
        e = *p;
        q = elem + length - 1;

        for (p++; p <= q; p++)
            *(p - 1) = *p;

        length--;

        if (length <= listsize / 4) {
            p = new T[listsize / 2];

            for (int j = 0; j < length; j++)
                *(p + j) = *(elem + j);

            delete[]elem;
            elem = p;
            listsize /= 2;
        }

        return true;
    }
    void ListTraverse(void(*visit)(T&))const;
};
template<typename T>
void SqList<T>::ListTraverse(void(*visit)(T&))const
{
    for (int i = 0; i < length; i++)
        visit(elem[i]);

    cout << endl;
}
#endif

