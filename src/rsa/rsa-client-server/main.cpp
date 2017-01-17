// console_template.cpp : Defines the entry point for the console application.
//

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <assert.h>

#include "HttpClient.h"

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <md5.h>
#include <rsa.h>
#include <hex.h>
#include <osrng.h>
#include <files.h>
#include <secblock.h>
#include <sha.h>
#include <filters.h>
#include <base64.h>
#include <modes.h>

#include <json/json.h>

#include "smbios.h"
#include "machine_define.h"


using namespace std;
using namespace CryptoPP;







namespace 
{
	const size_t MD5LENGTH = 32;
	std::string md5_encrypt(const std::string &msg) {
		std::string digest;
		CryptoPP::Weak1::MD5 md5;
		CryptoPP::StringSource(msg, true, new CryptoPP::HashFilter(md5,
			new CryptoPP::HexEncoder(new CryptoPP::StringSink(digest))));
		return digest;
	}
}

namespace
{
	const UINT16 sAsciiSubTableSize = 35;
	const UCHAR pAsciiSubTable[] = "ABQY7RNM8DCX06PE4FIGHJ9KL3S2TU1V5WZQ";


//Get MachineCode
int getMachineCode(char* idstring)
{

	ULONG64 uuid[4] = { 0,0,0,0 };

	SMBIOSParse(uuid);

	for (int i = 0; i < 4; i++)
	{
		if (i == 0)
			uuid[i] ^= 0x375f80b610cab9d4;
		else
			uuid[i] = ((uuid[i] << 3) ^ (uuid[i - 1] >> 3)) / 3;

		memcpy(&(idstring[i * 8]) + i, &uuid[i], sizeof(ULONG64));
		idstring[(i + 1) * 8 + i] = '-';
	}
	for (int i = 0; i < 36; i++)
	{
		if (i == 8 || i == 17 || i == 26)
			continue;
		idstring[i] = pAsciiSubTable[
			(0xFF & (idstring[i] + (i == 0 ? idstring[35] : idstring[i - 1])))
				% sAsciiSubTableSize];
	}
	idstring[35] = '\0';

	return 0;
}
}

void print_bin2hex(unsigned char * x, int len) {
	int i;
	for (i = 0; i<len; i++) {
		printf("%02x", x[i]);
		if ((i>0) && ((i + 1) % 4 == 0)) { printf(" "); }
	}
	printf("\n");

}





void load_hex_publickey(const string& key_content, RSA::PublicKey& PublicKey);
void load_hex_privatekey(const string& key_content, RSA::PrivateKey& PrivateKey);




void SaveKey(const RSA::PublicKey& PublicKey, const string& filename);
void SaveKey(const RSA::PrivateKey& PrivateKey, const string& filename);
void LoadKey(const string& filename, RSA::PublicKey& PublicKey);
void LoadKey(const string& filename, RSA::PrivateKey& PrivateKey);

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

typedef struct tagMessage {
    unsigned char id[32];
    int  flag;
} ENY_MESSAGE;



