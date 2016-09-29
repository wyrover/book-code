//Algo7-10.cpp STL排序算法的应用1
#include "C.h"
typedef int KeyType;
#include "Func7-1.h"
bool cmp(ID a, ID b)
{
    return a.key < b.key;
}
void main()
{
    vector<ID> a;
    vector<ID>::iterator ip;
    ID e;
    ifstream fin("F7-1.txt");

    while (!fin.eof()) {
        if (InputFromFile(fin, e))
            a.push_back(e);
    }

    fin.close();
    cout << "排序前：" << endl;

    for (ip = a.begin(); ip != a.end(); ip++)
        Visit(*ip);

    sort(a.begin(), a.end(), cmp);
    cout << endl << "排序后：" << endl;

    for (ip = a.begin(); ip != a.end(); ip++)
        Visit(*ip);

    cout << endl;
}

