
void may_throw1(const char* buffer);
void may_throw2(const char* buffer);

void foo()
{
    //不能在栈上分配10MB的内存，所以在堆上分配它
    char* buffer = new char[1024 * 1024 * 10];
    //这里是一些可能会抛出异常的代码
    may_throw1(buffer);
    may_throw2(buffer);
    delete[] buffer;
}

#include <boost/scoped_array.hpp>

void foo_fixed()
{
    //在堆上分配它
    boost::scoped_array<char> buffer(new char[1024 * 1024 * 10]);
    //这里是一些可能抛出异常的代码，但是现在异常不会导致内存泄漏
    may_throw1(buffer.get());
    may_throw2(buffer.get());
    // 'buffer'变量的析构函数会调用delete[]
}

#include<stdexcept>
#include<assert.h>

int main()
{
    //foo(); // Leaks memory
    try {
        foo_fixed();
    } catch (...) {
    }

    system("pause");
    return 0;
}

void may_throw1(const char* /*buffer*/)
{
    // Do nothing
}

void may_throw2(const char* /*buffer*/)
{
    throw std::exception();
}