#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

int main()
{
    vector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int value = 3;
    int cntLambdaCalled = 0;
    int cnt = count_if(cbegin(vec), cend(vec),
    [value, &cntLambdaCalled](int i) {
        ++cntLambdaCalled;
        return i > value;
    });
    cout << "The lambda expression was called " << cntLambdaCalled
         << " times." << endl;
    cout << "Found " << cnt << " values > " << value << endl;
    return 0;
}