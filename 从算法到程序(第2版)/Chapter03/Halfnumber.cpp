#include<iostream>
using namespace std;

int a[1001];
int comp(int n)
{
    int ans = 1;

    if (a[n] > 0)return a[n];

    for (int i = 1; i <= n / 2; i++)
        ans += comp(i);

    a[n] = ans;
    return ans;
}

int main()
{
    int n;

    while (cin >> n) {
        memset(a, 0, sizeof(a));
        a[1] = 1;
        cout << comp(n) << endl;
    }

    return 0;
}
