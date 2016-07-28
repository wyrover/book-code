#include <boost/predef.h> // Tools to identify the OS.

// We need this to enable cancelling of I/O operations on
// Windows XP, Windows Server 2003 and earlier.
// Refer to "http://www.boost.org/doc/libs/1_58_0/
// doc/html/boost_asio/reference/basic_stream_socket/
// cancel/overload1.html" for details.
#ifdef BOOST_OS_WINDOWS
#define _WIN32_WINNT 0x0501

#if _WIN32_WINNT <= 0x0502 // Windows Server 2003 or earlier.
#define BOOST_ASIO_DISABLE_IOCP
#define BOOST_ASIO_ENABLE_CANCELIO	
#endif
#endif

#include <boost/asio.hpp>

#include <thread>
#include <mutex>
#include <memory>
#include <iostream>

using namespace boost;

namespace http_errors {
	enum http_error_codes
	{
		invalid_response = 1
	};
	
	class http_errors_category
		: public boost::system::error_category
	{
	public:
		const char* name() const BOOST_SYSTEM_NOEXCEPT { return "http_errors"; }

		std::string message(int e) const {
			switch (e) {
			case invalid_response:
				return "Server response cannot be parsed.";
				break;
			default:
				return "Unknown error.";
				break;
			}
		}
	};
	
	const boost::system::error_category&
		get_http_errors_category()
	{
			static http_errors_category cat;
			return cat;
		}

	boost::system::error_code
		make_error_code(http_error_codes e)
	{
			return boost::system::error_code(
				static_cast<int>(e), get_http_errors_category());
		}
} // namespace http_errors

namespace boost {
	namespace system {
		template<>
		struct is_error_code_enum
			<http_errors::http_error_codes>
		{
			BOOST_STATIC_CONSTANT(bool, value = true);
		};
	} // namespace system
} // namespace boost

class HTTPClient;
class HTTPRequest;
class HTTPResponse;

typedef void(*Callback) (const HTTPRequest& request,
	const HTTPResponse& response,
	const system::error_code& ec);


class HTTPResponse {
	friend class HTTPRequest;
	HTTPResponse() :
		m_response_stream(&m_response_buf)
	{}
public:

	unsigned int get_status_code() const {
		return m_status_code;
	}

	const std::string& get_status_message() const {
		return m_status_message;
	}

	const std::map<std::string, std::string>& get_headers() {
		return m_headers;
	}

	const std::istream& get_response() const {
		return m_response_stream;
	}

private:
	asio::streambuf& get_response_buf() {
		return m_response_buf;
	}

	void set_status_code(unsigned int status_code) {
		m_status_code = status_code;
	}

	void set_status_message(const std::string& status_message) {
		m_status_message = status_message;
	}

	void add_header(const std::string& name,
		const std::string& value)
	{
		m_headers[name] = value;
	}

private:
	unsigned int m_status_code; // HTTP status code.
	std::string m_status_message; // HTTP status message.

	// Response headers.
	std::map<std::string, std::string> m_headers;
	asio::streambuf m_response_buf;
	std::istream m_response_stream;
};

class HTTPRequest {
	friend class HTTPClient;

	static const unsigned int DEFAULT_PORT = 80;

	HTTPRequest(asio::io_service& ios, unsigned int id) :
		m_port(DEFAULT_PORT),
		m_id(id),
		m_callback(nullptr),
		m_sock(ios),
		m_resolver(ios),
		m_was_cancelled(false),
		m_ios(ios)
	{}
	
public:
	void set_host(const std::string& host) {
		m_host = host;
	}

	void set_port(unsigned int port) {
		m_port = port;
	}

	void set_uri(const std::string& uri) {
		m_uri = uri;
	}

	void set_callback(Callback callback) {
		m_callback = callback;
	}

	std::string get_host() const {
		return m_host;
	}

	unsigned int get_port() const {
		return m_port;
	}

	const std::string& get_uri() const {
		return m_uri;
	}

	unsigned int get_id() const {
		return m_id;
	}

	void execute() {
		// Ensure that precorditions hold.
		assert(m_port > 0);
		assert(m_host.length() > 0);
		assert(m_uri.length() > 0);
		assert(m_callback != nullptr);

		// Prepare the resolving query.
		asio::ip::tcp::resolver::query resolver_query(m_host,
			std::to_string(m_port),
			asio::ip::tcp::resolver::query::numeric_service);

		std::unique_lock<std::mutex>
			cancel_lock(m_cancel_mux);

		if (m_was_cancelled) {
			cancel_lock.unlock();
			on_finish(boost::system::error_code(
				asio::error::operation_aborted));
			return;
		}

		// Resolve the host name.
		m_resolver.async_resolve(resolver_query,
			[this](const boost::system::error_code& ec,
			asio::ip::tcp::resolver::iterator iterator)
		{
			on_host_name_resolved(ec, iterator);
		});
	}

