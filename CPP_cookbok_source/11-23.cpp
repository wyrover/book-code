#include <numeric>
#include <cmath>
#include <iostream>
#include <functional>

using namespace std;

template<class Value_T>
struct DiffSquared {
    Value_T operator()(Value_T x, Value_T y) const
    {
        return (x - y) * (x - y);
    }
};

template<class Iter_T, class Iter2_T>
double vectorDistance(Iter_T first, Iter_T last, Iter2_T first2)
{
    double ret = inner_product(first, last, first2, 0.0L,
                               plus<double>(), DiffSquared<double>());
    return ret > 0.0 ? sqrt(ret) : 0.0;
}

int main()
{
    int v1[] = { 1, 5 };
    int v2[] = { 4, 9 };
    cout << "distance between vectors (1,5) and (4,9) is ";
    cout << vectorDistance(v1, v1 + 2, v2) << endl;
}