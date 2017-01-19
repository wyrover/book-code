// console_template.cpp : Defines the entry point for the console application.
//

//#include "targetver.h"
#include <msgpack.hpp>
#define STATIC_GETOPT
#include <getopt.h>
#include <openssl/rand.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <openssl/aes.h>
#include <openssl/md5.h>


#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cassert>

#include "aes_string.h"
#include "util.h"
#include "sha_md5.h"


#ifndef _BYTE
#define _BYTE
typedef unsigned char byte;
#endif

#ifndef _UINT32
#define _UINT32
typedef unsigned int                uint32;
#endif


#define READPUB 0
#define READSEC 1

#define SECFILE "sec.pem"
#define PUBFILE "pub.pem"

#define VERSION "1.0.2"
#define ARGUMENTS "a:g:cdi:o:t:n:xzvh"

INT64 file_get_size(const std::wstring& filename);
bool file_get_contents(const std::wstring& filename, std::vector<byte>& buffer);


bool aes_make_key_and_iv(std::wstring key_filename, unsigned int n_bits);

std::string base64_encode(const byte* buffer, size_t length);
int base64_decode(const char* b64message, byte** buffer, size_t* length);
std::string md5(const std::string &data);

void genkey(int size);
RSA* readpemkeys(int type);
int rsa_encrypt(RSA *key, unsigned char *plain, int len, unsigned char **cipher);
int rsa_decrypt(RSA *key, unsigned char *cipher, int len, unsigned char **plain);





enum
{
	BIN2HEX_ZEROES = 1,
	BIN2HEX_SPACES = 2,
	BIN2HEX_NEWLINES = 4
};

std::string Bin2Hex(const byte *data, size_t count, uint32 flags = BIN2HEX_SPACES | BIN2HEX_NEWLINES);



void showHelp();





