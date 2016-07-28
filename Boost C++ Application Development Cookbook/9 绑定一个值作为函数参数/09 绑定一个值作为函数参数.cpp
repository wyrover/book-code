#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <functional>
#include <iostream>
void foo()
{
    boost::array<int, 12> values = { {1, 2, 3, 4, 5, 6, 7, 100, 99, 98, 97, 96} };
    std::size_t count0 = std::count_if(values.begin(), values.end(), std::bind1st(std::less<int>(), 5));
    std::cout << count0 << std::endl;
    std::size_t count1 = std::count_if(values.begin(), values.end(), boost::bind(std::less<int>(), 5, _1));
    std::cout << count1 << std::endl;
    assert(count0 == count1);
    boost::array<std::string, 3> str_values = { {"We", "are", " the champions!"} };
    count0 = std::count_if(str_values.begin(), str_values.end(), std::mem_fun_ref(&std::string::empty));
    count1 = std::count_if(str_values.begin(), str_values.end(), boost::bind(&std::string::empty, _1));
    assert(count0 == count1);
    std::cout << count0 << std::endl;
    //对长度小于5的字符串计数：
    //改代码无法编译！并很难理解
    //count0 = std::count_if(str_values.begin(), str_values.end(), std::bind2nd(
    //  std::bind1st(
    //      std::less<std::size_t>(),
    //      std::mem_fun_ref(&std::string::size)
    //      )
    //  , 5
    //  ));
    //当习惯绑定时，可读性变得好很多
    count1 = std::count_if(str_values.begin(), str_values.end(), boost::bind(std::less<std::size_t>(),
                           boost::bind(&std::string::size, _1), 5));
    assert(2 == count1);
    //比较字符串
    std::string s("Expensive copy constructor of std::string will be called when binding");
    //在绑定时，将调用开销昂贵的std::string复制构造函数
    count0 = std::count_if(str_values.begin(), str_values.end(), std::bind2nd(std::less<std::string>(), s));
    count1 = std::count_if(str_values.begin(), str_values.end(), boost::bind(std::less<std::string>(), _1, s));
    assert(count0 == count1);
    std::cout << count0 << std::endl;
}

#include<boost/ref.hpp>

void wierd_appender(std::string& to, const std::string& from)
{
    to += from;
}

//用booost::ref库，绕开昂贵的复制
void foo2()
{
    std::size_t count0;
    std::size_t count1;
    boost::array<std::string, 3> str_values = { {"We", " are", " the champions!"} };
    std::string s("Expensize copy constructor of std::string now " "won't be called when binding");
    //现在，绑定时将不会调用开销昂贵的std::string复制构造函数
    count0 = std::count_if(str_values.begin(), str_values.end(), std::bind2nd(std::less<std::string>(), boost::cref(s)));
    count1 = std::count_if(str_values.begin(), str_values.end(), boost::bind(std::less<std::string>(), _1, boost::cref(s)));
    assert(count0 == count1);
    //boost::ref可以用于连接字符串
    std::string result;
    std::for_each(str_values.cbegin(), str_values.cend(), boost::bind(&wierd_appender, boost::ref(result), _1));
    assert(result == "We are the champions!");
}

int main()
{
    foo();
    foo2();
    system("pause");
    return 0;
}