#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_array.hpp>
#include <boost/type_traits/rank.hpp>
#include <boost/type_traits/extent.hpp>
#include <boost/type_traits/is_pod.hpp>
#include <string>
#include <iostream>
#include <cassert>

struct MyStruct {
    int n;
    float f;
    const char *s;
};

int main()
{
// check pointers
    typedef int* intptr;
    std::cout << "intptr is "
              << (boost::is_pointer<intptr>::value ? "" : "not ")
              << "pointer type\n";
// introspect arrays
    int arr[10], arr2[10][15];

    if (boost::is_array<decltype(arr)>::value) {
        assert(boost::rank<decltype(arr)>::value == 1);
        assert(boost::rank<decltype(arr2)>::value == 2);
        assert(boost::extent<decltype(arr)>::value == 10);
        assert(boost::extent<decltype(arr2)>::value == 10);
        assert((boost::extent<decltype(arr2), 1>::value) == 15);
        std::cout << "arr is an array\n";
    }

// POD vs non-POD types
    std::cout << "MyStruct is "
              << (boost::is_pod<MyStruct>::value ? "" : "not ")
              << "pod type." << '\n';
    std::cout << "std::string is "
              << (boost::is_pod<std::string>::value ? "" : "not ")
              << "pod type." << '\n';
}