int _tmain(int argc, TCHAR* argv[])
{
    _tsetlocale(LC_ALL, _T(""));
    

    HRESULT nResult = S_OK;


	byte tmp_data_test[16];
	RAND_bytes(tmp_data_test, 16); 

	cactus::hex_print_buffer(tmp_data_test, 16);

	printf("\n");

	cactus::print_bytes(tmp_data_test, 16);


	std::vector<byte> tmp_data_test2(16);
	RAND_bytes(&tmp_data_test2[0], 16); 

	std::string encode_hex_data = cactus::encode_hex(tmp_data_test2);

	printf("\n");
	std::cout << encode_hex_data.c_str() << std::endl;

	std::vector<byte> tmp_data_test3 = cactus::decode_hex(encode_hex_data);

	printf("\n");
	cactus::print_bytes(&tmp_data_test3[0], 16);


	int ch,size=0,len=0,ks=0;
	RSA *key=NULL;
	FILE *fpin=NULL, *fpout=NULL;
	unsigned char *cipher=NULL,*plain=NULL;

	std::wstring aes_key_filename;
	unsigned int n_bits = 0;

    std::wstring input_filename;
	std::wstring output_filename;
	bool crypts_flag = false;
	bool decrypt_flag = false;
	bool encode_data_file_flag = false;
	bool decode_data_file_flag = false;
    int bits;
    int c;
    static struct option long_options[] = {
		{_T("generates AES keys file"),  ARG_REQ,  0, _T('a')},
        {_T("generates RSA keys and save them in PEM format"),  ARG_REQ,  0, _T('g')},
		{_T("crypts"),  ARG_NONE,  0, _T('c')},
		{_T("decrypt"),  ARG_NONE,  0, _T('d')},
		{_T("input filename"),  ARG_REQ,  0, _T('i')},
        {_T("output filename"),  ARG_REQ,  0, _T('o')},
		{_T("aes key bits"),  ARG_REQ,  0, _T('n')},
		{_T("decode data file"),  ARG_NONE,  0, _T('x')},
		{_T("encode data file"),  ARG_NONE,  0, _T('z')},
        {_T("version"), ARG_NONE, 0 , _T('v')},
        {_T("help"),    ARG_NONE, 0 , _T('h')},
        { ARG_NULL , ARG_NULL , ARG_NULL , ARG_NULL }
    };

    while (1) {
        int option_index = 0;
        c = getopt_long(argc, argv, _T(ARGUMENTS), long_options, &option_index);

        // Check for end of operation or error
        if (c == -1 || c == '?') {
            break;
        }

        // Handle options
        switch (c) {
		case _T('a'):
			aes_key_filename = optarg;			
			break;
        case _T('g'):
            bits = _wtoi(optarg);
            _tprintf(_T("Generating RSA keys [size = %d bits]\n"), bits);
			genkey(bits);
            _tprintf(_T("Private Key saved in %s file.\n"), _T(SECFILE));
            _tprintf(_T("Public Key saved in %s file.\n"), _T(PUBFILE));
            _tprintf(_T("Done.\n"));
            break;
		case _T('c'):
			crypts_flag = true;
			break;
		case _T('d'):
			decrypt_flag = true;
			break;
		case _T('i'):
			input_filename = optarg;
			_tprintf(_T("input:%s\n"), input_filename.c_str());
			break;
		case _T('o'):
			output_filename = optarg;
			_tprintf(_T("output:%s\n"), output_filename.c_str());
			break;
		case _T('t'):
			AES_KEY aesKey_;
			unsigned char userKey_[16];
			unsigned char in_[16];
			unsigned char out_[16];
			//strcpy((char*)userKey_,"0123456789123456");
			strcpy((char*)in_,"0123456789123456");


		
			RAND_bytes(userKey_, 16); 

			fprintf(stdout,"Original message: %s", in_);
			AES_set_encrypt_key(userKey_, 128, &aesKey_);
			AES_encrypt(in_, out_, &aesKey_);

			AES_set_decrypt_key(userKey_, 128, &aesKey_);
			AES_decrypt(out_, in_,&aesKey_);
			fprintf(stdout,"Recovered Original message: %s", in_);      
			break;
		case _T('n'):
			n_bits = _wtoi(optarg);
			break;

		case _T('x'):
			decode_data_file_flag = true;
			break;

		case _T('z'):
			encode_data_file_flag = true;
			break;

        case _T('v'):
            _tprintf(_T(VERSION));
            nResult |= S_OK;
            break;

        case _T('h'):
            showHelp();
            nResult |= S_OK;
            break;

        default:
            showHelp();
            break;
        }
    }


	// 生成加密数据文件
	if (encode_data_file_flag && !input_filename.empty() && !output_filename.empty()) {

		std::vector<byte> data_buffer;
		file_get_contents(input_filename, data_buffer);

		
		std::vector<byte> out_data_buffer;
		

		std::vector<byte> key(16);
		std::vector<byte> iv(16);
		RAND_bytes(&key[0], 16);
		RAND_bytes(&iv[0], 16);


		aes_encrypt_bytes(key, iv, data_buffer, out_data_buffer);


		printf("AES encrypt key:\n");
		cactus::hex_print_buffer(&key[0], key.size());	
		printf("\n");
		printf("AES encrypt key length = %d\n", key.size());


		std::string data_sha = sha_hash(&out_data_buffer[0], out_data_buffer.size());


		printf("sha data %s\n", data_sha.c_str());
		

		msgpack::sbuffer buffer;
		msgpack::packer<msgpack::sbuffer> pk(&buffer);
		pk.pack(out_data_buffer);
		pk.pack(key);
		pk.pack(iv);
		pk.pack(data_sha);
		pk.pack("world");
		pk.pack("hello");


		
		
		

	
		
		


		//std::stringstream ss;	
		
		//msgpack::pack(sbuf, data_buffer);	
		//msgpack::pack(sbuf, "world");
		//msgpack::pack(sbuf, "hello");


		

		//ss.seekg(0);

		FILE *f = _wfopen(output_filename.c_str(), L"wb+");
		fwrite(buffer.data(), 1, buffer.size(), f);
		fflush(f);
		fclose(f);
		_tprintf(_T("生成加密数据文件成功.\n"));
	}

	if (decode_data_file_flag && !input_filename.empty() && !output_filename.empty()) {		

		std::vector<unsigned char> out_data;

		if (file_get_contents(input_filename, out_data)) {
			//std::cout << "v3:" << &v3[0] << std::endl;
			msgpack::zone zone_;
			std::size_t offset = 0;
			msgpack::object obj1 = msgpack::unpack(zone_, (const char*)&out_data[0], out_data.size(), offset);
			std::vector<unsigned char> data1 = obj1.as<std::vector<unsigned char> >();

			/*std::string string1;
			string1 = std::string((char*)&data1[0]);
			string1.resize(data1.size());
			printf("数据长度:%d 数据内容:%s\n", data1.size(), string1.c_str()); 	*/	

			msgpack::object obj2 = msgpack::unpack(zone_, (const char*)&out_data[0], out_data.size(), offset);
			std::vector<unsigned char> data2 = obj2.as<std::vector<unsigned char> >();


			_tprintf(_T("解密 key 长度:%d\n"), data2.size());



			printf("AES decrypt key:\n");
			cactus::hex_print_buffer(&data2[0], data2.size());				
			printf("\n");
			printf("AES decrypt key length = %d\n", data2.size());


			msgpack::object obj3 = msgpack::unpack(zone_, (const char*)&out_data[0], out_data.size(), offset);
			std::vector<unsigned char> data3 = obj3.as<std::vector<unsigned char> >();




			std::vector<byte> out_data_buffer;
			aes_decrypt_bytes(data2, data3, data1, out_data_buffer);		


			std::string const& string1 = msgpack::unpack(zone_, (const char*)&out_data[0], out_data.size(), offset).as<std::string>();
			printf("data sha:%s\n", string1.c_str()); 	

						
			std::string const& string2 = msgpack::unpack(zone_, (const char*)&out_data[0], out_data.size(), offset).as<std::string>();
			printf("数据长度:%d 数据内容:%s\n", string2.size(), string2.c_str()); 	
			
			std::string const& string3 = msgpack::unpack(zone_, (const char*)&out_data[0], out_data.size(), offset).as<std::string>();
			printf("数据长度:%d 数据内容:%s\n", string3.size(), string3.c_str()); 	
			

			/*msgpack::object obj2 = msgpack::unpack(zone_, (const char*)&out_data[0], out_data.size(), offset);
			std::vector<unsigned char> data4 = obj2.as<std::vector<unsigned char> >();
			printf("数据长度:%d 数据内容:%s\n", data4.size(),  &data4[0]);*/


			FILE *f = _wfopen(output_filename.c_str(), L"wb+");
			fwrite(&out_data_buffer[0], 1, out_data_buffer.size(), f);
			fflush(f);
			fclose(f);
			_tprintf(_T("还原加密数据文件成功.\n"));
			
		}
		
	}


	// AES key 文件生成
	if (!aes_key_filename.empty() && n_bits != 0) {
		if (aes_make_key_and_iv(aes_key_filename.c_str(), n_bits)) {
			_tprintf(_T("Generating AES key file successful."));
		}
	}


	if (crypts_flag && !input_filename.empty() && !output_filename.empty()) {
		key = readpemkeys(READPUB);
		if(!(fpin = _tfopen(input_filename.c_str(), _T("r")))) {
			fwprintf(stderr, TEXT("Error: Cannot locate input file.\n"));			
			exit(EXIT_FAILURE);
		}
		fpout = _tfopen(output_filename.c_str(), _T("w"));
		ks = RSA_size(key);
		plain = (unsigned char *)malloc(ks * sizeof(unsigned char));
		cipher = (unsigned char*)malloc(ks * sizeof(unsigned char));
		_tprintf(_T("Encrypting '%s' file.\n"), input_filename.c_str());
		while(!feof(fpin)) {
			memset(plain,'\0',ks + 1);
			memset(cipher, '\0', ks + 1);
			len = fread(plain, 1, ks - 11, fpin);
			size = rsa_encrypt(key, plain, len, &cipher);
			fwrite(cipher, 1, size, fpout);
		}
		fclose(fpout);
		fclose(fpin);
		free(cipher);
		free(plain);
		RSA_free(key);
		_tprintf(_T("Done.\n"));

	} else if (decrypt_flag && !input_filename.empty() && !output_filename.empty()) {
		
		key = readpemkeys(READSEC);
		if(!(fpin = _tfopen(input_filename.c_str(), _T("r")))) {
			fwprintf(stderr, TEXT("Error: Cannot locate input file.\n"));
			exit(EXIT_FAILURE);
		}
		fpout = _tfopen(output_filename.c_str(), _T("w"));
		ks = RSA_size(key);
		cipher = (unsigned char*)malloc(ks * sizeof(unsigned char));
		plain = (unsigned char*)malloc(ks * sizeof(unsigned char));
		_tprintf(_T("Decrypting '%s' file.\n"), input_filename.c_str());
		while(!feof(fpin)) {
			memset(cipher, '\0', ks);
			memset(plain, '\0', ks);
			if ((len = fread(cipher, 1, ks, fpin)) == 0)
				break;
			size = rsa_decrypt(key, cipher, len, &plain);
			fwrite(plain, 1, size, fpout);
		}
		fclose(fpout);
		fclose(fpin);
		free(plain);
		free(cipher);
		RSA_free(key);
		_tprintf(_T("Done.\n"));

	}

	system("pause");


    return int(nResult);
}


