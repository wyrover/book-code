#ifndef thread_pool_h__
#define thread_pool_h__

#include <memory>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>


//-------------------------------------------------------------------------------
class thread_pool
{
public:
    thread_pool(int thread_count);
    ~thread_pool(void);

    template <typename CompletionHandler>
    void enqueue(const CompletionHandler& handler);

private:
    static void thread_func(const thread_pool* pool);
    void execute_orders(void) const;

private:
    boost::shared_ptr<boost::asio::io_service>      _service;
    boost::shared_ptr<boost::asio::io_service::work>    _base_work;
    boost::thread_group                             _threads;

};
//-------------------------------------------------------------------------------
template<typename CompletionHandler>
void thread_pool::enqueue(const CompletionHandler& handler)
{
    _service->post(handler);
}
//-------------------------------------------------------------------------------

#endif // thread_pool_h__
