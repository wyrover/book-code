#include <iostream>
#include <string>
#include <functional>
#include <boost/thread/thread.hpp>

// A typedef to make the declarations below easier to read
typedef void (*WorkerFunPtr)(const std::string&);

template<typename FunT,   // The type of the function being called
         typename ParamT> // The type of its parameter
struct Adapter {
    Adapter(FunT f, ParamT& p) : // Construct this adapter and set the
        f_(f), p_(&p) {}          // members to the function and its arg

    void operator()()     // This just calls the function with its arg
    {
        f_(*p_);
    }
private:
    FunT    f_;
    ParamT* p_;  // Use the parameter's address to avoid extra copying
};

void worker(const std::string& s)
{
    std::cout << s << '\n';
}

int main()
{
    std::string s1 = "This is the first thread!";
    std::string s2 = "This is the second thread!";
    boost::thread thr1(Adapter<WorkerFunPtr, std::string>(worker, s1));
    boost::thread thr2(Adapter<WorkerFunPtr, std::string>(worker, s2));
    thr1.join();
    thr2.join();
}