#include <algorithm>
#include <cassert>

template<class Value_T, unsigned int N>
class kvector
{
public:
    // public fields
    Value_T m[N];

    // public typedefs
    typedef Value_T value_type;
    typedef Value_T* iterator;
    typedef const Value_T* const_iterator;
    typedef Value_T& reference;
    typedef const Value_T& const_reference;
    typedef size_t size_type;

    // shorthand for referring to kvector
    typedef kvector self;

    // member functions
    template<typename Iter_T>
    void copy(Iter_T first, Iter_T last)
    {
        copy(first, last, begin());
    }
    iterator begin()
    {
        return m;
    }
    iterator end()
    {
        return m + N;
    }
    const_iterator begin() const
    {
        return m;
    }
    const_iterator end() const
    {
        return m + N;
    }
    reference operator[](size_type n)
    {
        return m[n];
    }
    const_reference operator[](size_type n) const
    {
        return m[n];
    }
    static size_type size()
    {
        return N;
    }

    // vector operations
    self& operator+=(const self& x)
    {
        for (int i = 0; i < N; ++i) m[i] += x.m[i];

        return *this;
    }
    self& operator-=(const self& x)
    {
        for (int i = 0; i < N; ++i) m[i] -= x.m[i];

        return *this;
    }

    // scalar operations
    self& operator=(value_type x)
    {
        std::fill(begin(), end(), x);
        return *this;
    }
    self& operator+=(value_type x)
    {
        for (int i = 0; i < N; ++i) m[i] += x;

        return *this;
    }
    self& operator-=(value_type x)
    {
        for (int i = 0; i < N; ++i) m[i] -= x;

        return *this;
    }
    self& operator*=(value_type x)
    {
        for (int i = 0; i < N;  ++i) m[i] *= x;

        return *this;
    }
    self& operator/=(value_type x)
    {
        for (int i = 0; i < N; ++i) m[i] /= x;

        return *this;
    }
    self& operator%=(value_type x)
    {
        for (int i = 0; i < N; ++i) m[i] %= x;

        return *this;
    }
    self operator-()
    {
        self x;

        for (int i = 0; i < N; ++i) x.m[i] = -m[i];

        return x;
    }

    // friend operators
    friend self operator+(self x, const self& y)
    {
        return x += y;
    }
    friend self operator-(self x, const self& y)
    {
        return x -= y;
    }
    friend self operator+(self x, value_type y)
    {
        return x += y;
    }
    friend self operator-(self x, value_type y)
    {
        return x -= y;
    }
    friend self operator*(self x, value_type y)
    {
        return x *= y;
    }
    friend self operator/(self x, value_type y)
    {
        return x /= y;
    }
    friend self operator%(self x, value_type y)
    {
        return x %= y;
    }
};