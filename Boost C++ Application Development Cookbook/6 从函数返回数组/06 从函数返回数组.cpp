#include<boost/array.hpp>
#include<algorithm>

//对值增加1的函数化对象
struct add_1: public std::unary_function<char, void> {
    void operator()(char& c)const
    {
        ++c;
    }
    //如果没有心情编写函数化对象，但是不知道'boost::bind(std::plus<char>(), _1, 1)'
    //做的是什么，那么reading 9
};

typedef boost::array<char, 4> array4_t;
array4_t& vector_advance(array4_t& val)
{
    //boost::array有begin(),cbegin(),end(),cend(),
    //rbegin(),size(),empty()和其他对于STL容器常见的函数
    std::for_each(val.begin(), val.end(), add_1());
    return val;
}

int main()
{
    //在C++11中，我们可以就像初始化一个数组那样初始化boost::array;
    //array4_t val = {0, 1, 2, 3};
    //但是C++03中，需要添加额外的一对大括号
    array4_t val = { {0, 1, 2, 3} };
    //boost::array的用途就像一个平常的数组；
    array4_t val_res;//如果值类型支持默认的构造函数和赋值，
    val_res = vector_advance(val);//它可以被默认构造并且是可赋值的
    assert(val.size() == 4);
    assert(val[0] == 1);
    /*val[4];*/   //会触发一个断言，因为最大的索引为3
    //我们可以使这个断言在编译时工作，参见30
    assert(sizeof(val) == sizeof(char) * array4_t::static_size);
    system("pause");
    return 0;
}