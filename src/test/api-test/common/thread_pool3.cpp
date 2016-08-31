#include "thread_pool3.h"

#include <deque>
#include <vector>

#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

#include "reverse_lock.h"

#include "thread_pool3.h"

namespace cactus
{

class thread_pool::impl
{
public:
    explicit impl(size_t t)
        : finished_(false)
    {
        for (size_t i = 0; i != t; ++i)
            threads_.push_back(boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&impl::execute, this))));
    }

    void enqueue(const boost::function<void()> & f)
    {
        {
            boost::lock_guard<boost::mutex> lock(mutex_);
            queue_.push_back(f);
        }
        cond_.notify_one();
    }

    ~impl()
    {
        {
            boost::lock_guard<boost::mutex> lock(mutex_);
            finished_ = true;
        }
        cond_.notify_all();

        for (Threads::const_iterator i = threads_.begin(), end = threads_.end(); i != end; ++i)
            (*i)->join();
    }
private:
    void execute()
    {
        try {
            boost::unique_lock<boost::mutex> lock(mutex_);

            while (!finished_ && !boost::this_thread::interruption_requested()) {
                cond_.timed_wait(lock, boost::posix_time::milliseconds(100));

                if (!queue_.empty()) {
                    boost::function<void()> f = queue_.front();
                    queue_.pop_front();
                    reverse_lock<boost::unique_lock<boost::mutex> > rlock(lock);

                    try {
                        f();
                    } catch (boost::thread_interrupted&) {
                        break;
                    } catch (...) {
                    }
                }
            }
        } catch (boost::thread_interrupted&) {
        }
    }

    typedef std::vector<boost::shared_ptr<boost::thread> > Threads;
    Threads threads_;
    boost::condition_variable cond_;
    boost::mutex mutex_;
    bool finished_;
    std::deque<boost::function<void()> > queue_;
};

thread_pool::thread_pool(size_t threads)
    : impl_(new impl(threads))
{
}

thread_pool::~thread_pool()
{
}

void thread_pool::enqueue(const boost::function<void()> & f)
{
    impl_->enqueue(f);
}

}