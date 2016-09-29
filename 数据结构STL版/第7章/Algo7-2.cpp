//Algo7-2.cpp 冒泡排序类
#include "C.h"
#include "C4-1.h"
typedef int KeyType;
#include "SqTable.h"
template<typename D>class BubSort: public SqTable<D>
{
public:
    void BubbleSort(bool order = true, bool flag = true)
    {
        bool change = true;

        if (flag)
            for (int i = elem.size() - 1; i > 0 && change; i--) {
                change = false;

                for (int j = 0; j < i; j++)
                    if (order ? LT(elem[j + 1].key, elem[j].key) : LT(elem[j].key, elem[j + 1].key)) {
                        swap(elem[j], elem[j + 1]);
                        change = true;
                    }
            }
        else
            for (int i = 0; i < elem.size() - 1 && change; i++) {
                change = false;

                for (int j = elem.size() - 1; j > i; j--)
                    if (order ? LT(elem[j].key, elem[j - 1].key) : LT(elem[j - 1].key, elem[j].key)) {
                        swap(elem[j], elem[j - 1]);
                        change = true;
                    }
            }
    }
};
#include "Func7-1.h"
void main()
{
    BubSort<ID> bs[4];

    for (int i = 0; i < 4; i++)
        bs[i].Create_SeqFromFile("F7-1.txt");

    cout << "排序前：" << endl;
    bs[0].Traverse(Visit);
    bs[0].BubbleSort(true, true);
    cout << endl << "冒泡排序后(升序，最后一个数据最先到位)：" << endl;
    bs[0].Traverse(Visit);
    bs[1].BubbleSort(true, false);
    cout << endl << "冒泡排序后(升序，最前一个数据最先到位)：" << endl;
    bs[1].Traverse(Visit);
    bs[2].BubbleSort(false, true);
    cout << endl << "冒泡排序后(降序，最后一个数据最先到位)：" << endl;
    bs[2].Traverse(Visit);
    bs[3].BubbleSort(false, false);
    cout << endl << "冒泡排序后(降序，最前一个数据最先到位)：" << endl;
    bs[3].Traverse(Visit);
    cout << endl;
}

