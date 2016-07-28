////一个整数类型向量。整数类型只有一个操作符+，但任务是讲一个值乘以2。
////如果没有bind，则可以使用函数化对象实现它。
//#include <functional>
//#include <algorithm>
//#include <vector>
//class Number {};
//inline Number operator+(Number, Number);
//
////Your Code Start
//struct mul_2_func_obj :public std::unary_function<Number, Number>
//{
//  Number operator()(Number n1)const
//  {
//      return n1 + n1;
//  }
//};
//void mul_2_impl1(std::vector<Number>& values)
//{
//  std::for_each(values.begin(), values.end(), mul_2_func_obj());
//}
//
//int main()
//{
//  return 0;
//}

////使用Boost.Bind的代码将是如下这样：
//#include<boost/bind.hpp>
//#include<functional>
//#include<vector>
//#include <algorithm>
//
////class Number {};
//void mul_2_impl2(std::vector<int>& values)
//{
//  std::for_each(values.begin(), values.end(), boost::bind(std::plus<int>(), _1, _1));
//
//}
//
//template <class T>
//void mul_2_impl3(std::vector<T>& values)
//{
//  std::for_each(values.begin(), values.end(), boost::bind(std::plus<T>(), _1, _1));
//}
//
//int main()
//{
//  return 0;
//}

//另一个例子，有两个类，它们使用一些传感器设备。设备和类来自不同的厂商，
//所以他们提供了不同的API。这两个类只有一个公共方法watch，它接受一个函数化对象。

//class Device1
//{
//private:
//  short temperature();
//  short wetness();
//  int illumination();
//  int atmospheric_pressure();
//  void wait_for_data();
//public:
//  template<class FuncT>
//  void watch(const FuncT& f)
//  {
//      for (;;)
//      {
//          wait_for_data();
//          f(
//              temperature(),
//              wetness(),
//              illumination(),
//              atmospheric_pressure()
//              );
//      }
//  }
//};
//
//class Device2
//{
//private:
//  short temperature();
//  short wetness();
//  int illumination();
//  int atmospheric_pressure();
//  void wait_for_data();
//public:
//  template<class FuncT>
//  void watch(const FuncT& f)
//  {
//      for (;;)
//      {
//          wait_for_data();
//          f(
//              wetness(),
//              temperature(),
//              atmospheric_pressure(),
//              illumination()
//              );
//      }
//  }
//};
//
////Device1::watch和Device2::watch函数以不同的顺序将值传递给函数化对象。
////其他一些库提供一个函数，用来检测风暴，并在一场风暴的风险足够高时抛出异常。
//void detect_storm(int wetness, int temperature, int atmospheric_pressure);
//
////你的任务是为两个设备都提供一个风暴检测函数。下面是如何使用bind函数实现。
//Device1 d1;
////产生的函数化对象将安静地忽略传递给函数调用的额外参数
//d1.watch(boost::bind(&detect_storm, _2, _1, _4));
//...
//d2.watch(boost::bind(&detect_storm, _1, _2, _3));

//boost.bind库提供了良好的性能，因为它不使用动态分配和虚函数。即使在C++11 lambda函数不能使用时，也是有用的


//#include<boost/bind.hpp>
//#include<functional>
//#include<vector>
//#include <algorithm>
//template<class FuncT>
//void watch(const FuncT& f)
//{
//  f(10, std::string("String"));
//  f(10, "Char array");
//  f(10, 10);
//}
//
//struct templated_foo
//{
//  template<class T>
//  void operator()(T, int)const
//  {
//      //没有实现，只是显示绑定函数仍然可以用作模板
//  }
//};
//
//void check_templated_bind()
//{
//  //我们可以直接指定函数化对象的返回类型
//  //当绑定失败时，这样做
//  watch(boost::bind<void>(templated_foo(), _2, _1));
//}
//
//int main()
//{
//  return 0;
//}

#include<boost/bind.hpp>
#include<vector>
#include<algorithm>
#include<functional>
#include<iostream>

class Number {};
inline Number operator+(Number, Number)
{
    return Number();
}

struct mul_2_func_obj: public std::unary_function<Number, Number> {
    Number operator()(Number n1) const
    {
        return n1 + n1;
    }
};

void mul_2_impl1(std::vector<Number>& values)
{
    std::for_each(values.begin(), values.end(), mul_2_func_obj());
}

void mul_2_impl2(std::vector<Number>& values)
{
    std::for_each(values.begin(), values.end(), boost::bind(std::plus<Number>(), _1, _1));
}

template<class T>
void mul_2_impl3(std::vector<T>& values)
{
    std::for_each(values.begin(), values.end(), boost::bind(std::plus<T>(), _1, _1));
}

class Device1
{
private:
    short temperature();
    short wetness();
    int illumination();
    int atmospheric_pressure();
    void wait_for_data();
public:
    template<class FuncT>
    void watch(const FuncT& f)
    {
        for (;;) {
            wait_for_data();
            f(
                temperature(),
                wetness(),
                illumination(),
                atmospheric_pressure()
            );
        }
    }
};

class Device2
{
private:
    short temperature();
    short wetness();
    int illumination();
    int atmospheric_pressure();
    void wait_for_data();
public:
    template<class FuncT>
    void watch(const FuncT& f)
    {
        for (;;) {
            wait_for_data();
            f(
                wetness(),
                temperature(),
                atmospheric_pressure(),
                illumination()
            );
        }
    }
};

void detect_storm(int wetness, int temperature, int atmospheric_pressure);

#include<assert.h>

struct storm_exception : std::exception {};

void detect_storm(int wetness, int temperature, int atmospheric_pressure)
{
    if (wetness == 2 && temperature == 1 && atmospheric_pressure == 4)
        throw storm_exception();
}

short Device1::temperature()
{
    return 1;
}
short Device1::wetness()
{
    return 2;
}
int Device1::illumination()
{
    return 3;
}
int Device1::atmospheric_pressure()
{
    return 4;
}
void Device1::wait_for_data()
{
    static int counter = 0;

    if (counter) {
        assert(false);
    }

    ++counter;
}

short Device2::temperature()
{
    return 1;
}
short Device2::wetness()
{
    return 2;
}
int Device2::illumination()
{
    return 3;
}
int Device2::atmospheric_pressure()
{
    return 4;
}
void Device2::wait_for_data()
{
    static int counter = 0;

    if (counter) {
        assert(false);
    }

    ++counter;
}

#include<string>

template <class FuncT>
void watch(const FuncT& f)
{
    f(10, std::string("String"));
    f(10, "Char array");
    f(10, 10);
}

struct templated_foo {
    template<class T>
    void operator()(T, int) const
    {
    }
};

void check_templated_bind()
{
    watch(boost::bind<void>(templated_foo(), _2, _1));
}

int main()
{
    std::vector<Number> vec(10);
    mul_2_impl1(vec);
    mul_2_impl2(vec);
    mul_2_impl3(vec);

    try {
        try {
            Device1 d1;
            d1.watch(boost::bind(&detect_storm, _2, _1, _4));
        } catch (const storm_exception&) {
            std::cout << "device1 find storm\n";
        }

        try {
            Device2 d2;
            d2.watch(boost::bind(&detect_storm, _1, _2, _3));
        } catch (const storm_exception&) {
            std::cout << "device2 find storm\n";
        }
    } catch (...) {
        assert(false);
    }

    check_templated_bind();
    system("pause");
    return 0;
}