// console_template.cpp : Defines the entry point for the console application.
//

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <iostream>
#include <string>

#include <rsa.h>
#include <hex.h>
#include <osrng.h>
#include <files.h>
#include <secblock.h>
#include <sha.h>
#include <filters.h>
#include <base64.h>


using namespace std;
using namespace CryptoPP;




void SaveKey( const RSA::PublicKey& PublicKey, const string& filename );
void SaveKey( const RSA::PrivateKey& PrivateKey, const string& filename );
void LoadKey( const string& filename, RSA::PublicKey& PublicKey );
void LoadKey( const string& filename, RSA::PrivateKey& PrivateKey );

int RSAEncryption(const std::string &pubkey, 
				  const unsigned char *plain, unsigned int plainlen,
				  unsigned char *cipher, unsigned int cipherlen);
int RSADecryption(const std::string &prikey, 
				  const unsigned char *cipher, unsigned int cipherlen,
				  unsigned char *plain, unsigned int plainlen);

void RSAEncryption(const std::string &pubkey, 
				   const std::string &plain, std::string &cipher);
void RSADecryption(const std::string &prikey, 
				   const std::string &cipher, std::string &plain);

typedef struct tagMessage
{
	char id[32];
	int  flag;
} ENY_MESSAGE;


int my_rsa_encrypt(const unsigned char *plain, unsigned int plainlen,
				   unsigned char *cipher, unsigned int cipherlen)
{
	std::string pub_key_base64_string = "4M7MUBgnDntA+Pz3adGWOYpi4GiORgVvcT54Aa9lxaqh6ZZVqUT0YdzXpizs6FySHD6hZw3HXCpKLwTaJHglLKHT/a3BA4Y4q3IeoBvYju/L4dATCnT2CJhUjGx9YMM1Pl+ymm2USQRuMrJidtvQ0QASTVk/vm3OWVrXh2Q7SGU=";

	std::cout << pub_key_base64_string.c_str() << std::endl;


	string encString;
	

	StringSource(pub_key_base64_string.c_str(), true,
		new CryptoPP::Base64Decoder(
		new CryptoPP::StringSink(encString)
		)
		);



	HexDecoder decoder;
	decoder.Put( (byte*)encString.c_str(), encString.size() );
	decoder.MessageEnd();	
	
	AutoSeededRandomPool rng;
	RSA::PublicKey  publicKey;
	publicKey.Load(decoder);

	////////////////////////////////////////////////
	// Secret to protect
	SecByteBlock plaintext(plain, plainlen);

	////////////////////////////////////////////////
	// Encrypt
	RSAES_PKCS1v15_Encryptor encryptor(publicKey);
	//RSAES_OAEP_SHA_Encryptor encryptor(publicKey);

	// Now that there is a concrete object, we can validate
	assert( 0 != encryptor.FixedMaxPlaintextLength());
	assert( plaintext.size() <= encryptor.FixedMaxPlaintextLength());

	// Create cipher text space
	size_t ecl = encryptor.CiphertextLength( plaintext.size());
	assert( 0 != ecl );
	SecByteBlock ciphertext(ecl);

	// Paydirt
	encryptor.Encrypt( rng, plaintext, plaintext.size(), ciphertext);

	////////////////////////////////////////////////
	// Copy
	assert(cipherlen>= ciphertext.size());
	//memcpy(cipher, static_cast<void*>(ciphertext), ciphertext.size());
	memcpy_s(cipher, cipherlen, static_cast<void*>(ciphertext), ciphertext.size());


	CryptoPP::FileSink* outfile = new CryptoPP::FileSink("base64_data.dat");
	

	StringSource(ciphertext, ciphertext.size(), true,
		new Base64Encoder(outfile, false) 
		); 

	return static_cast<int>(ciphertext.size());
}




