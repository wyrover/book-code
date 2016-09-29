//Algo6-5.cpp 验证TrieTree类的成员函数
#include "C.h"
const char Empty = '$';
typedef string KeyType;
#include "TrieTree.h"
#include "Func6-3.h"
void main()
{
    TrieTree<Record> t;
    int i;
    string key;
    Record r;
    t.CreateFromFile("F6-5.txt");
    t.TraverseTrie(t.Root(), Out);
    cout << endl;
    t.Print(Out);
    cout << "请输入待查找的关键字符串：";
    InputKey(key);
    i = t.SearchTrie(key);

    if (i != -1)
        t.OutRecord(Out, i);
    else
        cout << "不存在关键字为" << key << "的数据";

    for (i = 0; i < 3; i++) {
        cout << endl << "请输入待删除的关键字符串：";
        InputKey(key);

        if (t.DeleteTrie(key, r)) {
            cout << "删除成功！其值为";
            Out(r);
        } else
            cout << "删除失败！";
    }

    r.key = "LIN";
    r.order = 17;
    t.InsertTrie(r);
    cout << endl << "按关键字符串的顺序遍历删除插入后的树t：" << endl;
    t.TraverseTrie(t.Root(), Out);
    cout << endl;
    t.Print(Out);
}

