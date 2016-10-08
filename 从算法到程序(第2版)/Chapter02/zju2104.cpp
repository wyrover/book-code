#include <iostream>
#include <map>
#include <algorithm>
using namespace std;

int main()
{
    int n;

    while (cin >> n && n) {
        map <string, int> Balloon;
        string s;

        for (int i = 0; i < n; i++) {
            cin >> s;
            Balloon[s]++;
        }

        int iMax = 0;
        map <string, int>::iterator point, loc;

        for (point = Balloon.begin(); point != Balloon.end(); point++)
            if (iMax < point->second) {
                iMax = point->second;
                loc = point;
            }

        cout << loc->first << endl;
    }
}