void showHelp()
{
    printf("dgconsole %s\n\n", VERSION);
    printf("Usage: dgconsole <options>\n\n");
    printf("Example:\n");
    printf("以下删除该键值下的一个注册表值\n");
    printf("  dgconsole.exe -g\"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E967-E325-11CE-BFC1-08002BE10318}\" -m\"test001\"\n\n");
    printf("以下删除该键值下的两个注册表值\n");
    printf("  dgconsole.exe -g\"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E967-E325-11CE-BFC1-08002BE10318}\" -m\"test001\" -m\"test002\"\n\n");
    printf("禁用设备\n");
    printf("  dgconsole.exe -d\"设备实例ID\"\n\n");
    printf("启用设备\n");
    printf("  dgconsole.exe -e\"设备实例ID\"\n\n");
    printf("删除设备\n");
    printf("  dgconsole.exe -r\"设备实例ID\"\n\n");
    printf("反安装驱动\n");
    printf("  dgconsole.exe -u\"设备实例ID\"\n\n");
    printf("安装驱动 \n");
    printf("  dgconsole.exe -p\"inf文件路径\" -i\"设备实例ID\"\n\n");
    printf("先禁用再启用,组合命令\n");
    printf("  dgconsole.exe -d\"设备实例ID\" -e\"设备实例ID\"\n\n");
    printf("先删除注册表，再启用设备\n");
    printf("  dgconsole.exe -g\"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E967-E325-11CE-BFC1-08002BE10318}\" -m\"test001\" -e\"设备实例ID\"\n\n");
    printf("关机\n");
    printf("  dgconsole.exe -c\n\n");
    printf("重启\n");
    printf("  dgconsole.exe -b\n\n");
    printf("获取显示器信息\n");
    printf("  dgconsole.exe -a\"display_info\"\n\n");
    printf("设置显示器最佳分辨率\n");
    printf("  dgconsole.exe -a\"set_monitor_display_resolution\"\n\n");
    exit(0);
}



