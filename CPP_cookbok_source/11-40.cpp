#include <iostream>

using namespace std;

template<int E>
struct BasicFixedReal {
    typedef BasicFixedReal self;
    static const int factor = 1 << (E - 1);
    BasicFixedReal() : m(0) { }
    BasicFixedReal(double d) : m(static_cast<int>(d * factor)) { }
    self& operator+=(const self& x)
    {
        m += x.m;
        return *this;
    }
    self& operator-=(const self& x)
    {
        m -= x.m;
        return *this;
    }
    self& operator*=(const self& x)
    {
        m *= x.m;
        m >>= E;
        return *this;
    }
    self& operator/=(const self& x)
    {
        m /= x.m;
        m *= factor;
        return *this;
    }
    self& operator*=(int x)
    {
        m *= x;
        return *this;
    }
    self& operator/=(int x)
    {
        m /= x;
        return *this;
    }
    self operator-()
    {
        return self(-m);
    }
    double toDouble() const
    {
        return double(m) / factor;
    }

    // friend functions
    friend self operator+(self x, const self& y)
    {
        return x += y;
    }
    friend self operator-(self x, const self& y)
    {
        return x -= y;
    }
    friend self operator*(self x, const self& y)
    {
        return x *= y;
    }
    friend self operator/(self x, const self& y)
    {
        return x /= y;
    }

    // comparison operators
    friend bool operator==(const self& x, const self& y)
    {
        return x.m == y.m;
    }
    friend bool operator!=(const self& x, const self& y)
    {
        return x.m != y.m;
    }
    friend bool operator>(const self& x, const self& y)
    {
        return x.m > y.m;
    }
    friend bool operator<(const self& x, const self& y)
    {
        return x.m < y.m;
    }
    friend bool operator>=(const self& x, const self& y)
    {
        return x.m >= y.m;
    }
    friend bool operator<=(const self& x, const self& y)
    {
        return x.m <= y.m;
    }
private:
    int m;
};

typedef BasicFixedReal<10> FixedReal;

int main()
{
    FixedReal x(0);

    for (int i = 0; i < 100; ++i) {
        x += FixedReal(0.0625);
    }

    cout << x.toDouble() << endl;
}