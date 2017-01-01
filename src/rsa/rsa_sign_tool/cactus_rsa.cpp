/*!
* \file cactus_rsa.cpp
* \brief 常用加密模块封装
*
* 在cryptopp基础上封装了常用函数, 提供了文件md5, 文件sha256, 文件base64, 字符串base64
* 字符串aes不同模式实现, rsa密钥对生成, rsa文件签名
*
* \author wangyang
* \date 2015/04/15
* \version 1.0
*/


#include "cactus_rsa.h"

#include <rsa.h>
#include <osrng.h>     // PRNG
#include <hex.h>       // Hex Encoder/Decoder
#include <files.h>     // File Source and Sink
#include <filters.h>
#include <base64.h>
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <md5.h>
#include <modes.h>
#include <fstream>


using namespace CryptoPP;

namespace cactus
{

RSA_TEST::RSA_TEST()
{
}

RSA_TEST::~RSA_TEST()
{
}

void RSA_TEST::generate_rsa_key(int bits)
{
    try {
        CryptoPP::AutoSeededRandomPool rng;
        // Specify 512 bit modulus, accept e = 17
		// 1024
		// 2048
        CryptoPP::RSAES_OAEP_SHA_Decryptor Decryptor(rng, bits);
        CryptoPP::Base64Encoder private_file_sink(new CryptoPP::FileSink("base64_private_key.txt"), false); // Hex Encoder
        Decryptor.DEREncode(private_file_sink);
        private_file_sink.MessageEnd();

        CryptoPP:: RSAES_OAEP_SHA_Encryptor Encryptor(Decryptor);
        CryptoPP::Base64Encoder public_file_sink(new CryptoPP::FileSink("base64_public_key.txt"), false); // Hex Encoder
        Encryptor.DEREncode(public_file_sink);
        public_file_sink.MessageEnd();

		
		CryptoPP::HexEncoder hex_private_file_sink(new CryptoPP::FileSink("hex_private_key.txt", true /*binary*/));
		Decryptor.DEREncode(hex_private_file_sink);
		hex_private_file_sink.MessageEnd();

		CryptoPP::HexEncoder hex_public_file_sink(new CryptoPP::FileSink("hex_public_key.txt", true /*binary*/));
		Encryptor.DEREncode(hex_public_file_sink);
		hex_private_file_sink.MessageEnd();


		Decryptor.DEREncode(FileSink("private_key.der", true /*binary*/));
		Encryptor.DEREncode(FileSink("public_key.der", true /*binary*/));



		std::string data1 = "{ 0x";
		std::string data2 = "{ 0x";
		

		CryptoPP::HexEncoder hex_private_string(new StringSink(data1), true, 2, ", 0x", " }");
		Decryptor.DEREncode(hex_private_string);
		hex_private_string.MessageEnd();

		CryptoPP::HexEncoder hex_public_string(new StringSink(data2), true, 2, ", 0x", " }");
		Decryptor.DEREncode(hex_public_string);
		hex_public_string.MessageEnd();
	
		std::fstream file;
		file.open("key_pairs.txt", std::ios::out | std::ios::binary);		

		
		file << "byte prv_key[] = " << '\n';
		file << data1 << ";" << '\n';
		file << "byte pub_key[] = " << '\n';
		file << data2 << ";" << '\n';
		/*file << "--------------------------------------------------------------------------------------------------" << '\n';
		file << "hex encode key and iv:" << '\n';
		file << key2 << '\n';
		file << iv2 << '\n';
		file << "--------------------------------------------------------------------------------------------------" << '\n';
		file << "base64 encode key and iv:" << '\n';
		file << key3 << '\n';
		file << iv3 << '\n';*/

		
		file.close();
		

    } catch (CryptoPP::Exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    } catch (std::string& s) {
        std::cerr << "Error: " << s << std::endl;
    } catch (...) {
        std::cerr << "Unknown Error" << std::endl;
    }
}

void RSA_TEST::sign_file(std::string private_keyfile, std::string source_file, std::string dest_file)
{
    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::FileSource priFile(private_keyfile.c_str(), true, new CryptoPP::Base64Decoder);
    CryptoPP::RSASSA_PKCS1v15_SHA_Signer singer(priFile);
    CryptoPP::FileSource f(source_file.c_str(), true,
                           new CryptoPP::SignerFilter(rng, singer,
                                   new CryptoPP::HexEncoder(new CryptoPP::FileSink(dest_file.c_str()))));
}

bool RSA_TEST::verify_file(std::string public_keyfile, std::string source_file, std::string dest_file)
{
    //CryptoPP::FileSource pubFile(public_keyfile.c_str(), true, new CryptoPP::Base64Decoder);
    //Read public key
    CryptoPP::ByteQueue bytes;
    CryptoPP::FileSource file(public_keyfile.c_str(), true, new CryptoPP::Base64Decoder);
    file.TransferTo(bytes);
    bytes.MessageEnd();
    CryptoPP::RSA::PublicKey pubKey;
    pubKey.Load(bytes);
    CryptoPP::RSASSA_PKCS1v15_SHA_Verifier verifier(pubKey);
    //CryptoPP::RSASSA_PKCS1v15_SHA_Verifier verifier(pubFile);
    CryptoPP::FileSource signatureFile(source_file.c_str(), true, new CryptoPP::HexDecoder);

    if (signatureFile.MaxRetrievable() != verifier.SignatureLength())
        return false;

    CryptoPP::SecByteBlock signature(verifier.SignatureLength());
    signatureFile.Get(signature, signature.size());
    CryptoPP::VerifierFilter *verifierFilter = new CryptoPP::VerifierFilter(verifier);
    verifierFilter->Put(signature, verifier.SignatureLength());
    CryptoPP::FileSource f(dest_file.c_str(), true, verifierFilter);
    return verifierFilter->GetLastResult();
}

std::string md5_file(std::string filename)
{
    CryptoPP::Weak::MD5 md;
    const size_t size = CryptoPP::Weak::MD5::DIGESTSIZE * 2;
    byte buf[size] = {0};
    CryptoPP::ArraySink* array_sink = new CryptoPP::ArraySink(buf, size);
    CryptoPP::HexEncoder* hex_encoder = new CryptoPP::HexEncoder(array_sink);
    CryptoPP::HashFilter* hash_filter = new CryptoPP::HashFilter(md, hex_encoder);
    CryptoPP::FileSource(filename.c_str(), true, hash_filter);
    return std::string(reinterpret_cast<const char*>(buf), size);
}

std::string sha256_file(std::string filename)
{
    std::string data;
    CryptoPP::SHA256 hash;
    CryptoPP::FileSource(filename.c_str(), true, new CryptoPP::HashFilter(hash, new CryptoPP::HexEncoder(new CryptoPP::StringSink(data))));
    return data;
}

std::string sha256_string(std::string message)
{
    std::string data;
    CryptoPP::SHA256 hash;
    CryptoPP::StringSource foo(message, true, new CryptoPP::HashFilter(hash, new CryptoPP::HexEncoder(new CryptoPP::StringSink(data))));
    return data;
}

std::string base64_encode_string(std::string message)
{
    std::string data;
    CryptoPP::StringSource(message, true, new CryptoPP::Base64Encoder(new CryptoPP::StringSink(data), false));
    return data;
}

std::string base64_decode_string(std::string message)
{
    std::string data;
    CryptoPP::StringSource(message, true, new CryptoPP::Base64Decoder(new CryptoPP::StringSink(data)));
    return data;
}

bool base64_encode_file(std::string source_filename, std::string dest_filename)
{
    bool ret = false;

    try {
        CryptoPP::FileSink* outfile = new CryptoPP::FileSink(dest_filename.c_str());
        CryptoPP::FileSource(source_filename.c_str(), true, new CryptoPP::Base64Encoder(outfile, false));
        ret = true;
    } catch (...) {
        ret = false;
    }

    return ret;
}

bool base64_decode_file(std::string source_filename, std::string dest_filename)
{
    bool ret = false;

    try {
        CryptoPP::FileSink* outfile = new CryptoPP::FileSink(dest_filename.c_str());
        CryptoPP::FileSource(source_filename.c_str(), true, new CryptoPP::Base64Decoder(outfile));
        ret = true;
    } catch (...) {
        ret = false;
    }

    return ret;
}


std::string aes_ecb_encrypt_str(std::string key, const char *plainText)
{
    std::string data;
    //填key
    SecByteBlock key_block(AES::MAX_KEYLENGTH);
    memset(key_block, 0x30, key_block.size());
    key.size() <= AES::MAX_KEYLENGTH ? memcpy(key_block, key.c_str(), key.size()) : memcpy(key_block, key.c_str(), AES::MAX_KEYLENGTH);
    AES::Encryption aesEncryption((byte *)key_block, AES::MAX_KEYLENGTH);
    ECB_Mode_ExternalCipher::Encryption ecbEncryption(aesEncryption);
    StreamTransformationFilter ecbEncryptor(ecbEncryption, new HexEncoder(new StringSink(data)));
    ecbEncryptor.Put((byte *)plainText, strlen(plainText));
    ecbEncryptor.MessageEnd();
    return data;
}

std::string aes_ecb_decrypt_str(std::string key, const char *cipherText)
{
    std::string data;
    //填key
    SecByteBlock key_block(AES::MAX_KEYLENGTH);
    memset(key_block, 0x30, key_block.size());
    key.size() <= AES::MAX_KEYLENGTH ? memcpy(key_block, key.c_str(), key.size()) : memcpy(key_block, key.c_str(), AES::MAX_KEYLENGTH);
    ECB_Mode<AES >::Decryption ecbDecryption((byte *)key_block, AES::MAX_KEYLENGTH);
    HexDecoder decryptor(new StreamTransformationFilter(ecbDecryption, new StringSink(data)));
    decryptor.Put((byte *)cipherText, strlen(cipherText));
    decryptor.MessageEnd();
    return data;
}

std::string aes_cbc_encrypt_str(std::string key, std::string sIV, const char *plainText)
{
    std::string data;
    //填key
    SecByteBlock key_block(AES::MAX_KEYLENGTH);
    memset(key_block, 0x30, key_block.size());
    key.size() <= AES::MAX_KEYLENGTH ? memcpy(key_block, key.c_str(), key.size()) : memcpy(key_block, key.c_str(), AES::MAX_KEYLENGTH);
    //填iv
    byte iv[AES::BLOCKSIZE];
    memset(iv, 0x30, AES::BLOCKSIZE);
    sIV.size() <= AES::BLOCKSIZE ? memcpy(iv, sIV.c_str(), sIV.size()) : memcpy(iv, sIV.c_str(), AES::BLOCKSIZE);
    AES::Encryption aesEncryption((byte *)key_block, AES::MAX_KEYLENGTH);
    CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, iv);
    StreamTransformationFilter cbcEncryptor(cbcEncryption, new HexEncoder(new StringSink(data)));
    cbcEncryptor.Put((byte *)plainText, strlen(plainText));
    cbcEncryptor.MessageEnd();
    return data;
}

