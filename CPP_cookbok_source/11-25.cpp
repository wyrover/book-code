#include "stride_iter.hpp"

#include <algorithm>
#include <iterator>
#include <iostream>

using namespace std;

int main()
{
    int a[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
    stride_iter<int*> first(a, 2);
    stride_iter<int*> last(a + 8, 2);
    copy(first, last, ostream_iterator<int>(cout, "\n"));
}