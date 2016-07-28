#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <iostream>

using namespace boost::asio;
using namespace boost::asio::ip;

io_service svc;

void sync_echo(std::string msg)
{
	udp::endpoint ep(address::from_string("127.0.0.1"), 8888);
	udp::socket sock(svc);
	sock.open(udp::v4());
	sock.send_to(buffer(msg), ep);

	char buf[1024];
	udp::endpoint sender_ep;
	size_t bytes = sock.receive_from(buffer(buf), sender_ep);
	std::string copy(buf, bytes);
	std::cout << "server echoed our \"" << msg << "\": " << (copy == msg ? "OK" : "FAIL") << std::endl;
	sock.close();
}

int main()
{
	char* messages[] = {"Message 1", "Message 2", "Message 3", 0};
	boost::thread_group threads;
	for (char** msg = messages; *msg; ++msg)
	{
		threads.create_thread(boost::bind(sync_echo, *msg));
	}

	boost::this_thread::sleep(boost::posix_time::millisec(100));
	threads.join_all();

	return 0;
}

