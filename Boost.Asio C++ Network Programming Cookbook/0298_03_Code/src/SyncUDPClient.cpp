#include <boost/asio.hpp>
#include <iostream>

using namespace boost;

class SyncUDPClient {
public:
	SyncUDPClient() :
		m_sock(m_ios) {

		m_sock.open(asio::ip::udp::v4());
	}

	std::string emulateLongComputationOp(
		unsigned int duration_sec,
		const std::string& raw_ip_address,
		unsigned short port_num) {

		std::string request = "EMULATE_LONG_COMP_OP "
			+ std::to_string(duration_sec)
			+ "\n";

		asio::ip::udp::endpoint ep(
			asio::ip::address::from_string(raw_ip_address),
			port_num);

		sendRequest(ep, request);
		return receiveResponse(ep);
	};

private:
	void sendRequest(const asio::ip::udp::endpoint& ep,
		const std::string& request) {

		m_sock.send_to(asio::buffer(request), ep);
	}

	std::string receiveResponse(asio::ip::udp::endpoint& ep) {
		char response[6];
		std::size_t bytes_recieved =
			m_sock.receive_from(asio::buffer(response), ep);

		m_sock.shutdown(asio::ip::udp::socket::shutdown_both);
		return std::string(response, bytes_recieved);
	}

private:
	asio::io_service m_ios;

	asio::ip::udp::socket m_sock;
};

int main()
{
	const std::string server1_raw_ip_address = "127.0.0.1";
	const unsigned short server1_port_num = 3333;

	const std::string server2_raw_ip_address = "192.168.1.10";
	const unsigned short server2_port_num = 3334;

	try {
		SyncUDPClient client;

		std::cout << "Sending request to the server #1 ... "
			<< std::endl;

		std::string response =
			client.emulateLongComputationOp(10,
			server1_raw_ip_address, server1_port_num);

		std::cout << "Response from the serever #1 received: "
			<< response << std::endl;

		std::cout << "Sending request to the server #2... "
			<< std::endl;

		response =
			client.emulateLongComputationOp(10,
			server2_raw_ip_address, server2_port_num);

		std::cout << "Response from the server #2 received: "
			<< response << std::endl;
	}
	catch (system::system_error &e) {
		std::cout << "Error occured! Error code = " << e.code()
			<< ". Message: " << e.what();

		return e.code().value();
	}

	return 0;
}