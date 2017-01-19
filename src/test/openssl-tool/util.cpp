#include "util.h"
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <sstream>



void cactus::hex_print_buffer(byte* pdata, size_t len )
{
	for (size_t i = 0; i < len; i++) {
		printf("%02x", *pdata);

		if (i % 40 == 39) {
			putchar('\n');
		}

		pdata++;
	}
}

void cactus::print_bytes( byte* data, size_t length )
{
	size_t j = 0;
	while (j < length)
	{
		for ( unsigned int i = 0; i < 8; ++i, ++j )
		{
			std::cout << std::setfill( '0' ) << std::setw( 2 ) << std::hex << ( int )data[j] << " ";
		}
		std::cout << std::endl;
	}
}

std::string cactus::encode_hex(const std::vector<byte>& bs )
{
	std::stringstream stream;
	std::vector<byte>::const_iterator it;
	for(it = bs.begin(); it != bs.end(); ++it) {
		stream << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(*it);
	}
	return stream.str();
}

std::vector<byte> cactus::decode_hex( const std::string& s )
{
	std::vector<byte> ret_val;
	ret_val.reserve(s.length() / 2);

	for(size_t i = 0 ; i < s.length()/2 ; i++) {
		byte data;
		sscanf(&s[2*i], "%2hhx", &data);
		ret_val.push_back(data);
	}

	return ret_val;
}
