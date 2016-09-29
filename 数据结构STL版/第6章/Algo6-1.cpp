//Algo6-1.cpp 验证SqTable类的成员函数
#include "C.h"
#include "C4-1.h"
typedef long KeyType;
#include "SqTable.h"
#include "Func6-1.h"
KeyType key;
bool cmp(Sc e)
{
    return e.number == key;
}
void main()
{
    int j, k;
    SqTable<Sc> st[2];
    Sc e;
    string s = "F6-1.txt";

    for (int i = 0; i < 2; i++) {
        s[3] = i + '1';
        st[i].Create_SeqFromFile(s);
        cout << "准考证号  姓名  语文 外语 数学 物理 总分" << endl;
        st[i].Traverse(Total);
        st[i].Traverse(Visit);
        cout << "请输入待查找人的考号：";
        InputKey(key);

        if (i == 0) {
            cout << "请输入无序向量查找方法的序号（0～2）：";
            cin >> k;
            j = st[i].Search_Seq(key, k);
        } else
            j = st[i].Search_Bin(key);

//          j=st[i].Search_Bin1(key);//在st[i]中折半查找含有关键字key项的序号（高效）
        if (j >= 0) {
            st[i].GetElem(j, e);
            Visit(e);
        }
    }
}

