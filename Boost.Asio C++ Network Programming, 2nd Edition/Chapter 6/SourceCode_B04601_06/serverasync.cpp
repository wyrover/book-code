/* serverasync.cpp */
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <string>

boost::mutex global_stream_lock;

void WorkerThread(boost::shared_ptr<boost::asio::io_service> iosvc, int counter)
{
	global_stream_lock.lock();
	std::cout << "Thread " << counter << " Start.\n";
	global_stream_lock.unlock();

	while(true)
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

void OnAccept(const boost::system::error_code &ec)
{
	if(ec)
	{
		global_stream_lock.lock();
		std::cout << "OnAccept Error: " << ec << ".\n";
		global_stream_lock.unlock();
	}
	else
	{
		global_stream_lock.lock();
		std::cout << "Accepted!" << ".\n";
		global_stream_lock.unlock();
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
	
	boost::shared_ptr<boost::asio::io_service::strand> strand(
		new boost::asio::io_service::strand(*io_svc)
	);

	global_stream_lock.lock();
	std::cout << "Press ENTER to exit!\n";
	global_stream_lock.unlock();

	boost::thread_group threads;
	for(int i=1; i<=2; i++)
		threads.create_thread(boost::bind(&WorkerThread, io_svc, i));

	boost::shared_ptr< boost::asio::ip::tcp::acceptor > acceptor(
		new boost::asio::ip::tcp::acceptor(*io_svc)
	);
	
	boost::shared_ptr<boost::asio::ip::tcp::socket> sckt(
		new boost::asio::ip::tcp::socket(*io_svc)
	);

	try
	{
		boost::asio::ip::tcp::resolver resolver(*io_svc);
		boost::asio::ip::tcp::resolver::query query( 
			"127.0.0.1", 
			boost::lexical_cast<std::string>(4444)
		);
		boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
		acceptor->open(endpoint.protocol());
		acceptor->set_option( boost::asio::ip::tcp::acceptor::reuse_address(false));
		acceptor->bind(endpoint);
		acceptor->listen(boost::asio::socket_base::max_connections);
		acceptor->async_accept(*sckt, boost::bind(OnAccept, _1));

		global_stream_lock.lock();
		std::cout << "Listening on: " << endpoint << std::endl;
		global_stream_lock.unlock();
	}
	catch(std::exception &ex)
	{
		global_stream_lock.lock();
		std::cout << "Message: " << ex.what() << ".\n";
		global_stream_lock.unlock();
	}

	std::cin.get();

	boost::system::error_code ec;
	acceptor->close(ec);

	sckt->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
	sckt->close(ec);

	io_svc->stop();

	threads.join_all();

	return 0;
}