#ifndef aes_string_h__
#define aes_string_h__


#include <string>
#include <vector>

#ifndef _BYTE
#define _BYTE
typedef unsigned char byte;
#endif


std::string AES_Encrypt_String(std::string const& data, std::string const& key, std::vector<byte> const& iVec);
std::string AES_Decrypt_String(std::string const& data, std::string const& key, std::vector<byte> const& iVec);

int aes_encrypt_bytes(const std::vector<byte>& key, const std::vector<byte>& iv, std::vector<byte>& inData, std::vector<byte>& outData);

int aes_decrypt_bytes(const std::vector<byte>& key, const std::vector<byte>& iv, std::vector<byte>& inData, std::vector<byte>& outData);

#endif // aes_string_h__
