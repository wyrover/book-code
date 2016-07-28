/* multithreads.cpp */
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <iostream>

boost::asio::io_service io_svc;
int a = 0;

void WorkerThread()
{
	std::cout << ++a << ".\n";
	io_svc.run();
	std::cout << "End.\n";
}

int main(void)
{
	boost::shared_ptr<boost::asio::io_service::work> worker(
		new boost::asio::io_service::work(io_svc)
	);

	std::cout << "Press any key to exit!" << std::endl;

	boost::thread_group threads;
	for(int i=0; i<5; i++)
		threads.create_thread(WorkerThread);

	std::cin.get();

	io_svc.stop();

	threads.join_all();

	return 0;
}