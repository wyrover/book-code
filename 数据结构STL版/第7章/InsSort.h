//InsSort.h ≤Â»Î≈≈–Ú¿‡
#ifndef _INSSORT_H_
#define _INSSORT_H_
#include "SqTable.h"
template<typename D>class InsSort: public SqTable<D>
{
public:
    void InsertSort()
    {
        D temp;

        for (int i = 1; i < elem.size(); i++)
            if LT(elem[i].key, elem[i - 1].key) {
                temp = elem[i];

                for (int j = i - 1; LT(temp.key, elem[j].key) && j >= 0; j--)
                    elem[j + 1] = elem[j];

                elem[j + 1] = temp;
            }
    }
    void BInsertSort()
    {
        int m, low, high;
        D temp;

        for (int i = 1; i < elem.size(); i++)
            if LT(elem[i].key, elem[i - 1].key) {
                temp = elem[i];
                low = 0;
                high = i - 1;

                while (low <= high) {
                    m = (low + high) / 2;

                    if LT(temp.key, elem[m].key)
                        high = m - 1;
                    else
                        low = m + 1;
                }

                for (int j = i - 1; j >= high + 1; j--)
                    elem[j + 1] = elem[j];

                elem[high + 1] = temp;
            }
    }
    void P2_InsertSort()
    {
        int i, j, first, final, mid;
        vector<D> d;
        d.resize(elem.size());
        d[0] = elem[0];
        first = final = 0;

        for (i = 1; i < elem.size(); i++) {
            if (first > final)
                j = elem.size();
            else
                j = 0;

            mid = (first + final + j) / 2 % elem.size();

            if LT(elem[i].key, d[mid].key) {
                j = first;
                first = (first - 1 + elem.size()) % elem.size();

                while GT(elem[i].key, d[j].key) {
                    d[(j - 1 + elem.size()) % elem.size()] = d[j];
                    j = (j + 1) % elem.size();
                }

                d[(j - 1 + elem.size()) % elem.size()] = elem[i];
            } else {
                j = final++;

                while LT(elem[i].key, d[j].key) {
                    d[(j + 1) % elem.size()] = d[j];
                    j = (j - 1 + elem.size()) % elem.size();
                }

                d[(j + 1) % elem.size()] = elem[i];
            }
        }

        for (i = 0; i < elem.size(); i++)
            elem[i] = d[(first + i) % elem.size()];
    }
};
#endif

