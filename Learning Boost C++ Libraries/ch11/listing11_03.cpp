#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <iostream>
namespace asio = boost::asio;

#define PRINT_ARGS(msg) do {\
        boost::lock_guard<boost::mutex> lg(mtx); \
        std::cout << '[' << boost::this_thread::get_id() \
                  << "] " << msg << std::endl; \
    } while (0)

int main()
{
    asio::io_service service;
    boost::mutex mtx;

    for (int i = 0; i < 20; ++i) {
        service.post([i, &mtx]() {
            PRINT_ARGS("Handler[" << i << "]");
            boost::this_thread::sleep(
                boost::posix_time::seconds(1));
        });
    }

    boost::thread_group pool;

    for (int i = 0; i < 4; ++i) {
        pool.create_thread([&service]() {
            service.run();
        });
    }

    pool.join_all();
}

