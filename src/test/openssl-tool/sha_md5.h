#ifndef sha_md5_h__
#define sha_md5_h__

#include <string>

#ifndef _BYTE
#define _BYTE
typedef unsigned char byte;
#endif

std::string sha_hash(std::string const& data);
std::string sha_hash(const byte* pData, size_t len);


#endif // sha_md5_h__