std::string aes_cbc_decrypt_str(std::string key, std::string sIV, const char *cipherText)
{
    std::string data;
    //填key
    SecByteBlock key_block(AES::MAX_KEYLENGTH);
    memset(key_block, 0x30, key_block.size());
    key.size() <= AES::MAX_KEYLENGTH ? memcpy(key_block, key.c_str(), key.size()) : memcpy(key_block, key.c_str(), AES::MAX_KEYLENGTH);
    //填iv
    byte iv[AES::BLOCKSIZE];
    memset(iv, 0x30, AES::BLOCKSIZE);
    sIV.size() <= AES::BLOCKSIZE ? memcpy(iv, sIV.c_str(), sIV.size()) : memcpy(iv, sIV.c_str(), AES::BLOCKSIZE);
    CBC_Mode<AES >::Decryption cbcDecryption((byte *)key_block, AES::MAX_KEYLENGTH, iv);
    HexDecoder decryptor(new StreamTransformationFilter(cbcDecryption, new StringSink(data)));
    decryptor.Put((byte *)cipherText, strlen(cipherText));
    decryptor.MessageEnd();
    return data;
}

std::string aes_cbc_cts_encrypt_str(std::string key, std::string sIV, const char *plainText)
{
    std::string data;
    //填key
    SecByteBlock key_block(AES::MAX_KEYLENGTH);
    memset(key_block, 0x30, key_block.size());
    key.size() <= AES::MAX_KEYLENGTH ? memcpy(key_block, key.c_str(), key.size()) : memcpy(key_block, key.c_str(), AES::MAX_KEYLENGTH);
    //填iv
    byte iv[AES::BLOCKSIZE];
    memset(iv, 0x30, AES::BLOCKSIZE);
    sIV.size() <= AES::BLOCKSIZE ? memcpy(iv, sIV.c_str(), sIV.size()) : memcpy(iv, sIV.c_str(), AES::BLOCKSIZE);
    AES::Encryption aesEncryption((byte *)key_block, AES::MAX_KEYLENGTH);
    CBC_CTS_Mode_ExternalCipher::Encryption cbcctsEncryption(aesEncryption, iv);
    StreamTransformationFilter cbcctsEncryptor(cbcctsEncryption, new HexEncoder(new StringSink(data)));
    cbcctsEncryptor.Put((byte *)plainText, strlen(plainText));
    cbcctsEncryptor.MessageEnd();
    return data;
}