int __stdcall rsa_decryption(const unsigned char *cipher, unsigned int cipherlen,
                             unsigned char *plain, unsigned int plainlen)
{
    std::string hex_private_key = "30820274020100300D06092A864886F70D01010105000482025E3082025A02010002818100931C14D56869421A86D5F5B193D8227D006AD576F95F61B681ABD0F97C89A573935AD3245221B99FB76163271386DC1ADBE1B3B394DB753AF96F30688BE012C329BB06C7637B2A2AFC1E2D02B2E45B1418BFC50BD9E2918E97FBDEC7614B31E97DD43B75088BDAF2A98BC8E6F1B3D38F7BFA2A150EBC3086680A95CA58F1751D0201110281800E6C2A330A3C83F88FBA9F9DF56580DF141E8D66045EAF3A0CB67DE6436CDE064FB39230BCC20D2DC6AF31E0B199C543E35C66F37D0674E7AA05E19BD17A5C311A4608CCED71724FC023903F98BE99F1DF912405F88C9E5E1A7E7364E1C6F3C52B001ED1BBF80E63017D678C27A7A913E410AC114103C2FB3D642866468AA695024100C3D572D2F6DC5178000AF9F8B02F84CB60A2EBB40E94FB64BA3852749C33C8FB7D290738C1E833690C8E7C1FAC5417CF469AF8381E3E3FA3D14A815150C0F35F024100C04E6D63C380178587754081531CB40C02EE03814A80139D35ECF34DC62A18DFB076C6ADC95B14CAF4315FCB4A7E658EEBE856C68C8AC5ACBE28DECC38AA450302407332438B27CCE4A0F0F765DD94D0A877A241B7D353DF2A777C9999EA3DC41BDF3A909AD617B5C3E370CC4903928BD1C5389764D5B76FE9332FD1793EE43543DD0241009E5EB47046A5B9048DABDAC4DB0894462087C6A6B5D2E2F9F02C8C21EE7D056CEBAD1C16A5D289981464E57A3D5908579519B0DFBF08DF06BAB83F028904B14D024016B332D653A15025CE690421B64F1AC5DC54E531C4D5DECC694B2871DD96E6F6DCBC7BBC1A3559769C5AD9A1AA43CDCF9698BC50D62D6C7158EFF030B9D2229D";
    AutoSeededRandomPool rng;
    RSA::PrivateKey privateKey;
    load_hex_privatekey(hex_private_key, privateKey);
    ////////////////////////////////////////////////
    // Secret to protect
    SecByteBlock ciphertext(cipher, cipherlen);
    ////////////////////////////////////////////////
    // Decrypt
    RSAES_PKCS1v15_Decryptor decryptor(privateKey);
    //RSAES_OAEP_SHA_Decryptor decryptor(privateKey);
    // Now that there is a concrete object, we can check sizes
    assert(0 != decryptor.FixedCiphertextLength());
    assert(ciphertext.size() <= decryptor.FixedCiphertextLength());
    // Create recovered text space
    size_t dpl = decryptor.MaxPlaintextLength(ciphertext.size());
    assert(0 != dpl);
    SecByteBlock recovered(dpl);
    // Paydirt
    DecodingResult result = decryptor.Decrypt(rng,
                            ciphertext, ciphertext.size(), recovered);
    // More sanity checks
    assert(result.isValidCoding);
    assert(result.messageLength <= decryptor.MaxPlaintextLength(ciphertext.size()));
    // At this point, we can set the size of the recovered
    //  data. Until decryption occurs (successfully), we
    //  only know its maximum size
    recovered.resize(result.messageLength);
    ////////////////////////////////////////////////
    // Copy
    assert(plainlen >= recovered.size());
    memcpy_s(plain, plainlen, static_cast<void*>(recovered), recovered.size());
    return static_cast<int>(recovered.size());    
}


