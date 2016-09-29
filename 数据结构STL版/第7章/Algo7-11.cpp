//Algo7-11.cpp STL排序算法的应用2
#include "C.h"
#include "Func7-2.h"
bool cmp(Rec a, Rec b)
{
    return a.key < b.key;
}
void main()
{
    vector<Rec> a;
    vector<Rec>::iterator ip;
    Rec e;
    ifstream fin("F7-2.txt");

    while (!fin.eof()) {
        if (InputFromFile(fin, e))
            a.push_back(e);
    }

    fin.close();
    cout << "排序前：" << endl;

    for (ip = a.begin(); ip != a.end(); ip++)
        Visit(*ip);

    sort(a.begin(), a.end(), cmp);
    cout << "排序后：" << endl;

    for (ip = a.begin(); ip != a.end(); ip++)
        Visit(*ip);

    cout << endl;
}

