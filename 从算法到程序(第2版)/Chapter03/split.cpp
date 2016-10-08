#include <iostream>
using namespace std;

int split(int n, int m)
{
    if (n == 1 || m == 1) return 1;
    else if (n < m) return split(n, n);
    else if (n == m) return split(n, n - 1) + 1;
    else return split(n, m - 1) + split(n - m, m);
}

int main()
{
    int n;

    while (cin >> n)
        cout << split(n, n) << endl;

    return 0;
}