int __stdcall rsa_decryption2(const unsigned char *cipher, unsigned int cipherlen,
	unsigned char **plain, unsigned int *plainlen)
{
	std::string hex_private_key = "30820274020100300D06092A864886F70D01010105000482025E3082025A02010002818100931C14D56869421A86D5F5B193D8227D006AD576F95F61B681ABD0F97C89A573935AD3245221B99FB76163271386DC1ADBE1B3B394DB753AF96F30688BE012C329BB06C7637B2A2AFC1E2D02B2E45B1418BFC50BD9E2918E97FBDEC7614B31E97DD43B75088BDAF2A98BC8E6F1B3D38F7BFA2A150EBC3086680A95CA58F1751D0201110281800E6C2A330A3C83F88FBA9F9DF56580DF141E8D66045EAF3A0CB67DE6436CDE064FB39230BCC20D2DC6AF31E0B199C543E35C66F37D0674E7AA05E19BD17A5C311A4608CCED71724FC023903F98BE99F1DF912405F88C9E5E1A7E7364E1C6F3C52B001ED1BBF80E63017D678C27A7A913E410AC114103C2FB3D642866468AA695024100C3D572D2F6DC5178000AF9F8B02F84CB60A2EBB40E94FB64BA3852749C33C8FB7D290738C1E833690C8E7C1FAC5417CF469AF8381E3E3FA3D14A815150C0F35F024100C04E6D63C380178587754081531CB40C02EE03814A80139D35ECF34DC62A18DFB076C6ADC95B14CAF4315FCB4A7E658EEBE856C68C8AC5ACBE28DECC38AA450302407332438B27CCE4A0F0F765DD94D0A877A241B7D353DF2A777C9999EA3DC41BDF3A909AD617B5C3E370CC4903928BD1C5389764D5B76FE9332FD1793EE43543DD0241009E5EB47046A5B9048DABDAC4DB0894462087C6A6B5D2E2F9F02C8C21EE7D056CEBAD1C16A5D289981464E57A3D5908579519B0DFBF08DF06BAB83F028904B14D024016B332D653A15025CE690421B64F1AC5DC54E531C4D5DECC694B2871DD96E6F6DCBC7BBC1A3559769C5AD9A1AA43CDCF9698BC50D62D6C7158EFF030B9D2229D";
	AutoSeededRandomPool rng;
	RSA::PrivateKey privateKey;
	load_hex_privatekey(hex_private_key, privateKey);
	////////////////////////////////////////////////
	// Secret to protect
	SecByteBlock ciphertext(cipher, cipherlen);
	////////////////////////////////////////////////
	// Decrypt
	RSAES_PKCS1v15_Decryptor decryptor(privateKey);
	//RSAES_OAEP_SHA_Decryptor decryptor(privateKey);
	// Now that there is a concrete object, we can check sizes
	assert(0 != decryptor.FixedCiphertextLength());
	assert(ciphertext.size() <= decryptor.FixedCiphertextLength());
	// Create recovered text space
	size_t dpl = decryptor.MaxPlaintextLength(ciphertext.size());
	assert(0 != dpl);
	SecByteBlock recovered(dpl);
	// Paydirt
	DecodingResult result = decryptor.Decrypt(rng,
		ciphertext, ciphertext.size(), recovered);
	// More sanity checks
	assert(result.isValidCoding);
	assert(result.messageLength <= decryptor.MaxPlaintextLength(ciphertext.size()));
	// At this point, we can set the size of the recovered
	//  data. Until decryption occurs (successfully), we
	//  only know its maximum size
	recovered.resize(result.messageLength);
	////////////////////////////////////////////////
	// Copy
	//assert(plainlen >= recovered.size());

	*plainlen = recovered.size();
	*plain = new unsigned char[*plainlen];
	memcpy(*plain, static_cast<void*>(recovered), recovered.size());
	return static_cast<int>(recovered.size());	
}


int __stdcall rsa_encryption(const unsigned char *plain, unsigned int plainlen,
                             unsigned char *cipher, unsigned int cipherlen)
{
    std::string hex_public_key = "30819D300D06092A864886F70D010101050003818B0030818702818100931C14D56869421A86D5F5B193D8227D006AD576F95F61B681ABD0F97C89A573935AD3245221B99FB76163271386DC1ADBE1B3B394DB753AF96F30688BE012C329BB06C7637B2A2AFC1E2D02B2E45B1418BFC50BD9E2918E97FBDEC7614B31E97DD43B75088BDAF2A98BC8E6F1B3D38F7BFA2A150EBC3086680A95CA58F1751D020111";
    AutoSeededRandomPool rng;
    RSA::PublicKey  publicKey;
    load_hex_publickey(hex_public_key, publicKey);
    ////////////////////////////////////////////////
    // Secret to protect
    SecByteBlock plaintext(plain, plainlen);
    ////////////////////////////////////////////////
    // Encrypt
    RSAES_PKCS1v15_Encryptor encryptor(publicKey);
    //RSAES_OAEP_SHA_Encryptor encryptor(publicKey);
    // Now that there is a concrete object, we can validate
    assert(0 != encryptor.FixedMaxPlaintextLength());
    assert(plaintext.size() <= encryptor.FixedMaxPlaintextLength());
    // Create cipher text space
    size_t ecl = encryptor.CiphertextLength(plaintext.size());
    assert(0 != ecl);
    SecByteBlock ciphertext(ecl);
    // Paydirt
    encryptor.Encrypt(rng, plaintext, plaintext.size(), ciphertext);
    ////////////////////////////////////////////////
    // Copy
    assert(cipherlen >= ciphertext.size());
    //memcpy(cipher, static_cast<void*>(ciphertext), ciphertext.size());
    memcpy_s(cipher, cipherlen, static_cast<void*>(ciphertext), ciphertext.size());
    return static_cast<int>(ciphertext.size());
}


