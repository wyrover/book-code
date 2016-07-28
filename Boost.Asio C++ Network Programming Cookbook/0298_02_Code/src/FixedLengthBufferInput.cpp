#include <boost/asio.hpp>
#include <iostream>
#include <memory> // For std::unique_ptr<>

using namespace boost;

int main()
{
	// We expect to receive a block of data no more than 20 bytes 
	// long. 
	const size_t BUF_SIZE_BYTES = 20;

	// Step 1. Allocating the buffer. 
	std::unique_ptr<char[]> buf(new char[BUF_SIZE_BYTES]);

	// Step 2. Creating buffer representation that satisfies 
	// MutableBufferSequence concept requirements.
	asio::mutable_buffers_1 input_buf =
		asio::buffer(static_cast<void*>(buf.get()),
		BUF_SIZE_BYTES);

	// Step 3. 'input_buf' is the representation of the buffer
	// 'buf' that can be used in Boost.Asio input operations.

	return 0;
}