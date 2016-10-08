#include <iostream>
using namespace std;

string a;
int k;

int main()
{
    cin >> a >> k;

    if (k >= a.size()) a.erase();
    else while (k > 0) {
            int i;

            for (i = 0; (i < a.size() - 1) && (a[i] <= a[i + 1]); ++i);

            a.erase(i, 1);
            k--;
        }

    while (a.size() > 1 && a[0] == '0')
        a.erase(0, 1);

    cout << a << endl;
    return 0;
}
