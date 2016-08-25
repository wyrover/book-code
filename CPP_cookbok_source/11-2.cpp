#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;

int getMaxInt(vector<int>& v)
{
    return *max_element(v.begin(), v.end());
}

int getMinInt(vector<int>& v)
{
    return *min_element(v.begin(), v.end());
}

int main()
{
    vector<int> v;

    for (int i = 10; i < 20; ++i) v.push_back(i);

    cout << "min integer = " << getMinInt(v) << endl;
    cout << "max integer = " << getMaxInt(v) << endl;
}