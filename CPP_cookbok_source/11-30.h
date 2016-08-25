#ifndef KMATRIX_HPP
#define KMATRIX_HPP

#include "kvector.hpp"
#include "kstride_iter.hpp"

template<class Value_T, int Rows_N, int Cols_N>
class kmatrix
{
public:
    // public typedefs
    typedef Value_T value_type;
    typedef kmatrix self;
    typedef Value_T* iterator;
    typedef const Value_T* const_iterator;
    typedef kstride_iter<Value_T*, 1> row_type;
    typedef kstride_iter<Value_T*, Cols_N> col_type;
    typedef kstride_iter<const Value_T*, 1> const_row_type;
    typedef kstride_iter<const Value_T*, Cols_N> const_col_type;

    // public constants
    static const int nRows = Rows_N;
    static const int nCols = Cols_N;

    // constructors
    kmatrix()
    {
        m = Value_T();
    }
    kmatrix(const self& x)
    {
        m = x.m;
    }
    explicit kmatrix(Value_T& x)
    {
        m = x.m;
    }

    // public functions
    static int rows()
    {
        return Rows_N;
    }
    static int cols()
    {
        return Cols_N;
    }
    row_type row(int n)
    {
        return row_type(begin() + (n * Cols_N));
    }
    col_type col(int n)
    {
        return col_type(begin() + n);
    }
    const_row_type row(int n) const
    {
        return const_row_type(begin() + (n * Cols_N));
    }
    const_col_type col(int n) const
    {
        return const_col_type(begin() + n);
    }
    iterator begin()
    {
        return m.begin();
    }
    iterator end()
    {
        return m.begin() + size();
    }
    const_iterator begin() const
    {
        return m;
    }
    const_iterator end() const
    {
        return m + size();
    }
    static int size()
    {
        return Rows_N * Cols_N;
    }

    // operators
    row_type operator[](int n)
    {
        return row(n);
    }
    const_row_type operator[](int n) const
    {
        return row(n);
    }

    // assignment operations
    self& operator=(const self& x)
    {
        m = x.m;
        return *this;
    }
    self& operator=(value_type x)
    {
        m = x;
        return *this;
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
    self& operator+=(value_type x)
    {
        m += x;
        return *this;
    }
    self& operator-=(value_type x)
    {
        m -= x;
        return *this;
    }
    self& operator*=(value_type x)
    {
        m *= x;
        return *this;
    }
    self& operator/=(value_type x)
    {
        m /= x;
        return *this;
    }
    self operator-()
    {
        return self(-m);
    }

    // friends
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
    friend bool operator==(const self& x, const self& y)
    {
        return x != y;
    }
    friend bool operator!=(const self& x, const self& y)
    {
        return x.m != y.m;
    }
private:
    kvector < Value_T, (Rows_N + 1) * Cols_N > m;
};

#endif