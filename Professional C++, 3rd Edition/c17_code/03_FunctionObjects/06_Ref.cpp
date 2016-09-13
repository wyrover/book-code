#include <algorithm>
#include <vector>
#include <iostream>
#include <functional>
using namespace std;

void increment(int& value)
{
    ++value;
}

int main()
{
    int index = 0;
    increment(index);
    auto incr1 = bind(increment, index);
    incr1();
    auto incr2 = bind(increment, ref(index));
    incr2();
    return 0;
}
