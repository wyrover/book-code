#include <boost/asio.hpp>
#include <memory>
#include <boost/thread.hpp>
#include <iostream>
namespace asio = boost::asio;

typedef std::unique_ptr<asio::io_service::work> work_ptr;

#define PRINT_ARGS(msg) do {\
        boost::lock_guard<boost::mutex> lg(mtx); \
        std::cout << '[' << boost::this_thread::get_id() \
                  << "] " << msg << std::endl; \
    } while (0)

int main()
{
    asio::io_service service;
    // keep the workers occupied
    work_ptr work(new asio::io_service::work(service));
    boost::mutex mtx;
    // set up the worker threads in a thread group
    boost::thread_group workers;

    for (int i = 0; i < 3; ++i) {
        workers.create_thread([&service, &mtx]() {
            PRINT_ARGS("Starting worker thread ");
            service.run();
            PRINT_ARGS("Worker thread done");
        });
    }

    // Post work
    for (int i = 0; i < 20; ++i) {
        service.post(
        [&service, &mtx]() {
            PRINT_ARGS("Hello, world!");
            service.post([&mtx]() {
                PRINT_ARGS("Hola, mundo!");
            });
        });
    }

    work.reset(); // destroy work object: signals end of work
    workers.join_all(); // wait for all worker threads to finish
}

