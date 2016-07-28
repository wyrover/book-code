void some_function(unsigned short param);
int foo();

void foo1()
{
    //在代码某处有些编译器会警告int类型的数据转换为
    //unsigned short 并有可能丢失数据
    some_function(foo());
}

void foo2()
{
    //通常情况下，程序员只是将其显示转化为无符号短整型的数据类型来忽略这些警告
    //警告被忽略。看起来像一段正确的代码。
    some_function(static_cast<unsigned short>(foo()));
    //但这可能会使得它的错误非常难以检测到。
}

#include <boost/numeric/conversion/cast.hpp>
void correct_implementation()
{
    //100% correct
    some_function(
        boost::numeric_cast<unsigned short>(foo())
    );
}

#include<iostream>

void test_function()
{
    for (unsigned int i = 0; i < 100; ++i) {
        try {
            correct_implementation();
        } catch (const boost::numeric::bad_numeric_cast& e) {
            std::cout << '#' << i << ' ' << e.what() << std::endl;
        }
    }
}

void test_function1()
{
    for (unsigned int i = 0; i < 100; ++i) {
        try {
            correct_implementation();
        } catch (const boost::numeric::positive_overflow& e) {
            //针对正的溢出做一些事情
            std::cout << "POS OVERFLOW in #" << i << ' ' << e.what() << std::endl;
        } catch (const boost::numeric::negative_overflow& e) {
            //针对负的溢出做一些事情
            std::cout << "NEG OVERFLOW in #" << i << ' ' << e.what() << std::endl;
        }
    }
}

//boost::numeric_cast函数是通过boost::numeric::converter实现的，它可以被调整为使用不同的溢出、
//范围检查和四舍五入策略。
//NOTE:   but 我现在还看不懂。。。
#include<stdexcept>

template<class SourceT, class TargetT>
struct mythrow_overflow_handler {
    void operator()(boost::numeric::range_check_result r)
    {
        if (r != boost::numeric::cInRange) {
            throw std::logic_error("Not in range!");
        }
    }
};


template <class TargetT, class SourceT>
TargetT my_numeric_cast(const SourceT& in)
{
    using namespace boost;
    typedef numeric::conversion_traits<TargetT, SourceT> conv_traits;
    typedef numeric::numeric_cast_traits<TargetT, SourceT> cast_traits;
    typedef boost::numeric::converter
    <
    TargetT,
    SourceT,
    conv_traits,
    mythrow_overflow_handler<SourceT, TargetT> //!!!
    > converter;
    return converter::convert(in);
}

int main()
{
    foo1();
    foo2();
    test_function();
    std::cout << "\n\n\n";
    test_function1();
    std::cout << "\n\n\n";

    //在代码某处
    try {
        my_numeric_cast<short>(100000);
    } catch (const std::logic_error& e) {
        std::cout << "It works! " << e.what() << std::endl;
    }

    system("pause");
    return 1;
}

void some_function(unsigned short param)
{
    (void)param;
}

bool some_extremely_rare_condition()
{
    static int i = 0;
    ++i;

    if (i == 50 || i == 150) {
        return true;
    }

    return false;
}


bool another_extremely_rare_condition()
{
    static int i = 0;
    ++i;

    if (i == 60 || i == 160) {
        return true;
    }

    return false;
}

//Returns -1 if error occurred
int foo()
{
    if (some_extremely_rare_condition()) {
        return -1;
    } else if (another_extremely_rare_condition()) {
        return 1000000;
    }

    return 65535;
}