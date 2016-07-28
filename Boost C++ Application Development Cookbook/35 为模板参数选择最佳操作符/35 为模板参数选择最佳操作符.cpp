namespace detail
{
struct pre_inc_functor {
    template <class T>
    void operator()(T& value) const
    {
        ++value;
    }
};

struct post_inc_functor {
    template <class T>
    void operator()(T& value) const
    {
        value++;
    }
};

struct plus_assignable_functor {
    template <class T>
    void operator()(T& value) const
    {
        value += T(1);
    }
};

struct plus_functor {
    template <class T>
    void operator()(T& value) const
    {
        value = value + T(1);
    }
};
}

#include <boost/type_traits/conditional.hpp>
#include <boost/type_traits/has_plus_assign.hpp>
#include <boost/type_traits/has_plus.hpp>
#include <boost/type_traits/has_post_increment.hpp>
#include <boost/type_traits/has_pre_increment.hpp>

template <class T>
void inc(T& value)
{
    // call ++value
    // or call value++
    // or value += T(1);
    // or value = value + T(1);
    typedef detail::plus_functor step_0_t;
    typedef typename boost::conditional <
    boost::has_plus_assign<T>::value,
          detail::plus_assignable_functor,
          step_0_t
          >::type step_1_t;
    typedef typename boost::conditional <
    boost::has_post_increment<T>::value,
          detail::post_inc_functor,
          step_1_t
          >::type step_2_t;
    typedef typename boost::conditional <
    boost::has_pre_increment<T>::value,
          detail::pre_inc_functor,
          step_2_t
          >::type step_3_t;
    step_3_t() // 默认构造仿函数
    (value); // 调用一个仿函数的operator()
}

struct has_only_postinc {
    has_only_postinc operator++(int)
    {
        return *this;
    }
};

struct has_plus_assignable {
    explicit has_plus_assignable(int) {}
};

has_plus_assignable operator+=(has_plus_assignable v1, has_plus_assignable)
{
    return v1;
}

struct has_only_plus {
    explicit has_only_plus(int) {}
};

has_only_plus operator+(has_only_plus v1, has_only_plus)
{
    return v1;
}

#include <boost/mpl/if.hpp>
template <class T>
void inc_mpl(T& value)
{
    typedef detail::plus_functor step_0_t;
    typedef typename boost::mpl::if_ <
    boost::has_plus_assign<T>,
          detail::plus_assignable_functor,
          step_0_t
          >::type step_1_t;
    typedef typename boost::mpl::if_ <
    boost::has_post_increment<T>,
          detail::post_inc_functor,
          step_1_t
          >::type step_2_t;
    typedef typename boost::mpl::if_ <
    boost::has_pre_increment<T>,
          detail::pre_inc_functor,
          step_2_t
          >::type step_3_t;
    step_3_t() // 默认构造仿函数
    (value); // 调用一个仿函数的operator()
}

#include <assert.h>
int main()
{
    int i = 0;
    inc(i);
    assert(i == 1);
    has_only_postinc pi;
    inc(pi);
    has_only_plus v(0);
    inc(v);
    has_plus_assignable n(0);
    inc(n);
    inc_mpl(i);
    assert(i == 2);
    inc_mpl(pi);
    inc_mpl(v);
    system("pause");
    return 0;
}