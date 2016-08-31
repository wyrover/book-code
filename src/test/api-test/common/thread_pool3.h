#ifndef thread_pool3_h__
#define thread_pool3_h__

#include <boost/scoped_ptr.hpp>
#include <boost/function.hpp>

namespace cactus
{

class thread_pool
{
public:
    void enqueue(const boost::function<void()> & f);

    thread_pool(size_t threads);
    ~thread_pool();
private:
    class impl;
    boost::scoped_ptr<impl> impl_;
};

}

#endif // thread_pool3_h__
