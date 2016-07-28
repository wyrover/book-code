#include <boost/asio.hpp>

using namespace boost;

int main()
{
	// Step 1. Allocate simple buffers.
	char part1[6];
	char part2[3];
	char part3[7];

	// Step 2. Create an object representing a composite buffer.
	std::vector<asio::mutable_buffer> composite_buffer;

	// Step 3. Add simple buffers to the composite buffer object.
	composite_buffer.push_back(asio::mutable_buffer(part1,
		sizeof(part1)));
	composite_buffer.push_back(asio::mutable_buffer(part2,
		sizeof(part2)));
	composite_buffer.push_back(asio::mutable_buffer(part3,
		sizeof(part3)));

	// Now composite_buffer can be used with Boost.Asio 
	// input operation as if it was a simple buffer 
	// represented by contiguous block of memory.

	return 0;
}
