#include "aes_string.h"
#include <openssl/aes.h>

#define BIT_SIZE 128

std::string AES_Encrypt_String( std::string const& data, std::string const& key, std::vector<byte> const& iVec )
{
	if (data.empty() || key.empty())
		return data;

	unsigned char encryptionIvec[AES_BLOCK_SIZE];
	std::memcpy(encryptionIvec, &iVec[0], AES_BLOCK_SIZE);

	AES_KEY AESkey;
	AES_set_encrypt_key((unsigned const char*)key.c_str(), BIT_SIZE, &AESkey);
	unsigned char buffer[AES_BLOCK_SIZE];
	std::string value;

	for (unsigned int i = 0; i < data.size(); i += AES_BLOCK_SIZE)
	{
		AES_cbc_encrypt((unsigned const char*)data.c_str() + i, buffer, ((i + AES_BLOCK_SIZE) < data.size()) ? AES_BLOCK_SIZE : (data.size() - i), &AESkey, encryptionIvec, AES_ENCRYPT);
		value.resize(value.size() + AES_BLOCK_SIZE);
		std::memcpy(&value[i], buffer, AES_BLOCK_SIZE);
	}

	return value;
}

std::string AES_Decrypt_String( std::string const& data, std::string const& key, std::vector<byte> const& iVec )
{
	if (data.empty() || key.empty())
		return data;

	unsigned char decryptionIvec[AES_BLOCK_SIZE];
	std::memcpy(decryptionIvec, &iVec[0], AES_BLOCK_SIZE);

	AES_KEY AESkey;
	AES_set_decrypt_key((unsigned const char*)key.c_str(), BIT_SIZE, &AESkey);
	unsigned char buffer[AES_BLOCK_SIZE];
	std::string value;

	for (unsigned int i = 0; i < data.size(); i += AES_BLOCK_SIZE)
	{
		AES_cbc_encrypt((unsigned const char*)data.c_str() + i, buffer, AES_BLOCK_SIZE, &AESkey, decryptionIvec, AES_DECRYPT);
		value.resize(value.size() + AES_BLOCK_SIZE);
		std::memcpy(&value[i], buffer, AES_BLOCK_SIZE);
	}

	return value;
}



int aes_encrypt_bytes(const std::vector<byte>& key, const std::vector<byte>& iv, std::vector<byte>& inData, std::vector<byte>& outData)
{
	int retval = 0;

	if (key.empty() || iv.empty() || inData.empty())
		return retval;

	byte encryptionIvec[AES_BLOCK_SIZE];
	std::memcpy(encryptionIvec, &iv[0], AES_BLOCK_SIZE);

	AES_KEY AESkey;
	AES_set_encrypt_key(&key[0], BIT_SIZE, &AESkey);
	byte buffer[AES_BLOCK_SIZE];
	

	for (unsigned int i = 0; i < inData.size(); i += AES_BLOCK_SIZE)
	{
		memset(buffer, 0, AES_BLOCK_SIZE);
		AES_cbc_encrypt(&inData[i], buffer, ((i + AES_BLOCK_SIZE) < inData.size()) ? AES_BLOCK_SIZE : (inData.size() - i), &AESkey, encryptionIvec, AES_ENCRYPT);
		outData.resize(outData.size() + AES_BLOCK_SIZE);
		std::memcpy(&outData[i], buffer, AES_BLOCK_SIZE);
	}

	retval = 1;

	return retval;
}

int aes_decrypt_bytes(const std::vector<byte>& key, const std::vector<byte>& iv, std::vector<byte>& inData, std::vector<byte>& outData)
{

	int retval = 0;

	if (key.empty() || iv.empty() || inData.empty())
		return retval;

	byte decryptionIvec[AES_BLOCK_SIZE];
	std::memcpy(decryptionIvec, &iv[0], AES_BLOCK_SIZE);

	AES_KEY AESkey;
	AES_set_decrypt_key(&key[0], BIT_SIZE, &AESkey);
	byte buffer[AES_BLOCK_SIZE];
	

	for (unsigned int i = 0; i < inData.size(); i += AES_BLOCK_SIZE)
	{
		memset(buffer, 0, AES_BLOCK_SIZE);
		AES_cbc_encrypt(&inData[i], buffer, AES_BLOCK_SIZE, &AESkey, decryptionIvec, AES_DECRYPT);
		outData.resize(outData.size() + AES_BLOCK_SIZE);
		std::memcpy(&outData[i], buffer, AES_BLOCK_SIZE);
	}

	retval = 1;

	return retval;
}