std::string aes_cbc_cts_decrypt_str(std::string key, std::string sIV, const char *cipherText)
{
    std::string data;
    SecByteBlock key_block(AES::MAX_KEYLENGTH);
    memset(key_block, 0x30, key_block.size());
    key.size() <= AES::MAX_KEYLENGTH ? memcpy(key_block, key.c_str(), key.size()) : memcpy(key_block, key.c_str(), AES::MAX_KEYLENGTH);
    byte iv[AES::BLOCKSIZE];
    memset(iv, 0x30, AES::BLOCKSIZE);
    sIV.size() <= AES::BLOCKSIZE ? memcpy(iv, sIV.c_str(), sIV.size()) : memcpy(iv, sIV.c_str(), AES::BLOCKSIZE);
    CBC_CTS_Mode<AES >::Decryption cbcctsDecryption((byte *)key_block, AES::MAX_KEYLENGTH, iv);
    HexDecoder decryptor(new StreamTransformationFilter(cbcctsDecryption, new StringSink(data)));
    decryptor.Put((byte *)cipherText, strlen(cipherText));
    decryptor.MessageEnd();
    return data;
}

std::string aes_cfb_encrypt_str(std::string key, std::string sIV, const char *plainText)
{
    std::string data;
    //填key
    SecByteBlock key_block(AES::MAX_KEYLENGTH);
    memset(key_block, 0x30, key_block.size());
    key.size() <= AES::MAX_KEYLENGTH ? memcpy(key_block, key.c_str(), key.size()) : memcpy(key_block, key.c_str(), AES::MAX_KEYLENGTH);
    //填iv
    byte iv[AES::BLOCKSIZE];
    memset(iv, 0x30, AES::BLOCKSIZE);
    sIV.size() <= AES::BLOCKSIZE ? memcpy(iv, sIV.c_str(), sIV.size()) : memcpy(iv, sIV.c_str(), AES::BLOCKSIZE);
    AES::Encryption aesEncryption((byte *)key_block, AES::MAX_KEYLENGTH);
    CFB_Mode_ExternalCipher::Encryption cfbEncryption(aesEncryption, iv);
    StreamTransformationFilter cfbEncryptor(cfbEncryption, new HexEncoder(new StringSink(data)));
    cfbEncryptor.Put((byte *)plainText, strlen(plainText));
    cfbEncryptor.MessageEnd();
    return data;
}