void genkey(int size)
{
    RSA *key = NULL;
    FILE *fp;

    if ((key = RSA_generate_key(size, 3, NULL, NULL)) == NULL) {
        fprintf(stderr, "%s\n", ERR_error_string(ERR_get_error(), NULL));
        exit(EXIT_FAILURE);
    }

    if (RSA_check_key(key) < 1) {
        fprintf(stderr, "Error: Problems while generating RSA Key.\nRetry.\n");
        exit(EXIT_FAILURE);
    }

    fp = fopen(SECFILE, "w");

    if (PEM_write_RSAPrivateKey(fp, key, NULL, NULL, 0, 0, NULL) == 0) {
        fprintf(stderr, "Error: problems while writing RSA Private Key.\n");
        exit(EXIT_FAILURE);
    }

    fclose(fp);
    fp = fopen(PUBFILE, "w");

    if (PEM_write_RSAPublicKey(fp, key) == 0) {
        fprintf(stderr, "Error: problems while writing RSA Public Key.\n");
        exit(EXIT_FAILURE);
    }

    fclose(fp);
    RSA_free(key);
    return;
}


RSA* readpemkeys(int type)
{
	FILE *fp;
	RSA *key=NULL;

	if(type == READPUB) {
		if((fp = fopen(PUBFILE,"r")) == NULL) {
			fprintf(stderr,"Error: Public Key file doesn't exists.\n");
			exit(EXIT_FAILURE);
		}
		if((key = PEM_read_RSAPublicKey(fp,NULL,NULL,NULL)) == NULL) {
			fprintf(stderr,"Error: problems while reading Public Key.\n");
			exit(EXIT_FAILURE);
		}
		fclose(fp);
		return key;
	}
	if(type == READSEC) {
		if((fp = fopen(SECFILE,"r")) == NULL) {
			fprintf(stderr,"Error: Private Key file doesn't exists.\n");
			exit(EXIT_FAILURE);
		}
		if((key = PEM_read_RSAPrivateKey(fp,NULL,NULL,NULL)) == NULL) {
			fprintf(stderr,"Error: problmes while reading Private Key.\n");
			exit(EXIT_FAILURE);
		}
		fclose(fp);
		if(RSA_check_key(key) == -1) {
			fprintf(stderr,"Error: Problems while reading RSA Private Key in '%s' file.\n",SECFILE);
			exit(EXIT_FAILURE);
		} else if(RSA_check_key(key) == 0) {
			fprintf(stderr,"Error: Bad RSA Private Key readed in '%s' file.\n",SECFILE);
			exit(EXIT_FAILURE);
		}
		else
			return key;
	}
	return key;
}

