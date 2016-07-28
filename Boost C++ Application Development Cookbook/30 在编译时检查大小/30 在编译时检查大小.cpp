#include <cstring>
#include <boost/array.hpp>

// 这段代码有以下问题：
//      未检查缓冲区的大小，所以它可能溢出
//      此函数可能被用于非纯旧数据（POD）类型，这将导致不正确的行为
// 通过增加一些断言，可能解决部分问题

template <class T, std::size_t BufSizeV>
void serialize_bad(const T& value, boost::array<unsigned char, BufSizeV>& buffer)
{
    assert(BufSizeV >= sizeof(value));
    // TODO:fixme
    std::memcpy(&buffer[0], &value, sizeof(value));
}
// 但是，这是一个糟糕的解决方案，BufSizeV 和 sizeof(value)的值在编译时已知，
// 所以如果缓冲区过小，这段代码有可能编译失败，而不是有一个运行时断言（如果未调用函数，
// 可能不会在调用过程中触发它，甚至可能在分布模式被优化掉，所以可能会发生非常糟糕的事情）


#include <boost/static_assert.hpp>
#include <boost/type_traits/is_pod.hpp>

template <class T, std::size_t BufSizeV>
void serialize(const T& value, boost::array<unsigned char, BufSizeV>& buffer)
{
    BOOST_STATIC_ASSERT(BufSizeV >= sizeof(value));
    BOOST_STATIC_ASSERT(boost::is_pod<T>::value);
    std::memcpy(&buffer[0], &value, sizeof(value));
}

BOOST_STATIC_ASSERT(3 >= 1);

struct some_struct {
    enum enum_t { value = 1};
};
BOOST_STATIC_ASSERT(some_struct::value);

template <class T1, class T2>
struct some_templated_struct {
    enum enum_t { value = (sizeof(T1) == sizeof(T2)) };
};
BOOST_STATIC_ASSERT((some_templated_struct<int, unsigned int>::value));

#include <iostream>
#include <boost/type_traits/is_unsigned.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_const.hpp>

template <class T1, class T2>
void type_traits_examples(T1& /*v1*/, T2& /*v2*/)
{
    // 如果T1是一个无符号数，返回true
    std::cout <<  std::boolalpha << boost::is_unsigned<T1>::value << std::endl;
    // 如果T1与T2具有完全相同的类型，返回true
    std::cout << boost::is_same<T1, T2>::value << std::endl;
    // 这行删除T1类型的const修饰符
    // 这里是将发生在T1类型上的，如果T1是：
    // const int => int
    // int => int
    // int const valatile => int volatile
    // const int& => const int&
    typedef typename boost::remove_const<T1>::type t1_nonconst_t;
}

template <class T, std::size_t BufSizeV>
void serialize2(const T& value, boost::array<unsigned char, BufSizeV>& buf)
{
    BOOST_STATIC_ASSERT_MSG(boost::is_pod<T>::value,
                            "This serialize2 function may be used only "
                            "with POD types."
                           );
    BOOST_STATIC_ASSERT_MSG(BufSizeV >= sizeof(value),
                            "Can not fit value to buffer. "
                            "Make buffer bigger."
                           );
    std::memcpy(&buf[0], &value, sizeof(value));
}

int main()
{
    const int i = 1;
    type_traits_examples(i, i);
    // Somewhere in code:
    boost::array<unsigned char, 1> buf;
    // serialize2(std::string("Hello word"), buf);
    (void)buf;
    system("pause");
    return 0;
}