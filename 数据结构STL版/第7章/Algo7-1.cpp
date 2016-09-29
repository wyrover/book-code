//Algo7-1.cpp 验证插入排序类
#include "C.h"
#include "C4-1.h"
typedef int KeyType;
#include "InsSort.h"
#include "Func7-1.h"
void main()
{
    InsSort<ID> is[3];

    for (int i = 0; i < 3; i++) {
        is[i].Create_SeqFromFile("F7-1.txt");

        if (i == 0) {
            cout << "排序前：" << endl;
            is[i].Traverse(Visit);
        }

        switch (i) {
        case 0:
            is[i].InsertSort();
            cout << endl << "直接插入排序后：" << endl;
            break;

        case 1:
            is[i].BInsertSort();
            cout << endl << "折半插入排序后：" << endl;
            break;

        case 2:
            is[i].P2_InsertSort();
            cout << endl << "二路插入排序后：" << endl;
        }

        is[i].Traverse(Visit);
    }

    cout << endl;
}

