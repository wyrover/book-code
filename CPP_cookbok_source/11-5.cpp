#include <numeric>
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    int sum = accumulate(v.begin(), v.end(), 0);
    double mean = double(sum) / v.size();
    cout << "sum = " << sum << endl;
    cout << "count = " << v.size() << endl;
    cout << "mean = " << mean << endl;
}