int __stdcall rsa_encryption2(const unsigned char *plain, unsigned int plainlen,
	unsigned char **cipher, unsigned int *cipherlen)
{
	std::string hex_public_key = "30819D300D06092A864886F70D010101050003818B0030818702818100931C14D56869421A86D5F5B193D8227D006AD576F95F61B681ABD0F97C89A573935AD3245221B99FB76163271386DC1ADBE1B3B394DB753AF96F30688BE012C329BB06C7637B2A2AFC1E2D02B2E45B1418BFC50BD9E2918E97FBDEC7614B31E97DD43B75088BDAF2A98BC8E6F1B3D38F7BFA2A150EBC3086680A95CA58F1751D020111";
	AutoSeededRandomPool rng;
	RSA::PublicKey  publicKey;
	load_hex_publickey(hex_public_key, publicKey);
	////////////////////////////////////////////////
	// Secret to protect
	SecByteBlock plaintext(plain, plainlen);
	////////////////////////////////////////////////
	// Encrypt
	RSAES_PKCS1v15_Encryptor encryptor(publicKey);
	//RSAES_OAEP_SHA_Encryptor encryptor(publicKey);
	// Now that there is a concrete object, we can validate
	assert(0 != encryptor.FixedMaxPlaintextLength());
	assert(plaintext.size() <= encryptor.FixedMaxPlaintextLength());
	// Create cipher text space
	size_t ecl = encryptor.CiphertextLength(plaintext.size());
	assert(0 != ecl);
	SecByteBlock ciphertext(ecl);
	// Paydirt
	encryptor.Encrypt(rng, plaintext, plaintext.size(), ciphertext);
	////////////////////////////////////////////////
	// Copy
	

	*cipherlen = ciphertext.size();
	*cipher = new unsigned char[*cipherlen];
	memcpy(*cipher, static_cast<void*>(ciphertext), ciphertext.size());
	//memcpy_s(cipher, cipherlen, static_cast<void*>(ciphertext), ciphertext.size());
	return static_cast<int>(ciphertext.size());
}


void create_key_pairs()
{
    AutoSeededRandomPool rng;
    InvertibleRSAFunction parameters;
    parameters.GenerateRandomWithKeySize(rng, 1024);
    RSA::PrivateKey privateKey(parameters);
    RSA::PublicKey publicKey(parameters);
    std::string prikeyName("prikey.txt");
    std::string pubkeyName("pubkey.txt");
    SaveKey(privateKey, prikeyName);
    SaveKey(publicKey, pubkeyName);
}

int aes_encrypt(const unsigned char *plain, unsigned int plainlen,
	unsigned char **cipher, unsigned int *cipherlen)
{
	AutoSeededRandomPool prng;	
	std::vector<byte> key_block(AES::DEFAULT_KEYLENGTH);
	std::vector<byte> iv_block(AES::BLOCKSIZE);
	prng.GenerateBlock(&key_block[0], sizeof(byte) * AES::DEFAULT_KEYLENGTH);
	prng.GenerateBlock(&iv_block[0], sizeof(byte) * AES::BLOCKSIZE);
	
	
	AES::Encryption aesEncryption(&key_block[0], AES::MAX_KEYLENGTH);
	CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, &iv_block[0]);

	string ciphered;
	StreamTransformationFilter cbcEncryptor(cbcEncryption, new StringSink(ciphered));
	cbcEncryptor.Put((byte *)plain, plainlen);
	cbcEncryptor.MessageEnd();

	*cipherlen = ciphered.size();
	*cipher = new unsigned char[*cipherlen];

	memcpy(*cipher, ciphered.data(), *cipherlen);

	return static_cast<int>(*cipherlen);
}


