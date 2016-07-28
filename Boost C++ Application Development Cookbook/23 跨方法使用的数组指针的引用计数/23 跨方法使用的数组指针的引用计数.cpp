#include<cstring>
#include<boost/thread.hpp>
#include<boost/bind.hpp>

void do_process(const char* data, std::size_t size);

void do_process_in_backgroud(const char* data, std::size_t size)
{
    // 我们需要复制数据，因为我们不知道它什么时候会被调用者释放
    char* data_cpy = new char[size];
    std::memcpy(data_cpy, data, size);
    // 启动执行线程来处理数据
    boost::thread(boost::bind(&do_process, data_cpy, size)).detach();
    // 我们不能delete[] data_cpy, 因为do_process1 或 do_process2 可能任然在使用它
}

#include <boost/shared_array.hpp>
void do_process_shared_array(const boost::shared_array<char>& data, std::size_t size)
{
    do_process(data.get(), size);
}

// 第一个解决方案是boost的传统方法，在boost 1.53前，第二个解决方案的功能没有在shared_ptr中实现。
void do_process_in_background_v1(const char* data, std::size_t size)
{
    // 我们需要复制数据，因为我们不知道它什么时候会被调用者释放
    boost::shared_array<char> data_cpy(new char[size]);
    std::memcpy(data_cpy.get(), data, size);
    // 启动执行线程来处理数据
    boost::thread(boost::bind(&do_process_shared_array, data_cpy, size)).detach();
    // 不需要对data_cpy调用delete[], 因为当引用计数为零时，data_cpy的析构函数将释放数据
}

#include<boost/shared_ptr.hpp>
#include<boost/make_shared.hpp>

void do_process_shared_ptr(const boost::shared_ptr<char[]>& data, std::size_t size)
{
    do_process(data.get(), size);
}

//第二个解决方案是最快的一个（它使用较少的new调用），但它只能用于boost 1.53和更高版本。

void do_process_in_background_v2(const char* data, std::size_t size)
{
    // 执行速度比第一个解决方案快
    boost::shared_ptr<char[]> data_cpy = boost::make_shared<char[]>(size);
    std::memcpy(data_cpy.get(), data, size);
    // 启动执行线程来处理数据
    boost::thread(boost::bind(&do_process_shared_ptr, data_cpy, size)).detach();
    // 当引用计数为零时， data_cpy的析构函数将释放数据
}

void do_process_shared_ptr2(const boost::shared_ptr<char>& data, std::size_t size)
{
    do_process(data.get(), size);
}

//第三个解决方案是可移植性最好的。它可以用于旧版本的Boost与C+++11STL的shared_ptr<>（只是不要忘记将boost::checked_array_deleter<T>()改为std::default_delete<T[]>() ）。

void do_process_in_background_v3(const char* data, std::size_t size)
{
    // 执行速度与第一个解决方案相同
    boost::shared_ptr<char> data_cpy(new char[size], boost::checked_array_deleter<char>());
    std::memcpy(data_cpy.get(), data, size);
    // 启动执行线程来处理数据
    boost::thread(boost::bind(&do_process_shared_ptr2, data_cpy, size)).detach();
    // 当引用计数为零时， data_cpy的析构函数将释放数据
}

#include<boost/chrono/duration.hpp>

int main()
{
    // do_process_in_background(); // Will cause a memory leak
    char ch[] = "Hello deardeng.";
    do_process_in_background_v1(ch, sizeof(ch));
    do_process_in_background_v2(ch, sizeof(ch));
    do_process_in_background_v3(ch, sizeof(ch));
    // Give all the threads a chance to finish
    // Note: It is an awfull design, but it is OK
    // for example
    boost::this_thread::sleep_for(boost::chrono::seconds(2));
    system("pause");
    return 0;
}

void do_process(const char* data, std::size_t size)
{
    assert(size);
    assert(data);
}