std::string aes_cfb_decrypt_str(std::string key, std::string sIV, const char *cipherText)
{
    std::string data;
    //填key
    SecByteBlock key_block(AES::MAX_KEYLENGTH);
    memset(key_block, 0x30, key_block.size());
    key.size() <= AES::MAX_KEYLENGTH ? memcpy(key_block, key.c_str(), key.size()) : memcpy(key_block, key.c_str(), AES::MAX_KEYLENGTH);
    //填iv
    byte iv[AES::BLOCKSIZE];
    memset(iv, 0x30, AES::BLOCKSIZE);
    sIV.size() <= AES::BLOCKSIZE ? memcpy(iv, sIV.c_str(), sIV.size()) : memcpy(iv, sIV.c_str(), AES::BLOCKSIZE);
    CFB_Mode<AES >::Decryption cfbDecryption((byte *)key_block, AES::MAX_KEYLENGTH, iv);
    HexDecoder decryptor(new StreamTransformationFilter(cfbDecryption, new StringSink(data)));
    decryptor.Put((byte *)cipherText, strlen(cipherText));
    decryptor.MessageEnd();
    return data;
}

std::string aes_ofb_encrypt_str(std::string key, std::string sIV, const char *plainText)
{
    std::string data;
    //填key
    SecByteBlock key_block(AES::MAX_KEYLENGTH);
    memset(key_block, 0x30, key_block.size());
    key.size() <= AES::MAX_KEYLENGTH ? memcpy(key_block, key.c_str(), key.size()) : memcpy(key_block, key.c_str(), AES::MAX_KEYLENGTH);
    //填iv
    byte iv[AES::BLOCKSIZE];
    memset(iv, 0x30, AES::BLOCKSIZE);
    sIV.size() <= AES::BLOCKSIZE ? memcpy(iv, sIV.c_str(), sIV.size()) : memcpy(iv, sIV.c_str(), AES::BLOCKSIZE);
    AES::Encryption aesEncryption((byte *)key_block, AES::MAX_KEYLENGTH);
    OFB_Mode_ExternalCipher::Encryption ofbEncryption(aesEncryption, iv);
    StreamTransformationFilter ofbEncryptor(ofbEncryption, new HexEncoder(new StringSink(data)));
    ofbEncryptor.Put((byte *)plainText, strlen(plainText));
    ofbEncryptor.MessageEnd();
    return data;
}


