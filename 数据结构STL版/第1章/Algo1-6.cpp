//Algo1-6.cpp 在STL的list中使用自定义数据类型排序
#include "C.h"
#include "Func1-2.h"
//以下两个函数在注释掉Func1-2.h中的struct Student内定义的<、>运算符后可用
/*bool operator <(const Student &a, const Student &b)
{
    return a.score<b.score;
}
bool operator >(const Student &a, const Student &b)
{
    return a.score>b.score;
}*/
void main()
{
    list<Student> L;
    list<Student>::const_iterator it;
    ifstream fin("F1-1.txt");
    Student e;

    while (!fin.eof())
        if (InputFromFile(fin, e))
            L.push_back(e);

    fin.close();
    cout << "在L的表尾依次插入文件F1-1.txt中的数据后，表L=" << endl;

    for (it = L.begin(); it != L.end(); it++)
        cout << it->name << ' ' << it->score << endl;

    L.sort();
//  L.sort(greater<Student>());//降序
    cout << "按分数排序后，表L=" << endl;

    for (it = L.begin(); it != L.end(); it++)
        cout << it->name << ' ' << it->score << endl;
}

