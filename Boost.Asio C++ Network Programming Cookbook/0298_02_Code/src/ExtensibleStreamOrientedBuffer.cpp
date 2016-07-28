#include <boost/asio.hpp>
#include <iostream>

using namespace boost;

int main()
{
	asio::streambuf buf;

	std::ostream output(&buf);

	// Writing the message to the stream-based buffer.
	output << "Message1\nMessage2";

	// Now we want to read all data from a streambuf
	// until '\n' delimiter.
	// Instantiate an intput stream which uses our 
	// stream buffer.
	std::istream input(&buf);

	// We'll read data into this string.
	std::string message1;

	std::getline(input, message1);

	// Now message1 string contains 'Message1'.

	return 0;
}