int aes_key_iv_rsa_encrypt(const unsigned char *plain, unsigned int plainlen,
	unsigned char **cipher, unsigned int *cipherlen)
{
	AutoSeededRandomPool prng;
	std::vector<byte> key_block(AES::DEFAULT_KEYLENGTH);
	std::vector<byte> iv_block(AES::BLOCKSIZE);
	prng.GenerateBlock(&key_block[0], sizeof(byte) * AES::DEFAULT_KEYLENGTH);
	prng.GenerateBlock(&iv_block[0], sizeof(byte) * AES::BLOCKSIZE);


	std::vector<byte> key_iv_block(AES::BLOCKSIZE * 2);

	memcpy(&key_iv_block[0], &key_block[0], AES::BLOCKSIZE);
	memcpy(&key_iv_block[AES::BLOCKSIZE], &iv_block[0], AES::BLOCKSIZE);

	unsigned char *aes_cipher = NULL;
	unsigned char *p_ras_encrypted_aes_key_iv = NULL;
	try
	{
		CFB_Mode< AES >::Encryption e;
		e.SetKeyWithIV(&key_block[0], key_block.size(), &iv_block[0]);

		string ciphered;
		StreamTransformationFilter filter(e, new StringSink(ciphered));
		filter.Put((const byte*)plain, plainlen);
		filter.MessageEnd();

		unsigned int aes_cipherlen = ciphered.size();
		aes_cipher = new unsigned char[aes_cipherlen];
		memcpy(aes_cipher, ciphered.data(), aes_cipherlen);

		
		unsigned int ras_encrypted_aes_key_iv_len = 0;
		rsa_encryption2(&key_iv_block[0], AES::BLOCKSIZE * 2, &p_ras_encrypted_aes_key_iv, &ras_encrypted_aes_key_iv_len);


		*cipherlen = aes_cipherlen + ras_encrypted_aes_key_iv_len;
		*cipher = new unsigned char[*cipherlen];
		memcpy(*cipher, p_ras_encrypted_aes_key_iv, ras_encrypted_aes_key_iv_len);
		memcpy(*cipher + ras_encrypted_aes_key_iv_len, aes_cipher, aes_cipherlen);

	}
	catch (CryptoPP::Exception const& e)
	{
		if (aes_cipher) {
			delete[] aes_cipher;
			aes_cipher = NULL;
		}

		if (p_ras_encrypted_aes_key_iv) {
			delete[] p_ras_encrypted_aes_key_iv;
			p_ras_encrypted_aes_key_iv = NULL;
		}

		return 0;
	}

	if (aes_cipher) {
		delete[] aes_cipher;
		aes_cipher = NULL;
	}

	if (p_ras_encrypted_aes_key_iv) {
		delete[] p_ras_encrypted_aes_key_iv;
		p_ras_encrypted_aes_key_iv = NULL;
	}

	return static_cast<int>(*cipherlen);

}

