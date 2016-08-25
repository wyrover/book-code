#include <cmath>
#include <iostream>

using namespace std;

template<class Iter_T, class Iter2_T>
double vectorDistance(Iter_T first, Iter_T last, Iter2_T first2)
{
    double ret = 0.0;

    while (first != last) {
        double dist = (*first++) - (*first2++);
        ret += dist * dist;
    }

    return ret > 0.0 ? sqrt(ret) : 0.0;
}

int main()
{
    int v1[] = { 1, 5 };
    int v2[] = { 4, 9 };
    cout << "distance between vectors (1,5) and (4,9) is ";
    cout << vectorDistance(v1, v1 + 2, v2) << endl;
}