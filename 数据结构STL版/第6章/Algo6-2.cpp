//Algo6-2.cpp 验证HashTable类的成员函数
#include "C.h"
#include "C4-1.h"
const int EMPTY = 0;
const int TOMB = -1;
typedef int KeyType;
#include "HashTable.h"
#include "Func6-2.h"
void main()
{
    HashTable<HD> h;
    int n, p = 0;
    bool j;
    HD e;
    KeyType k;
    h.CreateHashFromFile("F6-3.txt");
    cout << "按哈希地址的顺序遍历哈希表：" << endl;
    h.TraverseHash(Visit);
    cout << "请输入待查找数据的关键字：";
    InputKey(k);
    j = h.SearchHash(k, p, n);

    if (j)
        Visit(h.GetElem(p));
    else
        cout << "未找到" << endl;

    if (h.type == 1) {
        cout << "请输入待删除数据的关键字：";
        InputKey(k);
        j = h.DeleteHash(k, e);

        if (j) {
            cout << "成功删除数据";
            Visit(e);
        }

        cout << "插入数据，请输入待插入数据的key order：";
        Input(e);
        h.InsertHash(e);
        cout << "按哈希地址的顺序遍历哈希表：" << endl;
        h.TraverseHash(Visit);
    }
}

