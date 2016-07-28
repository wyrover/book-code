#include <string>
#include <vector>
#include <iostream>
#define BOOST_THREAD_PROVIDES_FUTURE
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include <boost/thread/future.hpp>
#include <boost/chrono.hpp>

int main()
{
    boost::promise<std::string> prom;
    boost::future<std::string> fut(prom.get_future());
    boost::shared_future<std::string> shfut(std::move(fut));
    boost::thread publisher([&prom]() {
        std::string id =
            boost::lexical_cast<std::string>(
                boost::this_thread::get_id());
        std::cout << "Publisher thread " << id
                  << " starting.\n";
        boost::this_thread::sleep_for(
            boost::chrono::seconds(15));
        prom.set_value(id);
    });
    auto thrFunc = [](boost::shared_future<std::string> sf,
    int waitFor) {
        while (sf.wait_for(boost::chrono::seconds(waitFor))
               == boost::future_status::timeout) {
            std::cout << "Subscriber thread "
                      << boost::this_thread::get_id()
                      << " waiting ...\n";
        }

        std::cout << "\nSubscriber thread "
                  << boost::this_thread::get_id()
                  << " got " << sf.get() << ".\n";
    };
    boost::thread subscriber1(thrFunc, shfut, 2);
    boost::thread subscriber2(thrFunc, shfut, 4);
    boost::thread subscriber3(thrFunc, shfut, 6);
    publisher.join();
    subscriber1.join();
    subscriber2.join();
    subscriber3.join();
}

