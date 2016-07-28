#include<boost/variant.hpp>
#include<iostream>
#include<vector>
#include<string>

int main()
{
    typedef boost::variant<int, const char*, std::string> my_var_t;
    std::vector<my_var_t> some_values;
    some_values.push_back(10);
    some_values.push_back("Hello there!");
    some_values.push_back(std::string("Wow!"));
    std::string& s = boost::get<std::string>(some_values.back());
    s += " That is great!\n";
    std::cout << s << std::endl;
    //variant没有空状态,但它有一个empty()函数，它总是返回false(假)。如果确实需要表示一个空状态，
    //只需在boost.variant库支持的类型的开始位置添加某个简单的类型，当variant包含该类型时，把它解释为一个空状态。
    typedef boost::variant<boost::blank, int, const char*, std::string> my_var_tb;
    //默认构造函数构建一个boost::blank实例
    my_var_tb var;
    //'which()'方法返回一个变体持有的类型的索引
    assert(var.which() == 0);//空状态
    var = "Hello, dear deng";
    assert(var.which() != 0);
    //可以使用两种方法从一个变体得到一个值
    boost::variant<int, std::string> variable(0);
    //如果变量的实际值不是一个int类型，下面的方法可能抛出boost::bad_get异常
    int s1 = boost::get<int>(variable);
    //如果变量的实际值不是一个int类型，将返回一个NULL指针
    int *s2 = boost::get<int>(&variable);
    system("pause");
    return 0;
}