std::string aes_ofb_decrypt_str(std::string key, std::string sIV, const char *cipherText)
{
    std::string data;
    //填key
    SecByteBlock key_block(AES::MAX_KEYLENGTH);
    memset(key_block, 0x30, key_block.size());
    key.size() <= AES::MAX_KEYLENGTH ? memcpy(key_block, key.c_str(), key.size()) : memcpy(key_block, key.c_str(), AES::MAX_KEYLENGTH);
    //填iv
    byte iv[AES::BLOCKSIZE];
    memset(iv, 0x30, AES::BLOCKSIZE);
    sIV.size() <= AES::BLOCKSIZE ? memcpy(iv, sIV.c_str(), sIV.size()) : memcpy(iv, sIV.c_str(), AES::BLOCKSIZE);
    OFB_Mode<AES >::Decryption ofbDecryption((byte *)key_block, AES::MAX_KEYLENGTH, iv);
    HexDecoder decryptor(new StreamTransformationFilter(ofbDecryption, new StringSink(data)));
    decryptor.Put((byte *)cipherText, strlen(cipherText));
    decryptor.MessageEnd();
    return data;
}

std::string aes_ctr_encrypt_str(std::string key, std::string sIV, const char *plainText)
{
    std::string data;
    //填key
    SecByteBlock key_block(AES::MAX_KEYLENGTH);
    memset(key_block, 0x30, key_block.size());
    key.size() <= AES::MAX_KEYLENGTH ? memcpy(key_block, key.c_str(), key.size()) : memcpy(key_block, key.c_str(), AES::MAX_KEYLENGTH);
    //填iv
    byte iv[AES::BLOCKSIZE];
    memset(iv, 0x30, AES::BLOCKSIZE);
    sIV.size() <= AES::BLOCKSIZE ? memcpy(iv, sIV.c_str(), sIV.size()) : memcpy(iv, sIV.c_str(), AES::BLOCKSIZE);
    AES::Encryption aesEncryption((byte *)key_block, AES::MAX_KEYLENGTH);
    CTR_Mode_ExternalCipher::Encryption ctrEncryption(aesEncryption, iv);
    StreamTransformationFilter ctrEncryptor(ctrEncryption, new HexEncoder(new StringSink(data)));
    ctrEncryptor.Put((byte *)plainText, strlen(plainText));
    ctrEncryptor.MessageEnd();
    return data;
}


