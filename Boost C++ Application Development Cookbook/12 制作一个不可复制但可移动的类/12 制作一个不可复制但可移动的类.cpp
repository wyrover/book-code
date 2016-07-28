#include<boost/noncopyable.hpp>
#include<algorithm>
#include<iostream>
#include<string.h>
#include<assert.h>

//有一个无法复制的资源，它应该在析构函数中被正确释放，并且从一个函数返回它
class descriptor_owner : private boost::noncopyable
{
    void* descriptor_;

public:
    descriptor_owner() : descriptor_(NULL) {}
    explicit descriptor_owner(const char* param): descriptor_(_strdup(param)) {}

    void swap(descriptor_owner& desc)
    {
        std::swap(descriptor_, desc.descriptor_);
    }
    ~descriptor_owner()
    {
        free(descriptor_);
    }
};

////noncopyable error C2280: “descriptor_owner::descriptor_owner(const descriptor_owner &)”: 尝试引用已删除的函数
//descriptor_owner construct_descriptor()
//{
//  return descriptor_owner("Construct using this string");
//  //用这个字符串构造
//}

//可以使用swap方法解决这种情况
void construct_descriptor1(descriptor_owner& ret)
{
    descriptor_owner("Construct using this string").swap(ret);
}

//但这样的解决方法不允许我们在STL或Boost容器中使用descriptor_owner。另外，它看起来太可怕。

#include<boost/config.hpp>

//只可以移动的descriptor_owner类
#if !defined(BOOST_NO_RVALUE_REFERENCES) && !defined(BOOST_NO_CXX11_RVALUE_REFERENCES)

class descriptor_owner1
{
    void* descriptor_;

public:
    descriptor_owner1() : descriptor_(NULL) {}
    explicit descriptor_owner1(const char* param) : descriptor_(_strdup(param)) {}

    descriptor_owner1(descriptor_owner1&& param) : descriptor_(param.descriptor_)
    {
        param.descriptor_ = NULL;
    }

    descriptor_owner1& operator=(descriptor_owner1&& param)
    {
        clear();
        std::swap(descriptor_, param.descriptor_);
        return *this;
    }

    void clear()
    {
        free(descriptor_);
        descriptor_ = NULL;
    }

    bool empty()const
    {
        return !descriptor_;
    }

    ~descriptor_owner1()
    {
        clear();
    }
};

descriptor_owner1 construct_descriptor2()
{
    return descriptor_owner1("Construct using this string");
}

void foo_rv()
{
    std::cout << "C++11\n";
    descriptor_owner1 desc;
    desc = construct_descriptor2();
    assert(!desc.empty());
}
#else
void foo_rv()
{
    std::cout << "no C++11\n";
}
#endif

#include <boost/move/move.hpp>
#include <boost/container/vector.hpp>

//可以移动但不可以复制的类
class descriptor_owner_movable
{
private:
    void *descriptor_;
    BOOST_MOVABLE_BUT_NOT_COPYABLE(descriptor_owner_movable)
public:
    descriptor_owner_movable()
        : descriptor_(NULL)
    {}

    explicit descriptor_owner_movable(const char* param)
        : descriptor_(_strdup(param))
    {}

    descriptor_owner_movable(BOOST_RV_REF(descriptor_owner_movable) param) BOOST_NOEXCEPT//移动构造
:
    descriptor_(param.descriptor_)
    {
        param.descriptor_ = NULL;
    }

    descriptor_owner_movable& operator=(BOOST_RV_REF(descriptor_owner_movable) param) BOOST_NOEXCEPT { //移动赋值
        clear();
        std::swap(descriptor_, param.descriptor_);
        return *this;
    }

    void clear()
    {
        free(descriptor_);
        descriptor_ = NULL;
    }

    bool empty()const
    {
        return !descriptor_;
    }

    ~descriptor_owner_movable()BOOST_NOEXCEPT {
        clear();
    }
};

descriptor_owner_movable construct_descriptor3()
{
    return descriptor_owner_movable("Construct using this string");
}

#ifndef YOUR_PROJECT_VECTOR_HPP
#define YOUR_PROJECT_VECTOR_HPP

#include<boost/config.hpp>

//确实有右值
#if !defined(BOOST_NO_RVALUE_REFERENCES) && !defined(BOOST_NO_CXX11_RVALUE_REFERENCES)

#include<vector>
namespace your_project_namespace
{
using std::vector;
}

#else
//没有右值
#include<boost/container/vector.hpp>
namespace your_project_namespace
{
using boost::container::vector;
}

#endif //!defined(BOOST_NO_RVALUE_REFERENCES) && !defined(BOOST_NO_CXX11_RVALUE_REFERENCES)

#endif //YOUR_PROJECT_VECTOR_HPP


int main()
{
    foo_rv();
    descriptor_owner_movable movable;
    movable = construct_descriptor3();
    boost::container::vector<descriptor_owner_movable> vec;
    vec.resize(10);
    vec.push_back(construct_descriptor3());
    vec.back() = boost::move(vec.front());
    your_project_namespace::vector<descriptor_owner_movable> v;
    v.reserve(10);
    v.push_back(construct_descriptor3());
    v.back() = boost::move(v.front());
    system("pause");
    return 0;
}