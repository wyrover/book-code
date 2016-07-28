#include <boost/type_traits/is_pod.hpp>
#include <cstring>
#include <iostream>
#include <string>

struct MyStruct {
    int n;
    float f;
    const char *s;
};

template <typename T, size_t N>
T* fastCopy(T(&arr)[N], boost::true_type podType)
{
    std::cerr << "fastCopy for POD\n";
    T *cpyarr = new T[N];
    memcpy(cpyarr, arr, N * sizeof(T));
    return cpyarr;
}

template <typename T, size_t N>
T* fastCopy(T(&arr)[N], boost::false_type nonPodType)
{
    std::cerr << "fastCopy for non-POD\n";
    T *cpyarr = new T[N];
    std::copy(&arr[0], &arr[N], &cpyarr[0]);
    return cpyarr;
}

template <typename T, size_t N>
T* fastCopy(T(&arr)[N])
{
    return fastCopy(arr, typename boost::is_pod<T>::type());
}

int main()
{
    MyStruct podarr[10] = {};
    std::string strarr[10];
    auto* cpyarr = fastCopy(podarr);
    auto* cpyarr2 = fastCopy(strarr);
    delete []cpyarr;
    delete []cpyarr2;
}

