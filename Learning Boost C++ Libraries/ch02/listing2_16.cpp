#include <boost/static_assert.hpp>

template <typename T>
class SmallObjectAllocator
{
    BOOST_STATIC_ASSERT(sizeof(T) <= 16);

public:
    SmallObjectAllocator() {}
};

struct Foo {
    char data[32];
};

int main()
{
    SmallObjectAllocator<int> intAlloc;
    // SmallObjectAllocator<Foo> fooAlloc; // ERROR: sizeof(Foo) > 16
}

