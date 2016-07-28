#include <boost/asio.hpp>
#include <iostream>

using namespace boost::asio;

void checkErrorCode(const boost::system::error_code& ec, const std::string& tag)
{
	if (!ec)
		return;

	std::cout << tag << ": " <<  ec << std::endl;
	exit(1);
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "usage:" << std::endl;
		std::cout << "  sync_get.exe <url>" << std::endl;
		return 0;
	}

	io_service svc;
	boost::system::error_code ec;

	// dns resolver
	ip::tcp::resolver resolver(svc);
	ip::tcp::resolver::query query(argv[1], "80");
	ip::tcp::resolver::iterator it = resolver.resolve(query, ec);
	checkErrorCode(ec, "resolver.resolve()");

	ip::tcp::socket sock(svc);
	sock.connect(it->endpoint(), ec);
	checkErrorCode(ec, "sock.connect()");

	sock.write_some(buffer("GET /\r\n"), ec);
	checkErrorCode(ec, "sock.write_some()");

	std::cout << "bytes available: " << sock.available() << std::endl;
	char buf[512];
	size_t n = sock.read_some(buffer(buf, sizeof(buf)-1), ec);
	while (!ec)
	{
		buf[n] = '\0';
		std::cout << buf;
		n = sock.read_some(buffer(buf), ec);
	}
	std::cout << std::endl;
	
	return 0;
}

