#include<sstream>
#include<iostream>
void foo1()
{
    //而现在，'iss'变量会一直成为妨碍直到作用域的末尾
    //最好不要去想在那些操作期间，有多少不必要的操作，虚拟函数调用和内存分配发生。
    std::istringstream iss("100");
    int i;
    iss >> i;
    std::cout << i << std::endl;
}

#include<cstdlib>
void foo2()
{
    //所有的值都被转换为int了？
    //还是在中间的某处停止了？
    //并且现在'end'变量将会成为妨碍
    //顺便说一下，我们想要一个整数，但strtol返回long int...
    //转换后的值能容纳在int中吗？
    char* end;
    int i = std::strtol("100", &end, 10);
    printf("%d\n", i);
}

#include<boost/lexical_cast.hpp>

void foo3()
{
    int i = boost::lexical_cast<int>("100");
    (void)i;//Supressing warning about unused variable
    char chars[] = { '1', '0', '0' };
    int ii = boost::lexical_cast<int>(chars, 3);
    assert(ii == 100);

    try {
        //在x86中，short通常不能存储大于32767的值
        short s = boost::lexical_cast<short>("1000000");
        assert(false);//绝不能到达这里
        (void)s;//Supressing warning about unused variable
    } catch (const boost::bad_lexical_cast& e) {
        std::cout << e.what() << std::endl;
    }

    try {
        int i = boost::lexical_cast<int>("This is not a number!");
        assert(false);
        (void)i;//抑制有关未使用的变量的警告
    } catch (const boost::bad_lexical_cast& e) {
        std::cout << e.what() << std::endl;
    }
}

#include<locale>

//lexical_cast可以使用std::locale
void foo4()
{
    try {
        std::locale::global(std::locale("ru_RU.UTF8"));
        //在俄语中用逗号作为小数点（分隔符）
        float f = boost::lexical_cast<float>("1,0");
        assert(f < 1.01 && f > 0.99);
        std::locale::global(std::locale::classic());// Restoring C locale
    } catch (...) {
        //此处locale抛异常，不知道why，书上未说
    }
}

#include<algorithm>
#include<vector>
#include<iterator>
#include<boost/lexical_cast.hpp>

template <class ContainerT>
std::vector<long int> container_to_longs(const ContainerT& container)
{
    typedef typename ContainerT::value_type value_type;
    std::vector<long int> ret;
    typedef long int(*func_t)(const value_type&);
    func_t f = &boost::lexical_cast<long int, value_type>;
    std::transform(container.begin(), container.end(), std::back_inserter(ret), f);
    return ret;
}

#include <set>
#include <deque>
#include <string>
#include <boost/array.hpp>

void foo5()
{
    std::set<std::string> str_set;
    str_set.insert("1");
    assert(container_to_longs(str_set).front() == 1);
    std::deque<const char*> char_deque;
    char_deque.push_front("1");
    char_deque.push_back("2");
    assert(container_to_longs(char_deque).front() == 1);
    assert(container_to_longs(char_deque).back() == 2);
    typedef boost::array<unsigned char, 2> element_t;//array的array
    boost::array<element_t, 2> arrays = { {{{'1', '0'}}, {{'2', '0'}}} };
    assert(container_to_longs(arrays).front() == 10);
    std::vector<long int> ret = std::move(container_to_longs(arrays));
    assert(container_to_longs(arrays).back() == 20);
    assert(ret.back() == 20);
}

int main()
{
    foo1();
    foo2();
    foo3();
    foo4();
    foo5();
    system("pause");
    return 0;
}