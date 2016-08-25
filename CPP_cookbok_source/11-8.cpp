#include <algorithm>
#include <vector>
#include <iostream>
#include <iterator>

using namespace std;

struct OutOfRange {
    OutOfRange(int min, int max)
        : min_(min), max_(max)
    { }
    bool operator()(int x)
    {
        return (x < min_) || (x > max_);
    }
    int min_;
    int max_;
};

int main()
{
    vector<int> v;
    v.push_back(6);
    v.push_back(12);
    v.push_back(18);
    v.push_back(24);
    v.push_back(30);
    remove_copy_if(v.begin(), v.end(),
                   ostream_iterator<int>(cout, "\n"), OutOfRange(10, 25));
}