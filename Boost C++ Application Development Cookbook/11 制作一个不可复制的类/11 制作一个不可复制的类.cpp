
class descriptor_owner
{
    void* descriptor_;

public:
    explicit descriptor_owner(const char* params)
    {
        (void)params;
    }
    ~descriptor_owner()
    {
        //system_api_free_descriptor(descriptor_);
    }
};

//上面C++编译器会产生一个复制构造函数和赋值运算符，这样
//descriptor_owner类的潜在用户就有能力干出下面可怕的事

void foo()
{
    descriptor_owner d1("O_o");
    descriptor_owner d2("^_^");
    //d2的描述符未正确释放
    d2 = d1;
    //d2的析构函数将释放描述符
    //d1的析构函数将尝试释放已经释放的描述符
}

//boost::noncopyable通过使descriptor_owner_fixed的复制构造函数
//和赋值运算符私有，或者只是定义它们而没有实际实现
#include <boost/noncopyable.hpp>

class descriptor_owner_fixed : private boost::noncopyable
{
public:
    descriptor_owner_fixed(const char* params)
    {
        (void)params;
    }
};

void foo2()
{
    descriptor_owner_fixed d1("O_o");
    descriptor_owner_fixed d2("^_^");
    //将无法编译
    //d2 = d1;
    //也无法编译
    //descriptor_owner_fixed d3(d1);
}

struct noncopyable_or_not {
private:
    noncopyable_or_not(const noncopyable_or_not&);
    noncopyable_or_not& operator=(const noncopyable_or_not&);
public:
    noncopyable_or_not() {}
    noncopyable_or_not(noncopyable_or_not&) {}
    noncopyable_or_not& operator=(noncopyable_or_not&)
    {
        return *this;
    }
};

struct noncopyable_or_not2 {
private:
    noncopyable_or_not2(noncopyable_or_not2&);
    noncopyable_or_not2& operator=(noncopyable_or_not2&);
public:
    noncopyable_or_not2() {}
    noncopyable_or_not2(const noncopyable_or_not2&) {}
    noncopyable_or_not2& operator=(const noncopyable_or_not2&)
    {
        return *this;
    }
};

int main()
{
    foo();
    foo2();
    noncopyable_or_not non1;
    noncopyable_or_not non2 = non1;
    const noncopyable_or_not2 n1;
    noncopyable_or_not2 n2 = n1;
    //Supressing warnings about unused variables
    (void)non2;
    (void)n2;
    return 0;
}