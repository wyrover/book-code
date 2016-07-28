/* mutexbind.cpp */
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <iostream>

boost::mutex global_stream_lock;

void WorkerThread(boost::shared_ptr<boost::asio::io_service> iosvc, int counter)
{
	global_stream_lock.lock();
	std::cout << counter << ".\n";
	global_stream_lock.unlock();
	
	iosvc->run();
	
	global_stream_lock.lock();
	std::cout << "End.\n";
	global_stream_lock.unlock();
}

int main(void)
{
	boost::shared_ptr<boost::asio::io_service> io_svc(
		new boost::asio::io_service
	);
	
	boost::shared_ptr<boost::asio::io_service::work> worker(
		new boost::asio::io_service::work(*io_svc)
	);

	std::cout << "Press any key to exit!" << std::endl;

	boost::thread_group threads;
	for(int i=1; i<=5; i++)
		threads.create_thread(boost::bind(&WorkerThread, io_svc, i));

	std::cin.get();

	io_svc->stop();

	threads.join_all();

	return 0;
}