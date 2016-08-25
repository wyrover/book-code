#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "stride_iter.hpp" // see <link linkend="cplusplusckbk-CHP-11-SECT-12">Recipe 11.12</link>

#include <valarray>
#include <numeric>
#include <algorithm>

template<class Value_T>
class matrix
{
public:
    // public typedefs
    typedef Value_T value_type;
    typedef matrix self;
    typedef value_type* iterator;
    typedef const value_type* const_iterator;
    typedef Value_T* row_type;
    typedef stride_iter<value_type*> col_type;
    typedef const value_type* const_row_type;
    typedef stride_iter<const value_type*> const_col_type;

    // constructors
    matrix() : nrows(0), ncols(0), m() { }
    matrix(int r, int c) : nrows(r), ncols(c), m(r * c) { }
    matrix(const self& x) : m(x.m), nrows(x.nrows), ncols(x.ncols) { }

    template<typename T>
    explicit matrix(const valarray<T>& x)
        : m(x.size() + 1), nrows(x.size()), ncols(1)
    {
        for (int i = 0; i < x.size(); ++i) m[i] = x[i];
    }

    // allow construction from matricies of other types
    template<typename T>
    explicit matrix(const matrix<T>& x)
        : m(x.size() + 1), nrows(x.nrows), ncols(x.ncols)
    {
        copy(x.begin(), x.end(), m.begin());
    }

    // public functions
    int rows() const
    {
        return nrows;
    }
    int cols() const
    {
        return ncols;
    }
    int size() const
    {
        return nrows * ncols;
    }

    // element access
    row_type row_begin(int n)
    {
        return &m[n * cols()];
    }
    row_type row_end(int n)
    {
        return row_begin() + cols();
    }
    col_type col_begin(int n)
    {
        return col_type(&m[n], cols());
    }
    col_type col_end(int n)
    {
        return col_begin(n) + cols();
    }
    const_row_type row_begin(int n) const
    {
        return &m[n * cols()];
    }
    const_row_type row_end(int n) const
    {
        return row_begin() + cols();
    }
    const_col_type col_begin(int n) const
    {
        return col_type(&m[n], cols());
    }
    const_col_type col_end(int n) const
    {
        return col_begin() + cols();
    }
    iterator begin()
    {
        return &m[0];
    }
    iterator end()
    {
        return begin() + size();
    }
    const_iterator begin() const
    {
        return &m[0];
    }
    const_iterator end() const
    {
        return begin() + size();
    }

    // operators
    self& operator=(const self& x)
    {
        m = x.m;
        nrows = x.nrows;
        ncols = x.ncols;
        return *this;
    }
    self& operator=(value_type x)
    {
        m = x;
        return *this;
    }
    row_type operator[](int n)
    {
        return row_begin(n);
    }
    const_row_type operator[](int n) const
    {
        return row_begin(n);
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
    self& operator%=(value_type x)
    {
        m %= x;
        return *this;
    }
    self operator-()
    {
        return -m;
    }
    self operator+()
    {
        return +m;
    }
    self operator!()
    {
        return !m;
    }
    self operator~()
    {
        return ~m;
    }

    // friend operators
    friend self operator+(const self& x, const self& y)
    {
        return self(x) += y;
    }
    friend self operator-(const self& x, const self& y)
    {
        return self(x) -= y;
    }
    friend self operator+(const self& x, value_type y)
    {
        return self(x) += y;
    }
    friend self operator-(const self& x, value_type y)
    {
        return self(x) -= y;
    }
    friend self operator*(const self& x, value_type y)
    {
        return self(x) *= y;
    }
    friend self operator/(const self& x, value_type y)
    {
        return self(x) /= y;
    }
    friend self operator%(const self& x, value_type y)
    {
        return self(x) %= y;
    }
private:
    mutable valarray<Value_T> m;
    int nrows;
    int ncols;
};

#endif