int rsa_encrypt(RSA *key, unsigned char *plain, int len, unsigned char **cipher)
{
	int clen=0;

	srand(time(NULL));
	if((clen = RSA_public_encrypt(len, plain, *cipher, key, RSA_PKCS1_PADDING)) == -1) {
		fprintf(stderr, "%s\n", ERR_error_string(ERR_get_error(), NULL));
		exit(EXIT_FAILURE);
	} else
		return clen;
}

int rsa_decrypt(RSA *key, unsigned char *cipher, int len, unsigned char **plain)
{
	int plen=0;

	if((plen = RSA_private_decrypt(len, cipher, *plain, key, RSA_PKCS1_PADDING)) == -1) {
		fprintf(stderr, "%s\n", ERR_error_string(ERR_get_error(), NULL));
		exit(EXIT_FAILURE);
	} else
		return plen;
}



bool get_random_block(int length, std::string& result1, std::string& result2, std::string& result3)
{
	bool ret = false;


	std::string data1 = "";
	std::string data2 = "";
	std::string data3 = "";
	RAND_poll();
	
	std::vector<byte> key_block(length);
	RAND_bytes(&key_block[0], length); 

	data1 = "{ " + Bin2Hex(&key_block[0], length, BIN2HEX_ZEROES | BIN2HEX_SPACES) + " }";
	data2 = Bin2Hex(&key_block[0], length, BIN2HEX_NEWLINES);
	data3 = base64_encode(&key_block[0], length);
	
	result1 = data1;
	result2 = data2;
	result3 = data3;
	ret = true;
	
	return ret;
}



bool aes_make_key_and_iv(std::wstring key_filename, unsigned int n_bits)
{
	bool ret = false;
	std::string key1, key2, key3;
	std::string iv1, iv2, iv3;

	do {
		if (!get_random_block(n_bits, key1, key2, key3))
			break;

		if (!get_random_block(n_bits, iv1, iv2, iv3))
			break;

		std::fstream file;
		file.open(key_filename.c_str(), std::ios::out | std::ios::binary);

		if (!file.is_open())
			break;

		file << "key and iv array:" << '\n';
		file << "byte key[" << n_bits << "] = " << '\n';
		file << key1 << ";" << '\n';
		file << "byte iv[" << n_bits << "] = " << '\n';
		file << iv1 << ";" << '\n';
		file << "--------------------------------------------------------------------------------------------------" << '\n';
		file << "hex encode key/iv:" << '\n';
		file << key2 << '\n';
		file << iv2 << '\n';
		file << "--------------------------------------------------------------------------------------------------" << '\n';
		file << "base64 encode key/iv:" << '\n';
		file << key3 << '\n';
		file << iv3 << '\n';
		file << "--------------------------------------------------------------------------------------------------" << '\n';
		file << "base64 decode and hex encode key/iv:" << '\n';


		byte* pbuffer = NULL;		
		size_t data_len;
		base64_decode(key3.c_str(), &pbuffer, &data_len);



		byte* pbuffer2 = NULL;		
		size_t data_len2;
		base64_decode(iv3.c_str(), &pbuffer2, &data_len2);
		

		file << Bin2Hex(pbuffer, data_len, BIN2HEX_NEWLINES) << '\n';
		file << Bin2Hex(pbuffer2, data_len, BIN2HEX_NEWLINES) << '\n';		

		free(pbuffer);
		free(pbuffer2);
		ret = true;
	} while (0);

	return ret;
}


