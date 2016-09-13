#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>
using namespace std;

int main()
{
    vector<int> vec(10);
    iota(vec.begin(), vec.end(), 5);

    for (auto& i : vec) {
        cout << i << " ";
    }

    cout << endl;
    return 0;
}