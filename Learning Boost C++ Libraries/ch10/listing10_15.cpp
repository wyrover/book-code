#include <iostream>
#include <boost/coroutine/all.hpp>
#include <boost/bind.hpp>
#include <vector>
#include <string>

template <typename T>
using pull_type = typename
                  boost::coroutines::asymmetric_coroutine<T>::pull_type;

template <typename T>
using push_type = typename
                  boost::coroutines::asymmetric_coroutine<T>::push_type;

template <typename T>
void getNextElem(push_type<T>& sink,
                 const std::vector<T>& vec)
{
    for (const auto& elem : vec) {
        sink(elem);
    }
}

int main()
{
    std::vector<std::string> vec{"hello", "hi", "hola",
                                 "servus"};
    pull_type<std::string> greet_func(
        boost::bind(getNextElem<std::string>, ::_1,
                    boost::cref(vec)));

    while (greet_func) {
        std::cout << greet_func.get() << '\n';
        greet_func();
    }
}

