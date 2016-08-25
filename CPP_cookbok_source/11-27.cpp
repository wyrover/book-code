#include "kstride_iter.hpp"

#include <algorithm>
#include <iterator>
#include <iostream>

using namespace std;

int main()
{
    int a[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
    kstride_iter<int*, 2> first(a);
    kstride_iter<int*, 2> last(a + 8);
    copy(first, last, ostream_iterator<int>(cout, "\n"));
}