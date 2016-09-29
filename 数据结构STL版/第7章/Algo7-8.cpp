//Algo7-8.cpp 二路归并排序类
#include "C.h"
#include "C4-1.h"
typedef int KeyType;
#include "SqTable.h"
template<typename D>class MerSort: public SqTable<D>
{
private:
    vector<D> temp;
    void Merge(int low, int m, int high)
    {
        int i = low, j = m + 1, k = low, p;

        for (; i <= m && j <= high; k++)
            if LE(elem[i].key, elem[j].key)
                temp[k] = elem[i++];
            else
                temp[k] = elem[j++];

        if (i <= m)
            for (p = 0; p <= m - i; p++)
                temp[k + p] = elem[i + p];

        if (j <= high)
            for (p = 0; p <= high - j; p++)
                temp[k + p] = elem[j + p];

        for (p = low; p <= high; p++)
            elem[p] = temp[p];

        cout << endl << "Merge(" << low << ", " << m << ", " << high << ")后：" << endl;
        Traverse(Visit);
    }
    void MSort(int low, int high)
    {
        if (low < high) {
            int m = (low + high) / 2;
            MSort(low, m);
            MSort(m + 1, high);
            Merge(low, m, high);
        }
    }
public:
    void MergeSort()
    {
        temp.resize(elem.size());
        MSort(0, elem.size() - 1);
    }
};
#include "Func7-1.h"
void main()
{
    MerSort<ID> ms;
    ms.Create_SeqFromFile("F7-1.txt");
    cout << "排序前：" << endl;
    ms.Traverse(Visit);
    ms.MergeSort();
    cout << endl << "二路归并排序后：" << endl;
    ms.Traverse(Visit);
    cout << endl;
}

