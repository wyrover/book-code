//Algo7-4.cpp œ£∂˚≈≈–Ú¿‡
#include "C.h"
#include "C4-1.h"
typedef int KeyType;
#include "SqTable.h"
template<typename D>class SheSort: public SqTable<D>
{
private:
    vector<int> dt;
    void ShellInsert(int dk)
    {
        int j;
        D temp;

        for (int i = dk; i < elem.size(); i++)
            if LT(elem[i].key, elem[i - dk].key) {
                temp = elem[i];

                for (j = i - dk; j >= 0 && LT(temp.key, elem[j].key); j -= dk)
                    elem[j + dk] = elem[j];

                elem[j + dk] = temp;
            }
    }
public:
    SheSort(int* DT, int N)
    {
        for (int i = 0; i < N; i++)
            dt.push_back(DT[i]);

        assert(dt[N - 1] == 1);
    }
    void ShellSort()
    {
        for (int k = 0; k < dt.size(); k++) {
            ShellInsert(dt[k]);
            cout << endl << "dt[" << k << "]=" << dt[k] << "£¨µ⁄" << k + 1 << "ÃÀ≈≈–ÚΩ·π˚£®Ωˆ ‰≥ˆπÿº¸◊÷£©£∫";
            Traverse(PrintKey);
        }
    }
};
#include "Func7-1.h"
void main()
{
    int DT[3] = {5, 3, 1};
    SheSort<ID> shs(DT, 3);
    shs.Create_SeqFromFile("F7-1.txt");
    cout << "≈≈–Ú«∞£∫" << endl;
    shs.Traverse(Visit);
    shs.ShellSort();
    cout << endl << "œ£∂˚≈≈–Ú∫Û£∫" << endl;
    shs.Traverse(Visit);
    cout << endl;
}