std::string aes_ctr_decrypt_str(std::string key, std::string sIV, const char *cipherText)
{
    std::string data;
    //填key
    SecByteBlock key_block(AES::MAX_KEYLENGTH);
    memset(key_block, 0x30, key_block.size());
    key.size() <= AES::MAX_KEYLENGTH ? memcpy(key_block, key.c_str(), key.size()) : memcpy(key_block, key.c_str(), AES::MAX_KEYLENGTH);
    //填iv
    byte iv[AES::BLOCKSIZE];
    memset(iv, 0x30, AES::BLOCKSIZE);
    sIV.size() <= AES::BLOCKSIZE ? memcpy(iv, sIV.c_str(), sIV.size()) : memcpy(iv, sIV.c_str(), AES::BLOCKSIZE);
    CTR_Mode<AES >::Decryption ctrDecryption((byte *)key_block, AES::MAX_KEYLENGTH, iv);
    HexDecoder decryptor(new StreamTransformationFilter(ctrDecryption, new StringSink(data)));
    decryptor.Put((byte *)cipherText, strlen(cipherText));
    decryptor.MessageEnd();
    return data;
}

bool aes_make_key_file(std::string key_filename)
{
    bool ret = false;

    try {
        AutoSeededRandomPool prng;
        byte key[AES::DEFAULT_KEYLENGTH];
        prng.GenerateBlock(key, sizeof(key));
        StringSource(key, sizeof(key), true,
                     new HexEncoder(
                         new FileSink(key_filename.c_str())
                     ) // HexEncoder
                    ); // StringSource
        ret = true;
    } catch (const CryptoPP::Exception& e) {
        ret = false;
    }

    return ret;
}

bool get_random_block(int length, std::string& result1, std::string& result2, std::string& result3)
{
    bool ret = false;

    try {
        std::string data1 = "{ 0x";
        std::string data2 = "";
        std::string data3 = "";
        AutoSeededRandomPool prng;
        //byte key_block[length];
        std::vector<byte> key_block(length);
        prng.GenerateBlock(&key_block[0], sizeof(byte) * length);
        StringSource(&key_block[0], sizeof(byte) * length, true,
                     new HexEncoder(
                         new StringSink(data1), true, 2, ", 0x", " }"
                     ) // HexEncoder
                    ); // StringSource
        StringSource(&key_block[0], sizeof(byte) * length, true,
                     new HexEncoder(
                         new StringSink(data2), true
                     ) // HexEncoder
                    ); // StringSource
        StringSource(&key_block[0], sizeof(byte) * length, true,
                     new Base64Encoder(
                         new StringSink(data3), false
                     ) // HexEncoder
                    ); // StringSource
        result1 = data1;
        result2 = data2;
        result3 = data3;
        ret = true;
    } catch (const CryptoPP::Exception& e) {
    }

    return ret;
}

bool aes_make_key_and_iv(std::string key_filename)
{
    bool ret = false;
    std::string key1, key2, key3;
    std::string iv1, iv2, iv3;

    do {
        if (!get_random_block(CryptoPP::AES::DEFAULT_KEYLENGTH, key1, key2, key3))
            break;

        if (!get_random_block(CryptoPP::AES::BLOCKSIZE, iv1, iv2, iv3))
            break;

        std::fstream file;
        file.open(key_filename.c_str(), std::ios::out | std::ios::binary);

        if (!file.is_open())
            break;

        file << "key and iv array:" << '\n';
        file << "byte key[16] = " << '\n';
        file << key1 << ";" << '\n';
        file << "byte iv[16] = " << '\n';
        file << iv1 << ";" << '\n';
        file << "--------------------------------------------------------------------------------------------------" << '\n';
        file << "hex encode key and iv:" << '\n';
        file << key2 << '\n';
        file << iv2 << '\n';
        file << "--------------------------------------------------------------------------------------------------" << '\n';
        file << "base64 encode key and iv:" << '\n';
        file << key3 << '\n';
        file << iv3 << '\n';
        ret = true;
    } while (0);

    return ret;
}

