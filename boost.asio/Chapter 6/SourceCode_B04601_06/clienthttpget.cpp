/* clienthttpget.cpp */
#include "wrapper.h"
#include <conio.h>
#include <boost/thread/mutex.hpp>

boost::mutex global_stream_lock;

class MyConnection : public Connection
{
private:
	void OnAccept(const std::string &host, uint16_t port)
	{
		global_stream_lock.lock();
		std::cout << "[OnAccept] " << host << ":" << port << "\n";
		global_stream_lock.unlock();

		// Start the next receive
		Recv();
	}

	void OnConnect(const std::string &host, uint16_t port)
	{
		global_stream_lock.lock();
		std::cout << "[OnConnect] " << host << ":" << port << "\n";
		global_stream_lock.unlock();

		// Start the next receive
		Recv();

		std::string str = "GET / HTTP/1.0\r\n\r\n";

		std::vector<uint8_t> request;
		std::copy(str.begin(), str.end(), std::back_inserter(request));
		Send(request);
	}

	void OnSend(const std::vector<uint8_t> &buffer)
	{
		global_stream_lock.lock();
		std::cout << "[OnSend] " << buffer.size() << " bytes\n";
		for(size_t x=0; x<buffer.size(); x++)
		{
			std::cout << std::hex << std::setfill('0') << 
				std::setw(2) << (int)buffer[x] << " ";
			if((x + 1) % 16 == 0)
				std::cout << "\n";
		}
		std::cout << "\n";
		global_stream_lock.unlock();
	}

	void OnRecv(std::vector<uint8_t> &buffer)
	{
		global_stream_lock.lock();
		std::cout << "[OnRecv] " << buffer.size() << " bytes\n";
		for(size_t x=0; x<buffer.size(); x++)
		{
			std::cout << std::hex << std::setfill('0') << 
				std::setw(2) << (int)buffer[x] << " ";
			if((x + 1) % 16 == 0)
				std::cout << "\n";
		}
		std::cout << "\n";
		global_stream_lock.unlock();

		// Start the next receive
		Recv();
	}

	void OnTimer(const boost::posix_time::time_duration &delta)
	{
		global_stream_lock.lock();
		std::cout << "[OnTimer] " << delta << std::endl;
		global_stream_lock.unlock();
	}

	void OnError(const boost::system::error_code &error)
	{
		global_stream_lock.lock();
		std::cout << "[OnError] " << error << "\n";
		global_stream_lock.unlock();
	}

public:
	MyConnection(boost::shared_ptr<Hive> hive)
		: Connection(hive)
	{
	}

	~MyConnection()
	{
	}
};

int main(void)
{
	boost::shared_ptr<Hive> hive(new Hive());

	boost::shared_ptr<MyConnection> connection(new MyConnection(hive));
	connection->Connect("www.packtpub.com", 80);

	while(!_kbhit())
	{
		hive->Poll();
		Sleep(1);
	}

	hive->Stop();

	return 0;
}