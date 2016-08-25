#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;

template<class Iter_T, class Value_T>
void computeMinAndMax(Iter_T first, Iter_T last, Value_T& min, Value_T& max)
{
    min = *min_element(first, last);
    max = *max_element(first, last);
}

int main()
{
    vector<int> v;

    for (int i = 10; i < 20; ++i) v.push_back(i);

    int min = -1;
    int max = -1;
    computeMinAndMax(v.begin(), v.end(), min, max);
    cout << "min integer = " << min << endl;
    cout << "max integer = " << max << endl;
}