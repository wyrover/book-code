#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>

using namespace std;

template<class Seq_T>
struct comma_helper {
    typedef typename Seq_T::value_type value_type;
    explicit comma_helper(Seq_T& x) : m(x) { }
    comma_helper& operator=(const value_type& x)
    {
        m.clear();
        return operator+=(x);
    }
    comma_helper& operator+=(const value_type& x)
    {
        m.push_back(x);
        return *this;
    }
    Seq_T& m;
};

template<typename Seq_T>
comma_helper<Seq_T>
initialize(Seq_T& x)
{
    return comma_helper<Seq_T>(x);
}

template<class Seq_T, class Scalar_T>
comma_helper<Seq_T>&
operator, (comma_helper<Seq_T>& h, Scalar_T x)
{
    h += x;
    return h;
}

int main()
{
    vector v;
    int a = 2;
    int b = 5;
    initialize(v) = 0, 1, 1, a, 3, b, 8, 13;
    cout << v[3] << endl; // outputs 2
    system("pause");
    return EXIT_SUCCESS;
}