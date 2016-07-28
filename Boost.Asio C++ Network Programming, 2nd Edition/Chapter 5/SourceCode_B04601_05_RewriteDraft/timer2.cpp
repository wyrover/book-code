/* timer2.cpp */
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <iostream>

boost::mutex global_stream_lock;

void WorkerThread(boost::shared_ptr<boost::asio::io_service> iosvc, int counter)
{
	global_stream_lock.lock();
	std::cout << "Thread " << counter << " Start.\n";
	global_stream_lock.unlock();

	while( true )
	{
		try
		{
			boost::system::error_code ec;
			iosvc->run(ec);
			if(ec)
			{
				global_stream_lock.lock();
				std::cout << "Message: " << ec << ".\n";
				global_stream_lock.unlock();
			}
			break;
		}
		catch(std::exception &ex)
		{
			global_stream_lock.lock();
			std::cout << "Message: " << ex.what() << ".\n";
			global_stream_lock.unlock();
		}
	}

	global_stream_lock.lock();
	std::cout << "Thread " << counter << " End.\n";
	global_stream_lock.unlock();
}

void TimerHandler(
	const boost::system::error_code &ec,
	boost::shared_ptr<boost::asio::deadline_timer> tmr
)
{
	if(ec)
	{
		global_stream_lock.lock();
		std::cout << "Error Message: " << ec << ".\n";
		global_stream_lock.unlock();
	}
	else
	{
		global_stream_lock.lock();
		std::cout << "You see this every three seconds.\n";
		global_stream_lock.unlock();

		tmr->expires_from_now( boost::posix_time::seconds(3));
		tmr->async_wait(boost::bind(&TimerHandler, _1, tmr));
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
	std::cout << "Press ENTER to exit!\n";
	global_stream_lock.unlock();

	boost::thread_group threads;
	for(int i=1; i<=5; i++)
		threads.create_thread(boost::bind(&WorkerThread, io_svc, i));

	boost::shared_ptr<boost::asio::deadline_timer> timer(
		new boost::asio::deadline_timer(*io_svc)
	);
	timer->expires_from_now( boost::posix_time::seconds(3));
	timer->async_wait(boost::bind(&TimerHandler, _1, timer));

	std::cin.get();

	io_svc->stop();

	threads.join_all();

	return 0;
}