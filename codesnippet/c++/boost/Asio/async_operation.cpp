#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <iostream>

using namespace boost::asio;

io_service service;

void func(int i) {
    std::cout << "func called, i= " << i << std::endl;
}

void worker_thread() {
    service.run();
}

int main(int argc, char* argv[]) {
  for ( int i = 0; i < 10; ++i) {
      service.post(boost::bind(func, i));
  }

  boost::thread_group threads;
  for ( int i = 0; i < 3; ++i) {
      threads.create_thread(worker_thread);
  }
  // wait for all threads to be created
  //boost::this_thread::sleep( boost::posix_time::millisec(500));
  threads.join_all();

  return 0;
}