int aes_key_iv_rsa_decrypt(const unsigned char *cipher, unsigned int cipherlen,
	unsigned char **plain, unsigned int *plainlen)
{
	unsigned char *p_aes_key_iv = NULL;
	unsigned int aes_key_iv_len = 0;

	try
	{
		rsa_decryption2(cipher, 128, &p_aes_key_iv, &aes_key_iv_len);
	}
	catch (CryptoPP::Exception const& e)
	{
		if (p_aes_key_iv) {
			delete[] p_aes_key_iv;
			p_aes_key_iv = NULL;
		}

		return 0;
	}

	std::vector<byte> key_block(AES::DEFAULT_KEYLENGTH);
	std::vector<byte> iv_block(AES::BLOCKSIZE);

	memcpy(&key_block[0], p_aes_key_iv, AES::DEFAULT_KEYLENGTH);
	memcpy(&iv_block[0], p_aes_key_iv + AES::DEFAULT_KEYLENGTH, AES::BLOCKSIZE);

	if (p_aes_key_iv) {
		delete[] p_aes_key_iv;
		p_aes_key_iv = NULL;
	}

	string plained;

	try
	{

		CFB_Mode< AES >::Decryption d;
		d.SetKeyWithIV(&key_block[0], key_block.size(), &iv_block[0]);

		StreamTransformationFilter filter(d, new StringSink(plained));
		filter.Put((byte *)cipher + 128, cipherlen - 128);
		filter.MessageEnd();

		*plainlen = plained.size();
		*plain = new unsigned char[*plainlen];
		memcpy(*plain, plained.data(), *plainlen);

	}
	catch (CryptoPP::Exception const& e)
	{
		return 0;
	}

	
	return static_cast<int>(*plainlen);	
}

std::string make_a_json_string()
{
	char machine_code[36] = { 0 };
	getMachineCode(machine_code);
	std::string uuid = md5_encrypt(machine_code);
	Json::Value root;
	root["uuid"] = uuid;
	root["softid"] = 1;
	root["install"] = 1;
	root["time"] = 12121;
	return root.toStyledString();	
}


int send_to_server()
{
    
    try {

		char machine_code[36] = {0};

		getMachineCode(machine_code);

		std::cout << "machine code:" << machine_code << std::endl;

		std::string id = md5_encrypt(machine_code).c_str();		

		std::cout << "md5 machine code:" << id.c_str() << std::endl;
       
		ENY_MESSAGE message;
		memset(&message, 0, sizeof(message));
		
		memcpy(message.id, id.c_str(), id.length());
		message.flag = DUBA_MORE1;
		unsigned char cipherMem[256] = {0};
		int nLength = rsa_encryption((unsigned char*) &message, sizeof(message),
			(unsigned char*)cipherMem, 256);


		/*FILE* fp = fopen("post_data.dat", "wb");

		if (fp) {
			size_t ret = fwrite(cipherMem, 1, nLength, fp);			
			fclose(fp);
		}*/

        CmHttpClient http_client;
        CStringA receive_content;
        DWORD error_code;


		BOOL result = http_client.Post2(L"http://liveupdate9.drivergenius.com/utg.ashx?act=update&ver=1.0", (BYTE*)cipherMem, nLength, receive_content, &error_code);


		if (result) {			

			std::cout << (LPCSTR)receive_content << std::endl;
			std::cout << "update successful." << std::endl;
			
		}

        result = http_client.Post2(L"http://liveupdate9.drivergenius.com/utg.ashx?act=search&ver=1.0", (BYTE*)cipherMem, nLength, receive_content, &error_code);

        if (result) {			
            
			std::cout << (LPCSTR)receive_content << std::endl;
			std::cout << "search successful." << std::endl;
        }

		



    } catch (CryptoPP::Exception& e) {
        cerr << "Caught Exception..." << endl;
        cerr << e.what() << endl;
    }

	return 0;
}

void test1()
{
	ENY_MESSAGE message;
    memset(&message, 0, sizeof(message));
    std::string id = "hello world!";
    memcpy(message.id, id.c_str(), id.length());
    message.flag = 2;
    unsigned char cipherMem[72] = {0};
    int nLength = rsa_encryption((unsigned char*) &message, sizeof(message),
                                 (unsigned char*)cipherMem, 72);
    std::vector<unsigned char> plainMem(sizeof(message));
    nLength = rsa_decryption(cipherMem, nLength,
                             (unsigned char*) &plainMem[0], sizeof(message));
    ENY_MESSAGE message2;
    memcpy(message2.id, &plainMem[0], 32);
    memcpy(&message2.flag, &plainMem[32], 4);
    cout << "plain length: " << nLength << endl;
    cout << "id  : " << message2.id << endl;
    cout << "flag : " << message2.flag << endl;
}

