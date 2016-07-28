#include <boost/function.hpp>
typedef boost::function<void(int)> fobject_t;

// Now this function may accept functional objects
void process_integers(const fobject_t& f);

#include <assert.h>
#include <deque>
int main()
{
    // 不带参数什么也不做的lambda函数
    process_integers([](int /*i*/) {});
    // 存储一个引用的lambda函数
    std::deque<int> ints;
    process_integers([&ints](int i) {
        ints.push_back(i);
    });
    //修改其内容的lambda函数
    std::size_t match_count = 0;
    process_integers([ints, &match_count](int i)mutable {
        if (ints.front() == i)
        {
            ++match_count;
        }
        ints.pop_front();
    });
    assert(match_count == 6);
    system("pause");
    return 0;
}

void process_integers(const fobject_t& f)
{
    static const int data[] = { 1, 2, 3, 4, 5, 200, 0 };
    // We'll be using only 6 elements in this example
    std::for_each(data, data + 6, f);
}