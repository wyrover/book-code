#ifndef util_h__
#define util_h__


#include <vector>

#ifndef _BYTE
#define _BYTE
typedef unsigned char byte;
#endif

struct buffer {
	size_t length;
	byte bytes[1];

	byte& operator[] (size_t);
};


namespace cactus
{

	void hex_print_buffer(byte* pdata, size_t len);

	void print_bytes(byte* data, size_t length );



	std::string encode_hex(const std::vector<byte>& bs);

	std::vector<byte> decode_hex(const std::string& s);

}


#endif // util_h__