int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
    _tsetlocale(LC_ALL, _T(""));
    std::wcout.imbue(std::locale(""));  
    std::wcout << L"生成 RSA 密钥" << std::endl;
	std::wcout << L"密钥长度:1024" << std::endl;


	try {
		////////////////////////////////////////////////
		// Generate keys
		AutoSeededRandomPool rng;

		InvertibleRSAFunction parameters;
		parameters.GenerateRandomWithKeySize(rng, 1024);

		RSA::PrivateKey privateKey(parameters);
		RSA::PublicKey publicKey(parameters);

		std::string prikeyName("prikey.der");
		std::string pubkeyName("pubkey.der");
		SaveKey(privateKey, prikeyName);
		SaveKey(publicKey, pubkeyName);

		////////////////////////////////////////////////
		// Secret to protect
		/*char szPlainText[128] = "RSA encryption demo";
		unsigned char szCipherText[256] = {0};*/


		
		
		//RSAES_OAEP_SHA_Encryptor enc;
		//enc.AccessKey().Load(decoder);


		





		ENY_MESSAGE message;
		memset(&message, 0, sizeof(message));
		std::string id = "hello world!";
		memcpy(message.id, id.c_str(), id.length());
		message.flag = 2;



		unsigned char cipherMem[72] = {0};
		//std::vector<unsigned char> cipherMem(sizeof(message)*2);


		my_rsa_encrypt((unsigned char*) &message, sizeof(message),
			(unsigned char*)cipherMem, 72);

		
		//int nLength = RSAEncryption(pubkeyName, 
		//	(unsigned char*) &message, sizeof(message),
		//	(unsigned char*)cipherMem, 72);

		////////////////////////////////////////////////////
		////// Encryption
		////int nLength = RSAEncryption(pubkeyName, 
		////	(unsigned char*) szPlainText, strlen(szPlainText),
		////	szCipherText, sizeof(szCipherText));

		//////////////////////////////////////////////////
		//// Decryption
		///*memset(szPlainText, 0, sizeof(szPlainText));
		//nLength = RSADecryption(prikeyName, szCipherText, nLength, 
		//	(unsigned char*) szPlainText, sizeof(szPlainText));*/


		//std::vector<unsigned char> plainMem(sizeof(message));		
		//nLength = RSADecryption(prikeyName, &cipherMem[0], nLength, 
		//	(unsigned char*) &plainMem[0], sizeof(message));

		//


		//ENY_MESSAGE message2;
		//memcpy(message2.id, &plainMem[0], 32);
		//memcpy(&message2.flag, &plainMem[32], 4);

		//cout << "plain length: " << nLength << endl;
		//cout << "id  : " << message2.id << endl;
		//cout << "flag : " << message2.flag << endl;


		/*cout << "plain length: " << nLength << endl;
		cout << "plain text  : " << szPlainText << endl;*/
	} catch(CryptoPP::Exception& e) {
		cerr << "Caught Exception..." << endl;
		cerr << e.what() << endl;
	}





	




    
    return 0;
}

/*!
 * \author  chenyao
 * \mail    cheny@meizu.com
 * \padding
 *		RSAES_PKCS1v15_Encryptor 
 *			the padding to use: PKCS#1 v1.5
 *			openssl rsautl -encrypt -pkcs -in origin -inkey pubkey.pem -pubin -out encrypt
 *		RSAES_OAEP_SHA_Encryptor 
 * 			the padding to use: PKCS#1 OAEP
 *			openssl rsautl -encrypt -oaep -in origin -inkey pubkey.pem -pubin -out encrypt
 * \help    http://cryptopp.com/wiki/RSA_Encryption_Schemes
 * \details could be private key and public key
 *
 */
int RSAEncryption(const std::string &pubkey, 
		const unsigned char *plain, unsigned int plainlen,
		unsigned char *cipher, unsigned int cipherlen) 
{
	////////////////////////////////////////////////
	// Load keys
	AutoSeededRandomPool rng;

	RSA::PublicKey  publicKey;
	LoadKey(pubkey, publicKey);

	////////////////////////////////////////////////
	// Secret to protect
	SecByteBlock plaintext(plain, plainlen);

	////////////////////////////////////////////////
	// Encrypt
	RSAES_PKCS1v15_Encryptor encryptor(publicKey);
	//RSAES_OAEP_SHA_Encryptor encryptor(publicKey);

	// Now that there is a concrete object, we can validate
	assert( 0 != encryptor.FixedMaxPlaintextLength());
	assert( plaintext.size() <= encryptor.FixedMaxPlaintextLength());

	// Create cipher text space
	size_t ecl = encryptor.CiphertextLength( plaintext.size());
	assert( 0 != ecl );
	SecByteBlock ciphertext(ecl);

	// Paydirt
	encryptor.Encrypt( rng, plaintext, plaintext.size(), ciphertext);

	////////////////////////////////////////////////
	// Copy
	assert(cipherlen>= ciphertext.size());
	memcpy(cipher, static_cast<void*>(ciphertext), ciphertext.size());
	//memcpy_s(cipher, cipherlen, static_cast<void*>(ciphertext), ciphertext.size());

	return static_cast<int>(ciphertext.size());
}

/*!
 * \author  chenyao
 * \mail    cheny@meizu.com
 * \padding
 *		RSAES_PKCS1v15_Decryptor 
 *			the padding to use: PKCS#1 v1.5
 *			openssl rsautl -decrypt -pkcs -in encrypt -inkey prikey.pem -out decrypt
 *		RSAES_OAEP_SHA_Decryptor 
 * 			the padding to use: PKCS#1 OAEP
 *			openssl rsautl -decrypt -oaep -in encrypt -inkey prikey.pem -out decrypt
 * \help    http://cryptopp.com/wiki/RSA_Encryption_Schemes
 * \details must be private key
 *
 */
