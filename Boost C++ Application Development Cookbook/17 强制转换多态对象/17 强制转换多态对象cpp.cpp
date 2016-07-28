#include<boost/cast.hpp>
#include <iostream>
using namespace std;


struct object {
    virtual ~object() {}
};

struct banana : public object {
    void eat()const
    {
        std::cout << "eat banana..." << std::endl;
    }
    virtual ~banana() {}
};

struct pidgin/*鸽子*/ : public object {
    void fly() const {}
    virtual ~pidgin() {}
};

object* try_produce_banana();



//这样实现很难看！
void try_eat_banana_impl1()
{
    const object* obj = try_produce_banana();

    if (!obj) {
        throw std::bad_cast();
    }

    dynamic_cast<const banana&>(*obj).eat();
}
//booost.conversion提供了一个稍微好点的解决方案
//polymorphic_cast检查输入是否为空，然后试图做一个动态转换。
//在这些操作中的任何错误都将抛出一个std::bad_cast异常。
void try_eat_banana_impl2()
{
    const object* obj = try_produce_banana();
    boost::polymorphic_cast<const banana*>(obj)->eat();
}

object* try_produce_banana()
{
    static pidgin pidg;
    static banana banan;
    static int i = 0;
    ++i;

    if (i == 3 || i == 6) {
        return 0;
    } else if (i == 2 || i == 5) {
        return &pidg;
    }

    return &banan;
}



int main()
{
    try_eat_banana_impl1();

    try {
        try_eat_banana_impl1();
        assert(false);
    } catch (...) {}

    try {
        try_eat_banana_impl1();
        assert(false);
    } catch (...) {}

    try_eat_banana_impl2();

    try {
        try_eat_banana_impl2();
        assert(false);
    } catch (...) {}

    try {
        try_eat_banana_impl2();
        assert(false);
    } catch (...) {}

    system("pause");
    return 0;
}