void Test()
{
	send_to_server();
}


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
    _tsetlocale(LC_ALL, _T(""));	

	//Test();

	std::string test_data = make_a_json_string();

	unsigned char *cipher = NULL;
	unsigned int cipherlen = 0;

	aes_key_iv_rsa_encrypt((const unsigned char*)test_data.c_str(), test_data.size(), &cipher, &cipherlen);

	unsigned char *plain = NULL;
	unsigned int plainlen = 0;
	aes_key_iv_rsa_decrypt(cipher, cipherlen, &plain, &plainlen);

	
	std::string out_string;
	out_string.resize(plainlen);
	memcpy((void*)out_string.data(), plain, plainlen);

	printf("decrypt data:%s\n", out_string.c_str());


	FILE* fp = fopen("post_data.dat", "wb");

	if (fp) {
		size_t ret = fwrite(cipher, 1, cipherlen, fp);
		fclose(fp);
	}

	
	CmHttpClient http_client;
	CStringA receive_content;
	DWORD error_code;


	BOOL result = http_client.Post2(L"http://liveupdate9.drivergenius.com/utg.ashx?act=update&ver=1.0", (BYTE*)cipher, cipherlen, receive_content, &error_code);


	if (result) {

		std::cout << (LPCSTR)receive_content << std::endl;
		std::cout << "update successful." << std::endl;

	}
	
	CmHttpClient http_client2;
	result = http_client2.Get(L"http://liveupdate9.drivergenius.com/utg.ashx?act=search&uuid=03775BC3F735ACB0C1EEB0890672118C&ver=1.0", receive_content, &error_code);

	if (result) {

		std::cout << (LPCSTR)receive_content << std::endl;
		std::cout << "search successful." << std::endl;
	}


	if (plain) {
		delete[] plain;
		plain = NULL;
	}

	if (cipher) {
		delete[] cipher;
		cipher = NULL;
	}
	
    
    return 0;
}

/*!
 * \author  chenyao
 * \mail    cheny@meizu.com
 * \padding
 *      RSAES_PKCS1v15_Encryptor
 *          the padding to use: PKCS#1 v1.5
 *          openssl rsautl -encrypt -pkcs -in origin -inkey pubkey.pem -pubin -out encrypt
 *      RSAES_OAEP_SHA_Encryptor
 *          the padding to use: PKCS#1 OAEP
 *          openssl rsautl -encrypt -oaep -in origin -inkey pubkey.pem -pubin -out encrypt
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
    assert(0 != encryptor.FixedMaxPlaintextLength());
    assert(plaintext.size() <= encryptor.FixedMaxPlaintextLength());
    // Create cipher text space
    size_t ecl = encryptor.CiphertextLength(plaintext.size());
    assert(0 != ecl);
    SecByteBlock ciphertext(ecl);
    // Paydirt
    encryptor.Encrypt(rng, plaintext, plaintext.size(), ciphertext);
    ////////////////////////////////////////////////
    // Copy
    assert(cipherlen >= ciphertext.size());
    memcpy(cipher, static_cast<void*>(ciphertext), ciphertext.size());
    //memcpy_s(cipher, cipherlen, static_cast<void*>(ciphertext), ciphertext.size());
    return static_cast<int>(ciphertext.size());
}

/*!
 * \author  chenyao
 * \mail    cheny@meizu.com
 * \padding
 *      RSAES_PKCS1v15_Decryptor
 *          the padding to use: PKCS#1 v1.5
 *          openssl rsautl -decrypt -pkcs -in encrypt -inkey prikey.pem -out decrypt
 *      RSAES_OAEP_SHA_Decryptor
 *          the padding to use: PKCS#1 OAEP
 *          openssl rsautl -decrypt -oaep -in encrypt -inkey prikey.pem -out decrypt
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
    assert(0 != decryptor.FixedCiphertextLength());
    assert(ciphertext.size() <= decryptor.FixedCiphertextLength());
    // Create recovered text space
    size_t dpl = decryptor.MaxPlaintextLength(ciphertext.size());
    assert(0 != dpl);
    SecByteBlock recovered(dpl);
    // Paydirt
    DecodingResult result = decryptor.Decrypt(rng,
                            ciphertext, ciphertext.size(), recovered);
    // More sanity checks
    assert(result.isValidCoding);
    assert(result.messageLength <= decryptor.MaxPlaintextLength(ciphertext.size()));
    // At this point, we can set the size of the recovered
    //  data. Until decryption occurs (successfully), we
    //  only know its maximum size
    recovered.resize(result.messageLength);
    ////////////////////////////////////////////////
    // Copy
    assert(plainlen >= recovered.size());
    memcpy_s(plain, plainlen, static_cast<void*>(recovered), recovered.size());
    return static_cast<int>(recovered.size());
}

/*!
 * \author  chenyao
 * \mail    cheny@meizu.com
 * \padding
 *      RSAES_PKCS1v15_Encryptor
 *          the padding to use: PKCS#1 v1.5
 *      RSAES_OAEP_SHA_Encryptor
 *          the padding to use: PKCS#1 OAEP
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
    StringSource ss1(plain, true,
                     new PK_EncryptorFilter(rng, e, new StringSink(cipher))   // PK_EncryptorFilter
                    ); // StringSource
}

/*!
 * \author  chenyao
 * \mail    cheny@meizu.com
 * \padding
 *      RSAES_PKCS1v15_Decryptor
 *          the padding to use: PKCS#1 v1.5
 *      RSAES_OAEP_SHA_Decryptor
 *          the padding to use: PKCS#1 OAEP
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
    StringSource ss2(cipher, true,
                     new PK_DecryptorFilter(rng, d, new StringSink(plain))   // PK_DecryptorFilter
                    ); // StringSource
}

void SaveKey(const RSA::PublicKey& PublicKey, const string& filename)
{
    // DER Encode Key - X.509 key format
    PublicKey.Save(
        CryptoPP::HexEncoder(new FileSink(filename.c_str(), true /*binary*/)).Ref()
    );
}

