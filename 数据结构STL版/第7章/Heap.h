//Heap.h ∂—¿‡
#ifndef _HEAP_H_
#define _HEAP_H_
#include "SqTable.h"
template<typename D>class Heap: public SqTable<D>
{
protected:
    void HeapAdjustDown(int low, int high, bool flag)
    {
        D temp = elem[low];

        for (int j = 2 * low + 1; j <= high; j = j * 2 + 1) {
            if (flag) {
                if (j < high && LT(elem[j].key, elem[j + 1].key))
                    j++;

                if (GE(temp.key, elem[j].key))
                    break;
            } else {
                if (j < high && GT(elem[j].key, elem[j + 1].key))
                    j++;

                if (LE(temp.key, elem[j].key))
                    break;
            }

            elem[low] = elem[j];
            low = j;
        }

        elem[low] = temp;
    }
    void HeapAdjustUp(int high, bool flag)
    {
        D temp = elem[high];
        int i = 0;

        for (int j = high; j > 0; j = (j - 1) / 2) {
            i = (j - 1) / 2;

            if (flag)
                if GT(temp.key, elem[i].key)
                    swap(elem[j], elem[i]);
                else
                    break;
            else {
                if LT(temp.key, elem[i].key)
                    swap(elem[j], elem[i]);
                else
                    break;
            }
        }
    }
public:
    void HeapSort(bool flag = true)
    {
        int i;

        for (i = elem.size() / 2 - 1; i >= 0; i--)
            HeapAdjustDown(i, elem.size() - 1, flag);

        for (i = elem.size() - 1; i >= 1; i--) {
            swap(elem[0], elem[i]);
            HeapAdjustDown(0, i - 1, flag);
        }
    }
};
#endif

