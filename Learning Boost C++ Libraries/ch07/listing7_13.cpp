#include <iostream>

template <unsigned int N>
struct Factorial {
    enum {value = N * Factorial < N - 1 >::value};
};

template <>
struct Factorial<0> {
    enum {value = 1};  // 0! == 1
};

int main()
{
    std::cout << Factorial<8>::value << '\n';  // prints 40320
}

