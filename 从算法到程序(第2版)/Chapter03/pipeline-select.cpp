#include <iostream>
#include <cmath>
using namespace std;

#define NUM 1001
int a[NUM];
int select(int left, int right, int k)
{
    if (left >= right) return a[left];

    int i = left;
    int j = right + 1;
    int pivot = a[left];

    while (true) {
        do {
            i = i + 1;
        } while (a[i] < pivot);

        do {
            j = j - 1;
        } while (a[j] > pivot);

        if (i >= j) break;

        swap(a[i], a[j]);
    }

    if (j - left + 1 == k) return pivot;

    a[left] = a[j];
    a[j] = pivot;

    if (j - left + 1 < k)
        return select(j + 1, right, k - j + left - 1);
    else return select(left, j - 1, k);
}

int main()
{
    int n;
    int x;
    int y;
    cin >> n;

    for (int i = 0; i < n; i++)
        cin >> x >> a[i];

    y = select(0, n - 1, n / 2);
    int min = 0;

    for (int i = 0; i < n; i++)
        min += (int)fabs(a[i] - y);

    cout << min << endl;
    return 0;
}