	void cancel() {
		std::unique_lock<std::mutex>
			cancel_lock(m_cancel_mux);

		m_was_cancelled = true;

		m_resolver.cancel();

		if (m_sock.is_open()) {
			m_sock.cancel();
		}
	}
	
private:
	void on_host_name_resolved(
		const boost::system::error_code& ec,
		asio::ip::tcp::resolver::iterator iterator)
	{
		if (ec != 0) {
			on_finish(ec);
			return;
		}

		std::unique_lock<std::mutex>
			cancel_lock(m_cancel_mux);

		if (m_was_cancelled) {
			cancel_lock.unlock();
			on_finish(boost::system::error_code(
				asio::error::operation_aborted));
			return;
		}

		// Connect to the host.
		asio::async_connect(m_sock,
			iterator,
			[this](const boost::system::error_code& ec,
			asio::ip::tcp::resolver::iterator iterator)
		{
			on_connection_established(ec, iterator);
		});

	}
	
	void on_connection_established(
		const boost::system::error_code& ec,
		asio::ip::tcp::resolver::iterator iterator)
	{
			if (ec != 0) {
				on_finish(ec);
				return;
			}

			// Compose the request message.
			m_request_buf += "GET " + m_uri + " HTTP/1.1\r\n";

			// Add mandatory header.
			m_request_buf += "Host: " + m_host + "\r\n";

			m_request_buf += "\r\n";

			std::unique_lock<std::mutex>
				cancel_lock(m_cancel_mux);

			if (m_was_cancelled) {
				cancel_lock.unlock();
				on_finish(boost::system::error_code(
					asio::error::operation_aborted));
				return;
			}

			// Send the request message.
			asio::async_write(m_sock,
				asio::buffer(m_request_buf),
				[this](const boost::system::error_code& ec,
				std::size_t bytes_transferred)
			{
				on_request_sent(ec, bytes_transferred);
			});
		}
	
	void on_request_sent(const boost::system::error_code& ec,
		std::size_t bytes_transferred)
	{
			if (ec != 0) {
				on_finish(ec);
				return;
			}

			m_sock.shutdown(asio::ip::tcp::socket::shutdown_send);

			std::unique_lock<std::mutex>
				cancel_lock(m_cancel_mux);

			if (m_was_cancelled) {
				cancel_lock.unlock();
				on_finish(boost::system::error_code(
					asio::error::operation_aborted));
				return;
			}

			// Read the status line.
			asio::async_read_until(m_sock,
				m_response.get_response_buf(),
				"\r\n",
				[this](const boost::system::error_code& ec,
				std::size_t bytes_transferred)
			{
				on_status_line_received(ec, bytes_transferred);
			});
		}
	
	void on_status_line_received(
		const boost::system::error_code& ec,
		std::size_t bytes_transferred)
	{
			if (ec != 0) {
				on_finish(ec);
				return;
			}

			// Parse the status line.
			std::string http_version;
			std::string str_status_code;
			std::string status_message;

			std::istream response_stream(
				&m_response.get_response_buf());
			response_stream >> http_version;

			if (http_version != "HTTP/1.1"){
				// Response is incorrect.
				on_finish(http_errors::invalid_response);
				return;
			}

			response_stream >> str_status_code;

			// Convert status code to integer.
			unsigned int status_code = 200;

			try {
				status_code = std::stoul(str_status_code);
			}
			catch (std::logic_error&) {
				// Response is incorrect.
				on_finish(http_errors::invalid_response);
				return;
			}

			std::getline(response_stream, status_message, '\r');
			// Remove symbol '\n' from the buffer.
			response_stream.get();

			m_response.set_status_code(status_code);
			m_response.set_status_message(status_message);

			std::unique_lock<std::mutex>
				cancel_lock(m_cancel_mux);

			if (m_was_cancelled) {
				cancel_lock.unlock();
				on_finish(boost::system::error_code(
					asio::error::operation_aborted));
				return;
			}

			// At this point the status line is successfully
			// received and parsed.
			// Now read the response headers.
			asio::async_read_until(m_sock,
				m_response.get_response_buf(),
				"\r\n\r\n",
				[this](
				const boost::system::error_code& ec,
				std::size_t bytes_transferred)
			{
				on_headers_received(ec,
					bytes_transferred);
			});
		}
	
