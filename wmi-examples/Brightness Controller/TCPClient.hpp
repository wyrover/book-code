#include <iostream>
#include <string>
#include "shared_const_buffer.h"

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

// Comment the following line to disable debug output
#define DEBUG FALSE

#if(DEBUG == TRUE)
	#define debugOUT(s) std::cout << s << std::endl
#else
	#define debugOUT(s)
#endif

using namespace std;
using boost::asio::ip::tcp;

class TCPClient {
public:
	// Constructor
	TCPClient::TCPClient(boost::asio::io_service& IO_Service, const std::string& server, const std::string& port)
		: resolver_(IO_Service), socket_(IO_Service), ioService(IO_Service){
		
		connected = false;

		// Query donde defino nombre del servidor y puerto
		tcp::resolver::query query(server, port);

		resolver_.async_resolve(query,
			boost::bind(&TCPClient::handle_resolve, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::iterator));

		boost::thread t(boost::bind(&boost::asio::io_service::run, &ioService));

		transferAtLeast = 1;
		receiveBuffer.resize(transferAtLeast);
		readHandler = nullptr;

		debugOUT("Init completed");
	}

private:
	boost::asio::io_service &ioService;
	tcp::resolver	resolver_;
	tcp::socket		socket_;

	std::string	string_;

	std::vector<char> receiveBuffer;
	
	int transferAtLeast;
	std::function<void(std::size_t bytes_transferred, std::vector<char> &receiveBuffer)> readHandler;

	bool connected;

public:
	void Write(const std::string &data){
		debugOUT("Write Method [std::string]");
		string_ = data;

		ioService.post(boost::bind(&TCPClient::DoWrite, this));
	}

	template<typename T>
	void Write(std::vector<T> &data){
		debugOUT("Write Method [std::vector]");

		ioService.post(boost::bind(&TCPClient::DoWrite<T>, this, data));
	}

	bool isConnected(){
		return connected;
	}

	void Close(){
		ioService.post(boost::bind(&TCPClient::DoClose, this));
	}

	/* Agrega un Callback que es llamado cuando se reciben datos.
	   La funcion debe retornar un int indicando el nuevo valor de transferAtLeast o -1 si se continua con el mismo */
	void addReadCallback(std::function<void(std::size_t bytes_transferred, std::vector<char> &receiveBuffer)> f, int transferAtLeast){
		setTransferAtLeast(transferAtLeast);
		readHandler = f;
	}

	void removeReadCallback(){
		readHandler = nullptr;		// En C++11 se usa nullptr y no NULL
	}

	void setTransferAtLeast(int v){
		transferAtLeast = v;
		receiveBuffer.resize(transferAtLeast);
	}

private:

	void DoClose(){
		socket_.close();
	}

	void DoWrite(){
		debugOUT("DoWrite Method [std::string]");
		size_t size = string_.size();

		boost::asio::async_write(	socket_,
									boost::asio::buffer(string_, size),
									boost::bind(&TCPClient::handle_write,
									this, boost::asio::placeholders::error,
									boost::asio::placeholders::bytes_transferred)
								);
		debugOUT("async_write executed");
	}

	template<typename T>
	void DoWrite(std::vector<T> &data){
		debugOUT("DoWrite Method [std::vector]");

		std::shared_ptr<std::vector<T>> a = make_shared<std::vector<T> >(data);
		shared_const_buffer<T> b(a);

		boost::asio::async_write(	socket_,
									b,
									boost::bind(&TCPClient::handle_write,
									this, boost::asio::placeholders::error,
									boost::asio::placeholders::bytes_transferred)
								);
		debugOUT("async_write executed");
	}

	// Resolucion de la direccion del servidor
	void handle_resolve(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator){
		debugOUT("handle_resolve");
		if (!err){
			// Intentamos conectarnos al primer endpoint en la lista
			tcp::endpoint endpoint = *endpoint_iterator;
			socket_.async_connect(	endpoint,
									boost::bind(&TCPClient::handle_connect, this,
									boost::asio::placeholders::error, ++endpoint_iterator));
		}
		else{
			debugOUT("Error: " << err.message());
		}
	}

	// Conexion al servidor
	void handle_connect(const boost::system::error_code& error, tcp::resolver::iterator endpoint_iterator){
		debugOUT("handle_connect");
		if (!error){
			connected = true;
			debugOUT("Connected");

			boost::asio::async_read(socket_,
									boost::asio::buffer(receiveBuffer, transferAtLeast),
									boost::asio::transfer_at_least(transferAtLeast),
									boost::bind(&TCPClient::handle_read, this,
									boost::asio::placeholders::error,
									boost::asio::placeholders::bytes_transferred));

			debugOUT("async_read dispatched");
		}
		else if (endpoint_iterator != tcp::resolver::iterator()){
			debugOUT("Next Endpoint");
			// Conexion fallida, intentemos con el siguiente endpoint
			socket_.close();
			tcp::endpoint endpoint = *endpoint_iterator;
			
			socket_.async_connect(	endpoint,
									boost::bind(&TCPClient::handle_connect, this,
									boost::asio::placeholders::error, ++endpoint_iterator));
		}
		else{
			debugOUT("Error: " << error.message());
		}
	}

	// Datos enviados
	void handle_write(const boost::system::error_code& error, std::size_t bytes_transferred){
		if (!error){
			debugOUT("Handle_write() sent bytes: " << bytes_transferred);
		}
		else {
			debugOUT("Handle_write() error: " << error);
		}
	}

	// Datos recibidos
	void handle_read(const boost::system::error_code& error, std::size_t bytes_transferred){
		if (!error){
			debugOUT("Handle_read() received bytes: " << bytes_transferred);
			debugOUT("Handle_read() data: " << receiveBuffer[0]);

			// Ejecuto el CallBack
			if (readHandler != nullptr)
				readHandler(bytes_transferred, receiveBuffer);

			boost::asio::async_read(	socket_,
										boost::asio::buffer(receiveBuffer, transferAtLeast),
										boost::asio::transfer_at_least(transferAtLeast),
										boost::bind(&TCPClient::handle_read, this,
										boost::asio::placeholders::error,
										boost::asio::placeholders::bytes_transferred));

		}
		else{
			debugOUT("Handle_read() error: " << error);
		}
	}
};