std::string Bin2Hex(const byte *data,size_t count,uint32 flags)
{
	if (data != NULL && count > 0)
	{
		std::ostringstream myStream;

		//if no spaces, then only one 0x at the start
		if ((flags & BIN2HEX_ZEROES) && !(flags & BIN2HEX_SPACES) )
			myStream << "0x";	

		for (unsigned int j = 0;j <count;j++)
		{
			byte n = data[j];

			//if spaces, then 0x before every byte
			if ((flags & BIN2HEX_ZEROES) && (flags & BIN2HEX_SPACES) )
				myStream << "0x";

			if (n <= 15)
				myStream << "0";
			myStream << std::hex << (int)n;

			//if no zeroes, just use space to separate
			if (flags & BIN2HEX_SPACES && !(flags & BIN2HEX_ZEROES) )
				myStream << " ";
			// if zeroes, use , to separate
			if (flags & BIN2HEX_SPACES && (flags & BIN2HEX_ZEROES) )
				myStream << ", ";
		}
		myStream.flush();

		//remove the trailing space/,
		if (flags & BIN2HEX_SPACES)
			return myStream.str().substr(0,myStream.str().length()-2);
		else
			return myStream.str();
	}

	return std::string();



}

std::string base64_encode(const byte* buffer, size_t length)
{
	BIO *bio, *b64;
	BUF_MEM *bufferPtr;

	b64 = BIO_new(BIO_f_base64());
	bio = BIO_new(BIO_s_mem());
	bio = BIO_push(b64, bio);

	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Ignore newlines - write everything in one line
	BIO_write(bio, buffer, length);
	BIO_flush(bio);
	BIO_get_mem_ptr(bio, &bufferPtr);
	BIO_set_close(bio, BIO_NOCLOSE);
	//    b64_test = bufferPtr->data;

	std::string copy(bufferPtr->data, bufferPtr->length);

	BIO_free_all(bio);

	return copy;
}

size_t calc_decode_length(const char* b64input) { //Calculates the length of a decoded string
	size_t len = strlen(b64input),
		padding = 0;

	if (b64input[len-1] == '=' && b64input[len-2] == '=') //last two chars are =
		padding = 2;
	else if (b64input[len-1] == '=') //last char is =
		padding = 1;

	return (len*3)/4 - padding;
}

int base64_decode(const char* b64message, byte** buffer, size_t* length)
{
	BIO *bio, *b64;

	int decodeLen = calc_decode_length(b64message);
	*buffer = (unsigned char*)malloc(decodeLen + 1);
	(*buffer)[decodeLen] = '\0';

	bio = BIO_new_mem_buf(b64message, -1);
	b64 = BIO_new(BIO_f_base64());
	bio = BIO_push(b64, bio);

	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Do not use newlines to flush buffer
	*length = BIO_read(bio, *buffer, strlen(b64message));
	assert(*length == decodeLen); //length should equal decodeLen, else something went horribly wrong
	BIO_free_all(bio);

	return decodeLen;
}



std::string md5(const std::string &data) 
{

	MD5_CTX ctx;
	unsigned char md[16] = {0};
	char tmp[3]={'\0'}, buf[33]={'\0'};
	MD5((const unsigned char *)data.c_str(), data.size(), md);

	for (int i = 0; i < 16; i++) {
		sprintf(tmp, "%2.2x", md[i]);
		strcat(buf,tmp);
	}

	return std::string(buf);
}


