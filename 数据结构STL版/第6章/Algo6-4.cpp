//Algo6-4.cpp 验证DLTree类的成员函数
#include "C.h"
const char Empty = '$';
//（此时关键字中不可包括'$'且'$'要比所有可能的字符的ASCII码小）
typedef string KeyType;
#include "DLTree.h"
#include "Func6-3.h"
void main()
{
    int i;
    string key;
    DLTree<Record> t;
    Record r;
    t.CreateFromFile("F6-5.txt");
    t.TraverseDLTree(t.Root(), Out);
    cout << endl;
    t.Print(Out);
    cout << "请输入待查找的关键字符串：";
    InputKey(key);
    i = t.SearchDLTree(key);

    if (i > -1)
        t.OutRecord(Out, i);

    for (i = 0; i < 3; i++) {
        cout << endl << "请输入待删除的关键字符串：";
        InputKey(key);

        if (t.DeleteDLTree(key, r)) {
            cout << "删除成功！其值为";
            Out(r);
        } else
            cout << "删除失败！";
    }

    cout << endl;
    t.TraverseDLTree(t.Root(), Out);
    cout << endl;
    t.Print(Out);
}

