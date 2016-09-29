//Algo7-5.cpp 快速排序类
#include "C.h"
#include "C4-1.h"
typedef int KeyType;
#include "SqTable.h"
template<typename D>class QkSort: public SqTable<D>
{
private:
    bool flag;
    int Partition(int low, int high)
    {
        int i = low, j = high;

        if (!flag) {
            int val = rand() % (high - low) + low;
            swap(elem[val], elem[low]);
            cout << endl << "pivotloc=" << val << " Partition(" << i << ", " << j << ")=" << endl;
        } else
            cout << endl << "pivotloc=" << low << " Partition(" << i << ", " << j << ")=" << endl;

        D temp = elem[low];

        while (low < high) {
            while (low < high && GE(elem[high].key, temp.key))
                high--;

            elem[low] = elem[high];

            while (low < high && LE(elem[low].key, temp.key))
                low++;

            elem[high] = elem[low];
        }

        elem[low] = temp;
        Traverse(Visit);
        return low;
    }
    void QSort(int low, int high)
    {
        if (low < high) {
            int pivotloc = Partition(low, high);
            QSort(low, pivotloc - 1);
            QSort(pivotloc + 1, high);
        }
    }
public:
    void QuickSort(bool bFlag = true)
    {
        flag = bFlag;
        QSort(0, elem.size() - 1);
    }
};
#include "Func7-1.h"
void main()
{
    QkSort<ID> qs[2];

    for (int i = 0; i < 2; i++) {
        qs[i].Create_SeqFromFile("F7-1.txt");

        if (i == 0) {
            cout << "排序前：" << endl;
            qs[i].Traverse(Visit);
        }

        qs[i].QuickSort(i == 0);
        cout << endl << "快速排序后：" << endl;
        qs[i].Traverse(Visit);
        cout << endl;
    }
}

