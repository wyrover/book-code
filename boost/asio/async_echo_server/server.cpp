#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <string>

using namespace boost::asio;

class tcp_connection : public boost::enable_shared_from_this<tcp_connection>
{
public:
	typedef boost::shared_ptr<tcp_connection> pointer;

	static pointer create(io_service& svc)
	{
		return pointer(new tcp_connection(svc));
	}

	ip::tcp::socket& socket()
	{
		return socket_;
	}

	void start()
	{
		std::cout << "async_read" << std::endl;
		socket_.async_read_some(buffer(data_, sizeof(data_)), boost::bind(&tcp_connection::handle_read,
			shared_from_this(), placeholders::error, placeholders::bytes_transferred));
#if 0
		std::cout << "sync_read" << std::endl;
		boost::system::error_code ec;
		size_t len = socket_.read_some(buffer(data_, 512), ec);
		if (!ec)
		{
			socket_.write_some(buffer(data_, len), ec);
		}
#endif
	}

public:
	~tcp_connection()
	{
		std::cout << "~tcp_connection" << std::endl;
	}

private:
	tcp_connection(io_service& svc) : socket_(svc)
	{
	}

	void handle_read(const boost::system::error_code& ec, size_t bytes_transferred)
	{
		std::cout << "handle_read" << std::endl;
		if (!ec)
		{
			async_write(socket_, buffer(data_, bytes_transferred), boost::bind(&tcp_connection::handle_write,
				shared_from_this(), placeholders::error, placeholders::bytes_transferred));
		}
	}

	void handle_write(const boost::system::error_code& ec, size_t bytes_transferred)
	{
		// do nothing.
		std::cout << "handle_write" << std::endl;
	}

private:
	ip::tcp::socket socket_;
	char data_[512];
};

class tcp_server
{
public:
	tcp_server(io_service& svc) : acceptor_(svc, ip::tcp::endpoint(ip::tcp::v4(), 8888))
	{
		start_accept();
	}

private:
	void start_accept()
	{
		tcp_connection::pointer new_conn =
			tcp_connection::create(acceptor_.get_io_service());

		acceptor_.async_accept(new_conn->socket(),
			boost::bind(&tcp_server::handle_accept, this, new_conn, placeholders::error));

		std::cout << "begin accept" << std::endl;
	}

	void handle_accept(tcp_connection::pointer new_conn, const boost::system::error_code& ec)
	{
		if (!ec)
		{
			new_conn->start();
		}

		start_accept();
	}

private:
	ip::tcp::acceptor acceptor_;
};

int main()
{
	try
	{
		io_service svc;
		tcp_server server(svc);
		svc.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "main err: " << e.what() << std::endl;
	}

	return 0;
}

