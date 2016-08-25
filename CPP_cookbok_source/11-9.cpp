#include <numeric>
#include <cmath>
#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>

using namespace std;

template<int N, class T>
T nthPower(T x)
{
    T ret = x;

    for (int i = 1; i < N; ++i) {
        ret *= x;
    }

    return ret;
}

template<class T, int N>
struct SumDiffNthPower {
    SumDiffNthPower(T x) : mean_(x) { };
    T operator()(T sum, T current)
    {
        return sum + nthPower<N>(current - mean_);
    }
    T mean_;
};

template<class T, int N, class Iter_T>
T nthMoment(Iter_T first, Iter_T last, T mean)
{
    size_t cnt = distance(first, last);
    return accumulate(first, last, T(), SumDiffNthPower<T, N>(mean)) / cnt;
}

template<class T, class Iter_T>
T computeVariance(Iter_T first, Iter_T last, T mean)
{
    return nthMoment<T, 2>(first, last, mean);
}

template<class T, class Iter_T>
T computeStdDev(Iter_T first, Iter_T last, T mean)
{
    return sqrt(computeVariance(first, last, mean));
}

template<class T, class Iter_T>
T computeSkew(Iter_T begin, Iter_T end, T mean)
{
    T m3 = nthMoment<T, 3>(begin, end, mean);
    T m2 = nthMoment<T, 2>(begin, end, mean);
    return m3 / (m2 * sqrt(m2));
}

template<class T, class Iter_T>
T computeKurtosisExcess(Iter_T begin, Iter_T end, T mean)
{
    T m4 = nthMoment<T, 4>(begin, end, mean);
    T m2 = nthMoment<T, 2>(begin, end, mean);
    return m4 / (m2 * m2) - 3;
}

template<class T, class Iter_T>
void computeStats(Iter_T first, Iter_T last, T& sum, T& mean,
                  T& var, T& std_dev, T& skew, T& kurt)
{
    size_t cnt = distance(first, last);
    sum = accumulate(first, last, T());
    mean = sum / cnt;
    var = computeVariance(first, last, mean);
    std_dev = sqrt(var);
    skew = computeSkew(first, last, mean);
    kurt = computeKurtosisExcess(first, last, mean);
}

int main()
{
    vector<int> v;
    v.push_back(2);
    v.push_back(4);
    v.push_back(8);
    v.push_back(10);
    v.push_back(99);
    v.push_back(1);
    double sum, mean, var, dev, skew, kurt;
    computeStats(v.begin(), v.end(), sum, mean, var, dev, skew, kurt);
    cout << "count = " << v.size() << "\n";
    cout << "sum = " << sum << "\n";
    cout << "mean = " << mean << "\n";
    cout << "variance = " << var << "\n";
    cout << "standard deviation = " << dev << "\n";
    cout << "skew = " << skew << "\n";
    cout << "kurtosis excess = " << kurt << "\n";
    cout << endl;
}