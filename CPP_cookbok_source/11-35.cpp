#include <complex>
#include <iostream>

using namespace std;

template<class T>
struct BasicPolar {
public:
    typedef BasicPolar self;

    // constructors
    BasicPolar() : m() {  }
    BasicPolar(const self& x) : m(x.m) {  }
    BasicPolar(const T& rho, const T& theta) : m(polar(rho, theta)) { }

    // assignment operations
    self operator-()
    {
        return Polar(-m);
    }
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
        return *this;
    }
    self& operator/=(const self& x)
    {
        m /= x.m;
        return *this;
    }
    operator complex<T>() const
    {
        return m;
    }

    // public member functions
    T rho() const
    {
        return abs(m);
    }
    T theta() const
    {
        return arg(m);
    }

    // binary operations
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
private:
    complex<T> m;
};

typedef BasicPolar<double> Polar;

int main()
{
    double rho = 3.0; // magnitude
    double theta = 3.141592 / 2; // angle
    Polar coord(rho, theta);
    cout << "rho = " << coord.rho() << ", theta = " << coord.theta() << endl;
    coord += Polar(4.0, 0.0);
    cout << "rho = " << coord.rho() << ", theta = " << coord.theta() << endl;
    system("pause");
}