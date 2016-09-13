#include <vector>
#include <numeric>
#include <iostream>
using namespace std;

int main()
{
    vector<int> vec(10);
    iota(begin(vec), end(vec), 5);

    for (auto& i : vec) {
        cout << i << " ";
    }

    cout << endl;
    return 0;
}