// std::unary_function<> 模板所需要的
#include <functional>

// 为接受int并且不返回任何东西的函数制作一个typedef
typedef void(*func_t)(int);
// 函数接受指向函数的指针，并且对于它拥有的每个整数调用被接受的函数
// 它不能处理函数化对象 :（
void process_integers(func_t f);

// 函数化对象
class int_processor : public std::unary_function<int, void>
{
    const int min_;
    const int max_;
    bool& triggered_;

public:
    int_processor(int min, int max, bool& triggered)
        : min_(min)
        , max_(max)
        , triggered_(triggered)
    {}

    void operator()(int i) const
    {
        if (i < min_ || i > max_) {
            triggered_ = true;
        }
    }
};

#include <boost/function.hpp>
typedef boost::function<void(int)> fobject_t;

// 现在这个函数可以接受函数化对象
void process_integers(const fobject_t& f);

int main()
{
    bool is_triggered = false;
    int_processor fo(0, 200, is_triggered);
    process_integers(fo);
    assert(is_triggered);
    system("pause");
    return 0;
}

void foo(const fobject_t& f)
{
    // boost::function 是可以转换成bool的
    if (f) {
        // 'f'中有值
    } else {
        // 'f'为空
        // ...
    }
}

bool g_is_triggered = false;
void set_functional_object(fobject_t& f)
{
    int_processor fo(100, 200, g_is_triggered);
    f = fo;
    // 'fo'离开作用域并且将被销毁，但'f'即使在作用域外也是可用的
}

#include <vector>
#include <algorithm>
#include <boost/bind.hpp>

void foo1()
{
    std::vector<int> v;
    std::for_each(v.begin(), v.end(), boost::bind(std::plus<int>(), 10, _1));
    fobject_t f(boost::bind(std::plus<int>(), 10, _1));
    std::for_each(v.begin(), v.end(), f);
}

void process_integers(const fobject_t& f)
{
    static const int data[] = { 1, 2, 3, 4, 5, 200 };
    std::for_each(data, data + sizeof(data), f);
}