bool aes_ecb_encrypt_file(std::string source_filename, std::string dest_filename, byte* key_block, int key_block_length)
{
    bool ret = false;

    try {
        ECB_Mode<AES>::Encryption e(key_block, key_block_length);
        FileSource(source_filename.c_str(), true,
                   new StreamTransformationFilter(e,
                           new FileSink(dest_filename.c_str())
                                                 )
                  );
        ret = true;
    } catch (const CryptoPP::Exception& e) {
    }

    return ret;
}

bool aes_ecb_decrypt_file(std::string source_filename, std::string dest_filename, byte* key_block, int key_block_length)
{
    bool ret = false;

    try {
        ECB_Mode<AES>::Decryption e(key_block, key_block_length);
        FileSource(source_filename.c_str(), true,
                   new StreamTransformationFilter(e,
                           new FileSink(dest_filename.c_str())
                                                 )
                  );
        ret = true;
    } catch (const CryptoPP::Exception& e) {
    }

    return ret;
}

bool aes_cbc_encrypt_file(std::string source_filename, std::string dest_filename, std::string key, std::string iv, bool key_iv_base64)
{
    std::vector<byte> key_block(AES::DEFAULT_KEYLENGTH);
    std::vector<byte> iv_block(AES::BLOCKSIZE);
    StringSource(key.c_str(), true,
                 new CryptoPP::HexDecoder(
                     new CryptoPP::ArraySink(&key_block[0], AES::DEFAULT_KEYLENGTH)
                 )
                );
    StringSource(iv.c_str(), true,
                 new CryptoPP::HexDecoder(
                     new CryptoPP::ArraySink(&iv_block[0], AES::BLOCKSIZE)
                 )
                );
    CBC_Mode<AES>::Encryption e(&key_block[0], key_block.size(), &iv_block[0]);
    FileSource(source_filename.c_str(), true,
               new StreamTransformationFilter(e,
                       new FileSink(dest_filename.c_str())
                                             )
              );
    return true;
}


bool aes_cbc_decrypt_file(std::string source_filename, std::string dest_filename, std::string key, std::string iv, bool key_iv_base64)
{
    bool ret = false;
    std::vector<byte> key_block(AES::DEFAULT_KEYLENGTH);
    std::vector<byte> iv_block(AES::BLOCKSIZE);

    if (key_iv_base64) {
        StringSource(key.c_str(), true,
                     new CryptoPP::Base64Decoder(
                         new CryptoPP::ArraySink(&key_block[0], AES::DEFAULT_KEYLENGTH)
                     )
                    );
        StringSource(iv.c_str(), true,
                     new CryptoPP::Base64Decoder(
                         new CryptoPP::ArraySink(&iv_block[0], AES::BLOCKSIZE)
                     )
                    );
    } else {
        StringSource(key.c_str(), true,
                     new CryptoPP::HexDecoder(
                         new CryptoPP::ArraySink(&key_block[0], AES::DEFAULT_KEYLENGTH)
                     )
                    );
        StringSource(iv.c_str(), true,
                     new CryptoPP::HexDecoder(
                         new CryptoPP::ArraySink(&iv_block[0], AES::BLOCKSIZE)
                     )
                    );
    }

    try {
        CBC_Mode<AES>::Decryption d(&key_block[0], key_block.size(), &iv_block[0]);
        FileSource s(source_filename.c_str(), true,
                     new StreamTransformationFilter(d,
                             new FileSink(dest_filename.c_str())
                                                   )
                    );
        ret = true;
    } catch (const CryptoPP::Exception& e) {
    }

    return ret;
}


bool aes_cbc_encrypt_file(std::string source_filename, std::string dest_filename, byte* key_block, int key_block_length, byte* iv_block, int iv_block_length)
{
    bool ret = false;

    try {
        byte iv[AES::BLOCKSIZE];
        ::memcpy(iv, iv_block, AES::BLOCKSIZE);
        CBC_Mode<AES>::Encryption e(key_block, key_block_length, iv);
        FileSource(source_filename.c_str(), true,
                   new StreamTransformationFilter(e,
                           new FileSink(dest_filename.c_str())
                                                 )
                  );
        ret = true;
    } catch (const CryptoPP::Exception& e) {
    }

    return ret;
}

