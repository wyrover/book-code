#define  _CRT_SECURE_NO_WARNINGS

#include<boost/scope_exit.hpp>
#include<cstdlib>
#include<cstdio>
#include<cassert>


// 为了在一个成员函数里面捕捉它， 我们使用特殊符号this_;
class theres_more_example
{
public:
    void close(std::FILE*) {}

    void theres_more_example_func()
    {
        std::FILE* f = 0;
        BOOST_SCOPE_EXIT(f, this_) {
            // 捕获对象'this_'
            this_->close(f);
        }
        BOOST_SCOPE_EXIT_END
    }
};


int main()
{
    theres_more_example test;
    test.theres_more_example_func();
    std::FILE* f = std::fopen("example_file.txt", "w");
    BOOST_SCOPE_EXIT(&f) {
        // 无论在作用域中发生了什么，都将执行这段代码，并且将正确关闭文件
        std::fclose(f);
    }
    BOOST_SCOPE_EXIT_END
    // 一些可能抛出异常或返回的代码
    system("pause");
    return 0;
}