int RSADecryption(const std::string &prikey, 
		const unsigned char *cipher, unsigned int cipherlen,
		unsigned char *plain, unsigned int plainlen) 
{
	////////////////////////////////////////////////
	// Load keys
	AutoSeededRandomPool rng;

	RSA::PrivateKey privateKey;
	LoadKey(prikey, privateKey);

	////////////////////////////////////////////////
	// Secret to protect
	SecByteBlock ciphertext(cipher, cipherlen);

	////////////////////////////////////////////////
	// Decrypt
	RSAES_PKCS1v15_Decryptor decryptor(privateKey);
	//RSAES_OAEP_SHA_Decryptor decryptor(privateKey);

	// Now that there is a concrete object, we can check sizes
	assert( 0 != decryptor.FixedCiphertextLength());
	assert( ciphertext.size() <= decryptor.FixedCiphertextLength());

	// Create recovered text space
	size_t dpl = decryptor.MaxPlaintextLength( ciphertext.size());
	assert( 0 != dpl );
	SecByteBlock recovered(dpl);

	// Paydirt
	DecodingResult result = decryptor.Decrypt( rng,
		ciphertext, ciphertext.size(), recovered);

	// More sanity checks
	assert( result.isValidCoding );        
	assert( result.messageLength <= decryptor.MaxPlaintextLength( ciphertext.size()) );

	// At this point, we can set the size of the recovered
	//  data. Until decryption occurs (successfully), we
	//  only know its maximum size
	recovered.resize( result.messageLength );

	////////////////////////////////////////////////
	// Copy
	assert(plainlen>= recovered.size());
	memcpy_s(plain, plainlen, static_cast<void*>(recovered), recovered.size());

	return static_cast<int>(recovered.size());
}

/*!
 * \author  chenyao
 * \mail    cheny@meizu.com
 * \padding
 *		RSAES_PKCS1v15_Encryptor 
 *			the padding to use: PKCS#1 v1.5
 *		RSAES_OAEP_SHA_Encryptor 
 * 			the padding to use: PKCS#1 OAEP
 * \help    http://cryptopp.com/wiki/RSA_Encryption_Schemes
 * \details could be private key and public key
 */
void RSAEncryption(const std::string &pubkey, 
		const std::string &plain, std::string &cipher) 
{
	////////////////////////////////////////////////
	// Generate keys
	AutoSeededRandomPool rng;

	RSA::PublicKey  publicKey;
	LoadKey(pubkey, publicKey);

	////////////////////////////////////////////////
	// Encryption
	RSAES_PKCS1v15_Encryptor e(publicKey);
	//RSAES_OAEP_SHA_Encryptor e(publicKey);

	StringSource ss1( plain, true,
		new PK_EncryptorFilter( rng, e, new StringSink(cipher) ) // PK_EncryptorFilter
		); // StringSource
}

/*!
 * \author  chenyao
 * \mail    cheny@meizu.com
 * \padding
 *		RSAES_PKCS1v15_Decryptor 
 *			the padding to use: PKCS#1 v1.5
 *		RSAES_OAEP_SHA_Decryptor 
 * 			the padding to use: PKCS#1 OAEP
 * \help    http://cryptopp.com/wiki/RSA_Encryption_Schemes
 * \details must be private key
 */
void RSADecryption(const std::string &prikey, 
		const std::string &cipher, std::string &plain)
{
	////////////////////////////////////////////////
	// Generate keys
	AutoSeededRandomPool rng;

	RSA::PrivateKey privateKey;
	LoadKey(prikey, privateKey);

	////////////////////////////////////////////////
	// Decryption
	RSAES_PKCS1v15_Decryptor d(privateKey);
	//RSAES_OAEP_SHA_Decryptor d(privateKey);

	StringSource ss2( cipher, true,
		new PK_DecryptorFilter( rng, d, new StringSink(plain) ) // PK_DecryptorFilter
		); // StringSource
}

void SaveKey( const RSA::PublicKey& PublicKey, const string& filename )
{
    // DER Encode Key - X.509 key format
    PublicKey.Save(
        FileSink( filename.c_str(), true /*binary*/ ).Ref()
    );
}

void SaveKey( const RSA::PrivateKey& PrivateKey, const string& filename )
{
    // DER Encode Key - PKCS #8 key format
    PrivateKey.Save(
        FileSink( filename.c_str(), true /*binary*/ ).Ref()
    );
}

void LoadKey( const string& filename, RSA::PublicKey& PublicKey )
{
    // DER Encode Key - X.509 key format
    PublicKey.Load(
        FileSource( filename.c_str(), true, NULL, true /*binary*/ ).Ref()
    );
}

void LoadKey( const string& filename, RSA::PrivateKey& PrivateKey )
{
    // DER Encode Key - PKCS #8 key format
    PrivateKey.Load(
        FileSource( filename.c_str(), true, NULL, true /*binary*/ ).Ref()
    );
}