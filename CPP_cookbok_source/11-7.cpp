#include <stdexcept>
#include <iostream>
#include <iterator>

using namespace std;

template<class Value_T, class Iter_T>
Value_T computeMean(Iter_T first, Iter_T last)
{
    if (first == last) throw domain_error("mean is undefined");

    Value_T sum;
    int cnt = 0;

    while (first != last) {
        sum += *first++;
        ++cnt;
    }

    return sum / cnt;
}

int main()
{
    cout << "please type in several integers separated by newlines" << endl;
    cout << "and terminated by an EOF character (i.e., Ctrl-Z)" << endl;
    double mean = computeMean<double>(
                      istream_iterator<int>(cin),
                      istream_iterator<int>());
    cout << "the mean is " << mean << endl;
}