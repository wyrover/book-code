#ifndef KSTRIDE_ITER_HPP
#define KSTRIDE_ITER_HPP

#include <iterator>

template<class Iter_T, int Step_N>
class kstride_iter
{
public:
    // public typedefs
    typedef typename std::iterator_traits<Iter_T>::value_type value_type;
    typedef typename std::iterator_traits<Iter_T>::reference reference;
    typedef typename std::iterator_traits<Iter_T>::difference_type difference_type;
    typedef typename std::iterator_traits<Iter_T>::pointer pointer;
    typedef std::random_access_iterator_tag iterator_category;
    typedef kstride_iter self;

    // constructors
    kstride_iter() : m(NULL) { }
    kstride_iter(const self& x) : m(x.m) { }
    explicit kstride_iter(Iter_T x) : m(x) { }

    // operators
    self& operator++()
    {
        m += Step_N;
        return *this;
    }
    self operator++(int)
    {
        self tmp = *this;
        m += Step_N;
        return tmp;
    }
    self& operator+=(difference_type x)
    {
        m += x * Step_N;
        return *this;
    }
    self& operator--()
    {
        m -= Step_N;
        return *this;
    }
    self operator--(int)
    {
        self tmp = *this;
        m -= Step_N;
        return tmp;
    }
    self& operator-=(difference_type x)
    {
        m -= x * Step_N;
        return *this;
    }
    reference operator[](difference_type n)
    {
        return m[n * Step_N];
    }
    reference operator*()
    {
        return *m;
    }

    // friend operators
    friend bool operator==(self x, self y)
    {
        return x.m == y.m;
    }
    friend bool operator!=(self x, self y)
    {
        return x.m != y.m;
    }
    friend bool operator<(self x, self y)
    {
        return x.m < y.m;
    }
    friend difference_type operator-(self x, self y)
    {
        return (x.m - y.m) / Step_N;
    }
    friend self operator+(self x, difference_type y)
    {
        return x += y * Step_N;
    }
    friend self operator+(difference_type x, self y)
    {
        return y += x * Step_N;
    }
private:
    Iter_T m;
};

#endif