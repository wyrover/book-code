//Main1-2.cpp T为用户自定义类型的实例
#include "C.h"
#include "SqList.h"
#include "Func1-2.h"
void main()
{
    bool i;
    int j = 0;
    SqList<Student>L(5);
    ifstream fin("F1-1.txt");
    Student e, e0;

    while (!fin.eof()) {
        if (InputFromFile(fin, e))
            L.ListInsert(j++, e);
    }

    fin.close();
    cout << "在L的表尾依次插入文件F1-1.txt中的数据后，表L=";
    L.ListTraverse(printSt);
    cout << "L是否空？" << boolalpha << L.ListEmpty() << "，表长=";
    cout << L.ListLength() << endl;
    L.GetElem(3, e);
    cout << "序号为3的元素其值为";
    printSt(e);
    cout << endl;
    i = L.PriorElem(e, equal, e0);

    if (i) {
        printName(e);
        cout << "的前驱为";
        printName(e0);
        cout << endl;
    }

    i = L.NextElem(e, equal, e0);

    if (i) {
        printName(e);
        cout << "的后继为";
        printName(e0);
        cout << endl;
    }

    i = L.ListDelete(3, e);

    if (i) {
        cout << "删除序号为3的元素成功，其值为";
        printSt(e);
    }

    cout << "\n目前，表L=";
    L.ListTraverse(printSt);
    L.ClearList();
    cout << "清空L后，L是否空？" << boolalpha << L.ListEmpty();
    cout << "，表长=" << L.ListLength() << endl;
}

