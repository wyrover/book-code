#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

//template <class T>
//T process_data(const T& v1, const T& v2, const T& v3);

template <class T>
T process_data_plus_assign(const T& v1, const T& v2, const T& v3)
{
    BOOST_STATIC_ASSERT((boost::is_same<int, T>::value));
    (void)v2;
    (void)v3;
    return v1;
}

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/has_plus_assign.hpp>

// process_data的通用版本
template <class T>
typename boost::disable_if_c<boost::has_plus_assign<T>::value, T>::type
process_data(const T& v1, const T& v2, const T& v3)
{
    BOOST_STATIC_ASSERT((boost::is_same<const char*, T>::value));
    (void)v2;
    (void)v3;
    return v1;
}

// 这个process_data 将调用一个process_data_plus_assign
template <class T>
typename boost::enable_if_c<boost::has_plus_assign<T>::value, T>::type
process_data(const T& v1, const T& v2, const T& v3)
{
    return process_data_plus_assign(v1, v2, v3);
}

// 第一个版本
template <class T>
typename boost::disable_if<boost::has_plus_assign<T>, T>::type
process_data2(const T& v1, const T& v2, const T& v3);

// process_data_plus_assign
template <class T>
typename boost::enable_if<boost::has_plus_assign<T>, T>::type
process_data2(const T& v1, const T& v2, const T& v3);


int main()
{
    int i = 1;
    // Optimized version
    process_data(i, i, i);
    // Default version
    // Explicitly specifing template parameter
    process_data<const char*>("Testing", "example", "function");
    system("pause");
    return 0;
}