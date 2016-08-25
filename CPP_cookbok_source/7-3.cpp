#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "utils.h" // For printContainer( ): see 7.10

using namespace std;

int main()
{
    vector<int> v;
    back_insert_iterator<std::vector<int> > p =
        back_inserter(v);

    for (int i = 0; i < 10; ++i)
        *p = i;

    printContainer(v, true);
    random_shuffle(v.begin(), v.end());
    printContainer(v, true);
}
