#include<boost/any.hpp>
#include<iostream>
#include<vector>
#include<string>
int main()
{
    std::vector<boost::any> some_values;
    some_values.push_back(10);
    const char* c_str = "Hello there!";
    some_values.push_back(c_str);
    some_values.push_back(std::string("Wow"));
    std::string& s = boost::any_cast<std::string&>(some_values.back());
    s += " That is great!\n";
    std::cout << s;
    //any有一个空的状态，且可以使用empty()成员函数检查
    boost::any variable(std::string("Hello world"));
    //#1:如果变量的实际值不是一个std::string,以下方法可能会抛出一个boost::bad_any_cast异常
    std::string s1 = boost::any_cast<std::string>(variable);
    std::cout << s1 << std::endl;
    //#2:如果变量的实际值不是一个std::string将返回一个NULL指针
    std::string* s2 = boost::any_cast<std::string>(&variable);
    std::cout << *s2 << std::endl;
    system("pause");
    return 0;
}