	void on_headers_received(const boost::system::error_code& ec,
		std::size_t bytes_transferred)
	{
			if (ec != 0) {
				on_finish(ec);
				return;
			}

			// Parse and store headers.
			std::string header, header_name, header_value;
			std::istream response_stream(
				&m_response.get_response_buf());

			while (true) {
				std::getline(response_stream, header, '\r');

				// Remove \n symbol from the stream.
				response_stream.get();

				if (header == "")
					break;

				size_t separator_pos = header.find(':');
				if (separator_pos != std::string::npos) {
					header_name = header.substr(0,
						separator_pos);

					if (separator_pos < header.length() - 1)
						header_value =
						header.substr(separator_pos + 1);
					else
						header_value = "";

					m_response.add_header(header_name,
						header_value);
				}
			}

			std::unique_lock<std::mutex>
				cancel_lock(m_cancel_mux);

			if (m_was_cancelled) {
				cancel_lock.unlock();
				on_finish(boost::system::error_code(
					asio::error::operation_aborted));
				return;
			}

			// Now we want to read the response body.
			asio::async_read(m_sock,
				m_response.get_response_buf(),
				[this](
				const boost::system::error_code& ec,
				std::size_t bytes_transferred)
			{
				on_response_body_received(ec,
					bytes_transferred);
			});

			return;
		}
	
	void on_response_body_received(
		const boost::system::error_code& ec,
		std::size_t bytes_transferred)
	{
			if (ec == asio::error::eof)
				on_finish(boost::system::error_code());
			else
				on_finish(ec);
		}
	
	void on_finish(const boost::system::error_code& ec)
	{
			if (ec != 0) {
				std::cout << "Error occured! Error code = "
					<< ec.value()
					<< ". Message: " << ec.message();
			}

			m_callback(*this, m_response, ec);

			return;
		}

private:
	// Request paramters. 
	std::string m_host;
	unsigned int m_port;
	std::string m_uri;

	// Object unique identifier. 
	unsigned int m_id;

	// Callback to be called when request completes. 
	Callback m_callback;

	// Buffer containing the request line.
	std::string m_request_buf;

	asio::ip::tcp::socket m_sock;
	asio::ip::tcp::resolver m_resolver;

	HTTPResponse m_response;

	bool m_was_cancelled;
	std::mutex m_cancel_mux;

	asio::io_service& m_ios;
};

class HTTPClient {
public:
	HTTPClient(){
		m_work.reset(new boost::asio::io_service::work(m_ios));

		m_thread.reset(new std::thread([this](){
			m_ios.run();
		}));
	}

	std::shared_ptr<HTTPRequest>
		create_request(unsigned int id)
	{
			return std::shared_ptr<HTTPRequest>(
				new HTTPRequest(m_ios, id));
		}

	void close() {
		// Destroy the work object. 
		m_work.reset(NULL);

		// Waiting for the I/O thread to exit.
		m_thread->join();
	}

private:
	asio::io_service m_ios;
	std::unique_ptr<boost::asio::io_service::work> m_work;
	std::unique_ptr<std::thread> m_thread;
};

void handler(const HTTPRequest& request,
const HTTPResponse& response,
const system::error_code& ec)
{
	if (ec == 0) {
		std::cout << "Request #" << request.get_id()
			<< " has completed. Response: "
			<< response.get_response().rdbuf();
	}
	else if (ec == asio::error::operation_aborted) {
		std::cout << "Request #" << request.get_id()
			<< " has been cancelled by the user."
			<< std::endl;
	}
	else {
		std::cout << "Request #" << request.get_id()
			<< " failed! Error code = " << ec.value()
			<< ". Error message = " << ec.message()
			<< std::endl;
	}

	return;
}

int main()
{
	try {
		HTTPClient client;

		std::shared_ptr<HTTPRequest> request_one =
			client.create_request(1);

		request_one->set_host("localhost");
		request_one->set_uri("/index.html");
		request_one->set_port(3333);
		request_one->set_callback(handler);

		request_one->execute();

		std::shared_ptr<HTTPRequest> request_two =
			client.create_request(1);

		request_two->set_host("localhost");
		request_two->set_uri("/example.html");
		request_two->set_port(3333);
		request_two->set_callback(handler);

		request_two->execute();

		request_two->cancel();

		// Do nothing for 15 seconds, letting the
		// request complete.
		std::this_thread::sleep_for(std::chrono::seconds(15));

		// Closing the client and exiting the application.
		client.close();
	}
	catch (system::system_error &e) {
		std::cout << "Error occured! Error code = " << e.code()
			<< ". Message: " << e.what();

		return e.code().value();
	}

	return 0;
};
