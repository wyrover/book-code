#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>

template <typename T, typename U>
T bitwise_or(const T& left, const U& right)
{
    BOOST_STATIC_ASSERT(boost::is_pod<T>::value &&
                        boost::is_pod<U>::value);
    BOOST_STATIC_ASSERT(sizeof(T) >= sizeof(U));
    T result = left;
    const unsigned char *right_array =
        reinterpret_cast<const unsigned char*>(&right);
    unsigned char *left_array =
        reinterpret_cast<unsigned char*>(&result);

    for (size_t i = 0; i < sizeof(U); ++i) {
        left_array[i] |= right_array[i];
    }

    return result;
}

struct X {
    int x;
    int y;
};

struct Y {
    char a;
    char b;
    short s;
};

int main()
{
    X x1{64, 96};
    Y y1{'a', 'b', 20};
    X x2 = bitwise_or(x1, y1);
}
