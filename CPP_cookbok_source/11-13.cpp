#include <algorithm>
#include <vector>
#include <iterator>
#include <iostream>
#include <cstdlib>

using namespace std;

struct RndIntGen {
    RndIntGen(int l, int h)
        : low(l), high(h)
    { }
    int operator()() const
    {
        return low + (rand() % ((high - low) + 1));
    }
private:
    int low;
    int high;
};

int main()
{
    srand(static_cast<unsigned int>(clock()));
    vector<int> v(5);
    generate(v.begin(), v.end(), RndIntGen(1, 6));
    copy(v.begin(), v.end(), ostream_iterator<int>(cout, "\n"));
}