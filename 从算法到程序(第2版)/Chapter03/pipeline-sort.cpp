#include<iostream>
#include<cmath>
#include<algorithm>
using namespace std;

int main()
{
    int n;
    int x;
    int a[1000];
    cin >> n;

    for (int k = 0; k < n; k++)
        cin >> x >> a[k];

    sort(a, a + n);
    int min = 0;

    for (int i = 0; i < n; i++)
        min += (int)fabs(a[i] - a[n / 2]);

    cout << min << endl;
}
