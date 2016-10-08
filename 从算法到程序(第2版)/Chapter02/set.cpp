#include <set>
#include <iostream>
using namespace std;

int main()
{
    set <string> str;
    set <string>::iterator pos;
    str.insert("apple");
    str.insert("orange");
    str.insert("banana");
    str.insert("grapes");

    for (pos = str.begin(); pos != str.end(); pos++)
        cout << *pos << " ";

    cout << endl;
    str.clear();
    return 0;
}
