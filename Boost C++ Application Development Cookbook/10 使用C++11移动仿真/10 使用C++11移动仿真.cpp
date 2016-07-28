#include<string>
#include<vector>
#include <memory>
#include<assert.h>

#include<boost/swap.hpp>
#include<boost/move/move.hpp>

namespace other
{
//它的默认构造函数是廉价的/快速的
class characteristics {};
}//namespace other

struct person_info {
    //在这里声明字段
    //...
    bool is_male_;
    std::string name_;
    std::string second_name_;
    other::characteristics characteristics_;

private:
    BOOST_COPYABLE_AND_MOVABLE(person_info);

public:
    //为了例子简单，我们将假定
    //person_info的默认构造函数和swap调用时非常快/廉价的
    person_info() {}
    person_info(const person_info& p)
        : is_male_(p.is_male_)
        , name_(p.name_)
        , second_name_(p.second_name_)
        , characteristics_(p.characteristics_)
    {}

    //在C+++03中，STL容器既没有移动操作符也没有移动构造函数
    //boost.move库是用一个非常高效的方式来实现的。当使用C+++11编译器时，所有用于
    //右值仿真的宏将扩展为C+++11的特定功能，否则（在C++03编译器中）将使用特定的数据类型
    //和绝不复制传入的值也不调用任何动态内存分配或虚函数的函数来模拟右值。
    person_info(BOOST_RV_REF(person_info) person)//移动构造函数
    {
        swap(person);
    }

    person_info& operator=(BOOST_COPY_ASSIGN_REF(person_info) person)//复制赋值
    {
        if (this != &person) {
            person_info tmp(person);
            swap(tmp);
        }

        return *this;
    }

    person_info& operator=(BOOST_RV_REF(person_info) person)//移动赋值
    {
        if (this != &person) {
            swap(person);
            person_info tmp;
            tmp.swap(person);
        }

        return *this;
    }

    void swap(person_info& rhs)
    {
        std::swap(is_male_, rhs.is_male_);
        name_.swap(rhs.name_);
        second_name_.swap(rhs.second_name_);
        //boost::swap一个非常有用的工具函数，会先在变量的命名空间中搜索swap函数，如果类没有swap函数，将使用swap的STL实现。
        boost::swap(characteristics_, rhs.characteristics_);
    }
};

int main()
{
    person_info vasya;
    vasya.name_ = "Vasya";
    vasya.second_name_ = "Snow";
    vasya.is_male_ = true;
    person_info new_vasya(boost::move(vasya));
    assert(new_vasya.name_ == "Vasya");
    assert(new_vasya.second_name_ == "Snow");
    assert(vasya.name_.empty());
    assert(vasya.second_name_.empty());
    vasya = boost::move(new_vasya);
    assert(vasya.name_ == "Vasya");
    assert(vasya.second_name_ == "Snow");
    assert(new_vasya.name_.empty());
    assert(new_vasya.second_name_.empty());
    new_vasya = vasya;
    assert(vasya.name_ == "Vasya");
    assert(vasya.second_name_ == "Snow");
    assert(new_vasya.name_ == "Vasya");
    assert(new_vasya.second_name_ == "Snow");
    system("pause");
    return 0;
}