void SaveKey(const RSA::PrivateKey& PrivateKey, const string& filename)
{
    PrivateKey.Save(
        CryptoPP::HexEncoder(new FileSink(filename.c_str(), true /*binary*/)).Ref()
    );
    // DER Encode Key - PKCS #8 key format
    //PrivateKey.Save(
    //    FileSink(filename.c_str(), true /*binary*/).Ref()
    //);
}

void load_base64_publickey(const string& key_content, RSA::PublicKey& PublicKey)
{    

	CryptoPP::Base64Decoder decoder;
	decoder.Put((byte*)key_content.c_str(), key_content.size());
	decoder.MessageEnd();
	PublicKey.Load(decoder);

}

void load_base64_privatekey(const string& key_content, RSA::PrivateKey& PrivateKey)
{

	CryptoPP::Base64Decoder decoder;
	decoder.Put((byte*)key_content.c_str(), key_content.size());
	decoder.MessageEnd();
	PrivateKey.Load(decoder);
}

void load_hex_publickey(const string& key_content, RSA::PublicKey& PublicKey)
{	
	CryptoPP::HexDecoder decoder;
	decoder.Put((byte*)key_content.c_str(), key_content.size());
	decoder.MessageEnd();
	PublicKey.Load(decoder);
}

void load_hex_privatekey(const string& key_content, RSA::PrivateKey& PrivateKey)
{
	CryptoPP::HexDecoder decoder;
	decoder.Put((byte*)key_content.c_str(), key_content.size());
	decoder.MessageEnd();	
	PrivateKey.Load(decoder);
}





void LoadKey(const string& filename, RSA::PublicKey& PublicKey)
{
    // DER Encode Key - X.509 key format
    PublicKey.Load(
        FileSource(filename.c_str(), true, NULL, true /*binary*/).Ref()
    );
}

void LoadKey(const string& filename, RSA::PrivateKey& PrivateKey)
{
    // DER Encode Key - PKCS #8 key format
    PrivateKey.Load(
        FileSource(filename.c_str(), true, NULL, true /*binary*/).Ref()
    );
}