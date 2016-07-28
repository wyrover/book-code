#include<iostream>
class foo_class
{
public:
    char data[10000000];
    explicit foo_class(const char* /*param*/) {}
};

foo_class* get_data()
{
    static int i = 0;
    ++i;
    std::cout << "i----------------------->" << i << std::endl;

    if (i % 2) {
        return new foo_class("Just some string");
    } else {
        return 0;
    }
}

#include<boost/thread.hpp>
#include<boost/bind.hpp>

void process1(const foo_class* p);
void process2(const foo_class* p);
void process3(const foo_class* p);


void foo1()
{
    while (foo_class* p = get_data()) { //C way
        // There will be too many threads soon, see
        // recipe 'Parallel execution of different tasks'
        // for a good way to avoid uncontrolled growth of threads
        boost::thread(boost::bind(&process1, p)).detach();
        boost::thread(boost::bind(&process2, p)).detach();
        boost::thread(boost::bind(&process3, p)).detach();
        //delete p; //Oops!!!!
    }
}

#include <boost/shared_ptr.hpp>

void process_sp1(const boost::shared_ptr<foo_class>& p);
void process_sp2(const boost::shared_ptr<foo_class>& p);
void process_sp3(const boost::shared_ptr<foo_class>& p);

void foo2()
{
    typedef boost::shared_ptr<foo_class> ptr_t;
    ptr_t p;

    while (p = ptr_t(get_data())) { // C way
        std::cout << p << std::endl;
        boost::thread(boost::bind(&process_sp1, p)).detach();
        boost::thread(boost::bind(&process_sp2, p)).detach();
        boost::thread(boost::bind(&process_sp3, p)).detach();
        // no need to anything
    }
}

#include<string>
#include<boost/smart_ptr/make_shared.hpp>

void process_str1(boost::shared_ptr<std::string> p);
void process_str2(const boost::shared_ptr<std::string>& p);


//shared_ptr<std::string>ps(new int(0))它有两次对new的调用：第一次在构建
//一个整数的指针时，第二次在构造一个shared_ptr类时（它调用new在堆上分配一个原子计数器）。
//但当使用make_shared构造shared_ptr时，只调用new一次。
//它将分配单块内存并将在那个内存块构建一个原子计数器和int对象。
void foo3()
{
    boost::shared_ptr<std::string> ps = boost::make_shared<std::string>(
                                            "Guess why make_shared<std::string> "
                                            "is faster than shared_ptr<std::string> "
                                            "ps(new std::string('this string))"
                                        );
    boost::thread(boost::bind(&process_str1, ps)).detach();
    boost::thread(boost::bind(&process_str2, ps)).detach();
}

#include<boost/chrono/duration.hpp>
int main()
{
    // foo1(); //Will cause a memory leak
    foo2();
    foo3();
    // Give all the threads a chance to finish
    // Note: It is an awfull design, but it is OK
    // for example
    boost::this_thread::sleep_for(boost::chrono::seconds(2));
    system("pause");
    return 0;
}

void process1(const foo_class* p)
{
    assert(p);
}

void process2(const foo_class* p)
{
    assert(p);
}

void process3(const foo_class* p)
{
    assert(p);
}

void process_str1(boost::shared_ptr<std::string> p)
{
    assert(p);
}

void process_str2(const boost::shared_ptr<std::string>& p)
{
    assert(p);
}

void process_sp1(const boost::shared_ptr<foo_class>& p)
{
    std::cout << "thread_process_sp1..." << std::endl;
    assert(!!p);
}

void process_sp2(const boost::shared_ptr<foo_class>& p)
{
    std::cout << "thread_process_sp2..." << std::endl;
    assert(!!p);
}

void process_sp3(const boost::shared_ptr<foo_class>& p)
{
    std::cout << "thread_process_sp3..." << std::endl;
    assert(!!p);
}