#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include <thread>
#include <atomic>
#include <iostream>

using namespace boost;

class Service {
public:
	Service(){}

	void handle_client(
		asio::ssl::stream<asio::ip::tcp::socket>& ssl_stream)
	{
		try {
			// Blocks until the handshake completes.
			ssl_stream.handshake(
				asio::ssl::stream_base::server);

			asio::streambuf request;
			asio::read_until(ssl_stream, request, '\n');

			// Emulate request processing.
			int i = 0;
			while (i != 1000000)
				i++;
			std::this_thread::sleep_for(
				std::chrono::milliseconds(500));

			// Sending response.
			std::string response = "Response\n";
			asio::write(ssl_stream, asio::buffer(response));
		}
		catch (system::system_error &e) {
			std::cout << "Error occured! Error code = "
				<< e.code() << ". Message: "
				<< e.what();
		}
	}
};

class Acceptor {
public:
	Acceptor(asio::io_service& ios, unsigned short port_num) :
		m_ios(ios),
		m_acceptor(m_ios,
		asio::ip::tcp::endpoint(
		asio::ip::address_v4::any(),
		port_num)),
		m_ssl_context(asio::ssl::context::sslv23_server)
	{
		// Setting up the context.
		m_ssl_context.set_options(
			boost::asio::ssl::context::default_workarounds
			| boost::asio::ssl::context::no_sslv2
			| boost::asio::ssl::context::single_dh_use);

		m_ssl_context.set_password_callback(
			[this](std::size_t max_length,
			asio::ssl::context::password_purpose purpose)
			-> std::string
		{return get_password(max_length, purpose); }
		);

		m_ssl_context.use_certificate_chain_file("server.crt");
		m_ssl_context.use_private_key_file("server.key",
			boost::asio::ssl::context::pem);
		m_ssl_context.use_tmp_dh_file("dhparams.pem");

		// Start listening for incoming connection requests.
		m_acceptor.listen();
	}

	void accept() {
		asio::ssl::stream<asio::ip::tcp::socket>
			ssl_stream(m_ios, m_ssl_context);

		m_acceptor.accept(ssl_stream.lowest_layer());

		Service svc;
		svc.handle_client(ssl_stream);
	}

private:
	std::string get_password(std::size_t max_length,
		asio::ssl::context::password_purpose purpose) const
	{
		return "pass";
	}

private:
	asio::io_service& m_ios;
	asio::ip::tcp::acceptor m_acceptor;

	asio::ssl::context m_ssl_context;
};

class Server {
public:
	Server() : m_stop(false) {}

	void start(unsigned short port_num) {
		m_thread.reset(new std::thread([this, port_num]() {
			run(port_num);
		}));
	}

	void stop() {
		m_stop.store(true);
		m_thread->join();
	}

private:
	void run(unsigned short port_num) {
		Acceptor acc(m_ios, port_num);

		while (!m_stop.load()) {
			acc.accept();
		}
	}

	std::unique_ptr<std::thread> m_thread;
	std::atomic<bool> m_stop;
	asio::io_service m_ios;
};

int main()
{
	unsigned short port_num = 3333;

	try {
		Server srv;
		srv.start(port_num);

		std::this_thread::sleep_for(std::chrono::seconds(60));

		srv.stop();
	}
	catch (system::system_error &e) {
		std::cout << "Error occured! Error code = "
			<< e.code() << ". Message: "
			<< e.what();
	}

	return 0;
}
