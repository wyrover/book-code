/*!
* \file cactus_rsa.h
* \brief 常用加密模块封装 
* 
* 在cryptopp基础上封装了常用函数, 提供了文件md5, 文件sha256, 文件base64, 字符串base64
* 字符串aes不同模式实现, rsa密钥对生成, rsa文件签名
* 
* \author wangyang 
* \date 2015/04/15 
* \version 1.0
*/


#ifndef __CACTUS_CACTUS_RSA_H__
#define __CACTUS_CACTUS_RSA_H__

#include <string>

typedef unsigned char byte;	

namespace cactus
{

std::string md5_file(std::string filename);
std::string sha256_file(std::string filename);
std::string sha256_string(std::string message);
std::string base64_encode_string(std::string message);
std::string base64_decode_string(std::string message);
bool base64_encode_file(std::string source_filename, std::string dest_filename);
bool base64_decode_file(std::string source_filename, std::string dest_filename);
std::string aes_ecb_encrypt_str(std::string key, const char *plainText);
std::string aes_ecb_decrypt_str(std::string key, const char *cipherText);
std::string aes_cbc_encrypt_str(std::string key, std::string sIV, const char *plainText);
std::string aes_cbc_decrypt_str(std::string key, std::string sIV, const char *cipherText);
std::string aes_cbc_cts_encrypt_str(std::string key, std::string sIV, const char *plainText);
std::string aes_cbc_cts_decrypt_str(std::string key, std::string sIV, const char *cipherText);
std::string aes_cfb_encrypt_str(std::string key, std::string sIV, const char *plainText);
std::string aes_cfb_decrypt_str(std::string key, std::string sIV, const char *cipherText);
std::string aes_ofb_encrypt_str(std::string key, std::string sIV, const char *plainText);
std::string aes_ofb_decrypt_str(std::string key, std::string sIV, const char *cipherText);
std::string aes_ctr_encrypt_str(std::string key, std::string sIV, const char *plainText);
std::string aes_ctr_decrypt_str(std::string key, std::string sIV, const char *cipherText);

bool aes_make_key_file(std::string key_filename);
bool aes_make_key_and_iv(std::string key_filename);

bool aes_ecb_encrypt_file(std::string source_filename, std::string dest_filename, byte* key_block, int key_block_length);
bool aes_ecb_decrypt_file(std::string source_filename, std::string dest_filename, byte* key_block, int key_block_length);

bool aes_cbc_encrypt_file(std::string source_filename, std::string dest_filename, byte* key_block, int key_block_length, byte* iv_block, int iv_block_length);
bool aes_cbc_decrypt_file(std::string source_filename, std::string dest_filename, byte* key_block, int key_block_length, byte* iv_block, int iv_block_length);

bool aes_cbc_encrypt_file(std::string source_filename, std::string dest_filename, std::string key, std::string iv, bool key_iv_base64 = false);

bool aes_cbc_decrypt_file(std::string source_filename, std::string dest_filename, std::string key, std::string iv, bool key_iv_base64 = false);





class RSA_TEST
{
public:
	RSA_TEST();
    ~RSA_TEST();
public:    
    void generate_rsa_key(int bits);
    void sign_file(std::string private_keyfile, std::string source_file, std::string dest_file);
    bool verify_file(std::string public_keyfile, std::string source_file, std::string dest_file);
private:

};

} // namespace cactus

#endif // __CACTUS_CACTUS_RSA_H__