bool file_get_contents(const std::wstring& filename, std::vector<byte>& buffer)
{
	std::ifstream file(filename.c_str(), std::ios_base::binary);

	if (file) {
		file.seekg(0, std::ios_base::end);
		std::streamsize size = file.tellg();

		if (size > 0) {
			file.seekg(0, std::ios_base::beg);
			buffer.resize(static_cast<std::size_t>(size));
			file.read((char*)&buffer[0], size);
		}

		return true;
	} else {
		return false;
	}
}



INT64 file_get_size(const std::wstring& filename)
{
    /*
    CAtlFile file;
    if( FAILED( file.Create(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING)))
        return 0;

    ULONGLONG filesize = 0;
    file.GetSize(filesize);
    return filesize;
    */
    std::wfstream f(filename.c_str(), std::ios::binary | std::ios::ate);
    return static_cast<ULONGLONG>(f.tellg());
}


std::vector<byte> hex_to_bytes(const std::string& s) {
	size_t len = s.size() / 2;
	std::vector<byte> b(len);

	std::string::const_iterator it; 
	size_t pos = 0;
	for (it = s.begin(); it != s.end(); it++) {
		std::stringstream ss;
		ss << *it++;
		ss << *it;

		unsigned int c;
		ss >> std::hex >> c;

		b[pos++] = c;
	}

	return b;
}


std::string bytes_to_hex(const std::vector<byte>& b) {
	std::stringstream ss;
	for (int i = 0; i < b.size(); i += 1) {
		ss << std::setw(2) << std::setfill('0') << std::hex << (int)b[i];
	}

	return ss.str();
}



std::string bytes_to_str(const std::vector<byte>& v) {
	std::stringstream ss;

	for (int i = 0; i < v.size(); i += 1) {
		ss <<v[i];
	}

	return ss.str();
}

std::vector<byte> str_to_bytes(const std::string& s) {
	std::vector<byte> v;
	std::copy(s.begin(), s.end(), back_inserter(v));
	return v;
}



std::string bytes_to_base64(const std::vector<byte>& b) {
	static const char* codes = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
	std::stringstream ss;

	for (int i = 0; i < b.size(); i += 3) {
		unsigned char c = (b[i] & 0xFC) >> 2;
		ss <<codes[c];
		c = (b[i] & 0x03) << 4;
		if (i + 1 < b.size()) {
			c |= (b[i + 1] & 0xF0) >> 4;
			ss <<codes[c];
			c = (b[i + 1] & 0x0F) << 2;
			if (i + 2 < b.size()) {
				c |= (b[i + 2] & 0xC0) >> 6;
				ss <<codes[c];
				c = b[i + 2] & 0x3F;
				ss <<codes[c];
			} else {
				ss <<codes[c];
				ss << "=";
			}
		} else {
			ss <<codes[c];
			ss << "==";
		}
	}

	return ss.str();
}


std::string string_to_base64(const std::string& s) {
	std::vector<byte> b(s.size());
	for (int i = 0; i < b.size(); i += 1) {
		b[i] = s[i];
	}
	return bytes_to_base64(b);
}



struct print_byte_vector_t{
	const std::vector<byte>& bytes;
	print_byte_vector_t(const std::vector<byte>& b):bytes(b) {}
};

print_byte_vector_t print_byte_vector(const std::vector<byte>& b) {
	return print_byte_vector_t(b);
}

std::ostream& operator << (std::ostream& os, const print_byte_vector_t& bvt) {
	os << "b(";

	for (int i = 0; i < bvt.bytes.size(); i++) {
		os << bvt.bytes[i];
	}	

	return (os << ")");
}




struct char_or_hex_t {
	byte b;
	char_or_hex_t(const byte& a):b(a) {}
};

std::ostream& operator << (std::ostream& os, const char_or_hex_t& c) {
	if (c.b == '\'') {
		return (os << "\\'");
	}
	if (c.b >= 32 && c.b < 127) {
		return (os << "'" << c.b << "'");
	}
	return (os << "0x" << std::hex << (int)c.b);
}

std::ostream& operator << (std::ostream& os, const std::vector<byte>& v) {
	os << "[";
	std::vector<byte>::const_iterator it = v.begin();
	if (it != v.end()) {
		os << char_or_hex_t(*it);
	}
	for (it++; it != v.end(); it++) {
		os << ", " << char_or_hex_t(*it);
	}
	return (os << "]");
}