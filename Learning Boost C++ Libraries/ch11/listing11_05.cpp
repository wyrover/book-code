#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <cstdlib>
#include <iostream>
#include <ctime>
namespace asio = boost::asio;

#define PRINT_ARGS(msg) do {\
        boost::lock_guard<boost::mutex> lg(mtx); \
        std::cout << '[' << boost::this_thread::get_id() \
                  << "] " << msg << std::endl; \
    } while (0)

int main()
{
    std::srand(std::time(0));
    asio::io_service service;
    asio::io_service::strand strand(service);
    boost::mutex mtx;
    size_t regular = 0, on_strand = 0;
    auto workFuncStrand = [&mtx, &on_strand] {
        ++on_strand;
        PRINT_ARGS(on_strand << ". Hello, from strand!");
        boost::this_thread::sleep(
            boost::posix_time::seconds(2));
    };
    auto workFunc = [&mtx, &regular] {
        PRINT_ARGS(++regular << ". Hello, world!");
        boost::this_thread::sleep(
            boost::posix_time::seconds(2));
    };

    // Post work
    for (int i = 0; i < 15; ++i) {
        if (rand() % 2 == 0) {
            service.post(strand.wrap(workFuncStrand));
        } else {
            service.post(workFunc);
        }
    }

    // set up the worker threads in a thread group
    boost::thread_group workers;

    for (int i = 0; i < 3; ++i) {
        workers.create_thread([&service, &mtx]() {
            PRINT_ARGS("Starting worker thread ");
            service.run();
            PRINT_ARGS("Worker thread done");
        });
    }

    workers.join_all(); // wait for all worker threads to finish
}

