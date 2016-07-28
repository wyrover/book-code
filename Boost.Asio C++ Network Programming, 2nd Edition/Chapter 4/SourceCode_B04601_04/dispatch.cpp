/* dispatch.cpp */
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <iostream>

boost::mutex global_stream_lock;

void WorkerThread(boost::shared_ptr<boost::asio::io_service> iosvc)
{
	global_stream_lock.lock();
	std::cout << "Thread Start.\n";
	global_stream_lock.unlock();
	
	iosvc->run();
	
	global_stream_lock.lock();
	std::cout << "Thread Finish.\n";
	global_stream_lock.unlock();
}

void Dispatch(int i)
{
	global_stream_lock.lock();
	std::cout << "dispath() Function for i = " << i <<  std::endl;
	global_stream_lock.unlock();
}

void Post(int i)
{
	global_stream_lock.lock();
	std::cout << "post() Function for i = " << i <<  std::endl;
	global_stream_lock.unlock();
}

void Running(boost::shared_ptr<boost::asio::io_service> iosvc)
{
	for( int x = 0; x < 5; ++x )
	{
		iosvc->dispatch(boost::bind(&Dispatch, x));
		iosvc->post(boost::bind(&Post, x));
		boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
	}
}

int main(void)
{
	boost::shared_ptr<boost::asio::io_service> io_svc(
		new boost::asio::io_service
	);
	
	boost::shared_ptr<boost::asio::io_service::work> worker(
		new boost::asio::io_service::work(*io_svc)
	);

	global_stream_lock.lock();
	std::cout << "The program will exit automatically once all work has finished." << std::endl;
	global_stream_lock.unlock();

	boost::thread_group threads;

	threads.create_thread(boost::bind(&WorkerThread, io_svc));

	io_svc->post(boost::bind(&Running, io_svc));

	worker.reset();

	threads.join_all();

	return 0;
}