#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

int main()
{
    vector<int> vec{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int value = 3;
    int cnt = count_if(cbegin(vec), cend(vec),
    [value](int i) {
        return i > value;
    });
    cout << "Found " << cnt << " values > " << value << endl;
    return 0;
}