bool aes_cbc_decrypt_file(std::string source_filename, std::string dest_filename, byte* key_block, int key_block_length, byte* iv_block, int iv_block_length)
{
    bool ret = false;

    try {
        byte iv[AES::BLOCKSIZE];
        ::memcpy(iv, iv_block, AES::BLOCKSIZE);
        CBC_Mode<AES>::Decryption d(key_block, key_block_length, iv);
        FileSource s(source_filename.c_str(), true,
                     new StreamTransformationFilter(d,
                             new FileSink(dest_filename.c_str())
                                                   )
                    );
        ret = true;
    } catch (const CryptoPP::Exception& e) {
    }

    return ret;
}


//bool aes_ecb_encrypt_file(std::string source_filename, std::string dest_filename)
//{
//    bool ret = false;
//    AutoSeededRandomPool prng;
//    byte key[AES::DEFAULT_KEYLENGTH];
//    prng.GenerateBlock(key, sizeof(key));
//    std::string plain = "ECB Mode Test";
//    std::string cipher, encoded, recovered;
//    /*********************************\
//    \*********************************/
//    // Pretty print key
//    encoded.clear();
//    StringSource(key, sizeof(key), true,
//                 new HexEncoder(
//                     new StringSink(encoded)
//                 ) // HexEncoder
//                ); // StringSource
//    std::cout << "key: " << encoded << std::endl;
//
//    try {
//        std::cout << "plain text: " << plain << std::endl;
//        ECB_Mode< AES >::Encryption e;
//        e.SetKey(key, sizeof(key));
//        // The StreamTransformationFilter adds padding
//        //  as required. ECB and CBC Mode must be padded
//        //  to the block size of the cipher.
//        FileSource(source_filename.c_str(), true,
//                   new StreamTransformationFilter(e,
//                           new FileSink(dest_filename.c_str())
//                                                 ) // StreamTransformationFilter
//                  ); // StringSource
//        ret = true;
//    } catch (const CryptoPP::Exception& e) {
//        ret = false;
//    }
//
//    try {
//        ECB_Mode< AES >::Decryption d;
//        d.SetKey(key, sizeof(key));
//        // The StreamTransformationFilter removes
//        //  padding as required.
//        FileSource s(dest_filename.c_str(), true,
//                     new StreamTransformationFilter(d,
//                             new FileSink("test_aes_decrypt.txt")
//                                                   ) // StreamTransformationFilter
//                    ); // StringSource
//        ret = true;
//    } catch (const CryptoPP::Exception& e) {
//        ret = false;
//    }
//
//    return ret;
//}



void load_base64_publickey(const std::string& key_content, RSA::PublicKey& PublicKey)
{

	CryptoPP::Base64Decoder decoder;
	decoder.Put((byte*)key_content.c_str(), key_content.size());
	decoder.MessageEnd();
	PublicKey.Load(decoder);

}

void load_base64_privatekey(const std::string& key_content, RSA::PrivateKey& PrivateKey)
{

	CryptoPP::Base64Decoder decoder;
	decoder.Put((byte*)key_content.c_str(), key_content.size());
	decoder.MessageEnd();
	PrivateKey.Load(decoder);
}

void load_hex_publickey(const std::string& key_content, RSA::PublicKey& PublicKey)
{
	CryptoPP::HexDecoder decoder;
	decoder.Put((byte*)key_content.c_str(), key_content.size());
	decoder.MessageEnd();
	PublicKey.Load(decoder);
}

void load_hex_privatekey(const std::string& key_content, RSA::PrivateKey& PrivateKey)
{
	CryptoPP::HexDecoder decoder;
	decoder.Put((byte*)key_content.c_str(), key_content.size());
	decoder.MessageEnd();
	PrivateKey.Load(decoder);
}


} // namespace cactus