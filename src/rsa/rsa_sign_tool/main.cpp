/*!
* \file main.cpp
* \brief 加密工具命令行 
* 
* 可生成rsa密钥对, aes密钥, ase加密文件等等
* 
* \author wangyang 
* \date 2015/04/15 
* \version 1.0
*/

#include <tchar.h>
#include <rsa.h>
#include <osrng.h>     // PRNG
#include <hex.h>       // Hex Encoder/Decoder
#include <files.h>     // File Source and Sink
#include <filters.h>
#include <base64.h>
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/foreach.hpp>
#include "cactus_rsa.h"

using namespace std;
using namespace CryptoPP;
namespace po = boost::program_options;





int _tmain(int argc, _TCHAR* argv[])
{
    po::options_description desc("允许选项");
    desc.add_options()
    ("help,h", "帮助信息")
    ("key", "生成rsa密钥对")
	("bits", po::value<int>(), "密钥位数(512, 1024, 2048)")
    ("sign", po::value<string>(), "要签名的文件")
    ("verify", po::value<string>(), "要验证的文件")
    ("md5", po::value<string>(), "md5 文件名")
    ("base64_encode_file_source", po::value<string>(), "文件名")
    ("base64_encode_file_dest", po::value<string>(), "文件名")
    ("base64_decode_file_source", po::value<string>(), "文件名")
    ("base64_decode_file_dest", po::value<string>(), "文件名")
    ("aes_key", po::value<string>(), "输出 AES key 文件")
    ("aes-encrypt", "AES加密")
    ("aes-decrypt", "AES解密")
    ("a-key", po::value<string>(), "AES key")
    ("a-iv", po::value<string>(), "AES iv")
    ("a-source", po::value<string>(), "AES 输入文件")
    ("a-dest", po::value<string>(), "AES 输出文件");

    po::variables_map vmap;
    po::store(po::parse_command_line(argc, argv, desc), vmap);
    po::notify(vmap);

    if (vmap.count("help")) {
        cout << desc << endl;
        return 1;
    }

    if (vmap.count("key") && vmap.count("bits")) {
        cactus::RSA_TEST rsa;
		int bits = vmap["bits"].as<int>();
        rsa.generate_rsa_key(bits);
        return 0;
    }

    if (vmap.count("sign")) {
        std::string source_filename = vmap["sign"].as<std::string>(); 
        std::string dest_filename = source_filename + ".signed";
        cactus::RSA_TEST rsa;
        rsa.sign_file("key.pv", source_filename, dest_filename);
        std::cout << source_filename << " to " << dest_filename  << std::endl;
        return 0;
    }

    if (vmap.count("verify")) {
        std::string source_filename = vmap["verify"].as<std::string>(); 
        std::string dest_filename = "E:\\rover-mydrivers-work\\drivergenius2015\\product\\win32\\test2.txt";
        cactus::RSA_TEST rsa;
        bool ret = rsa.verify_file("key.pb", source_filename, dest_filename);
        if (ret) {
            std::cout << source_filename << " to " << dest_filename  << std::endl;
            return 0;
        }

        return -1;
    }

    if (vmap.count("md5")) {
        std::string source_filename = vmap["md5"].as<std::string>(); 
        std::string hash_data = cactus::md5_file(source_filename);
        std::cout << hash_data << std::endl;
        return 0;
    }

    if (vmap.count("base64_encode_file_source") && vmap.count("base64_encode_file_dest")) {       
        std::string source_filename = vmap["base64_encode_file_source"].as<std::string>(); 
        std::string dest_filename = vmap["base64_encode_file_dest"].as<string>();         
        if (cactus::base64_encode_file(source_filename, dest_filename))
            std::cout << "base64_encode_file: " << source_filename << " success." << std::endl;

        return 0;
    }

    if (vmap.count("base64_decode_file_source") && vmap.count("base64_decode_file_dest")) {      
 
        std::string source_filename = vmap["base64_decode_file_source"].as<std::string>(); 
        std::string dest_filename = vmap["base64_decode_file_dest"].as<string>();         
        if (cactus::base64_decode_file(source_filename, dest_filename))
            std::cout << "base64_decode_file: " << source_filename << " success." << std::endl;

        return 0;
    }

    if (vmap.count("aes_key")) {
        std::string key_filename = vmap["aes_key"].as<std::string>(); 
        cactus::aes_make_key_and_iv(key_filename);
        return 0;
    }    

    if (vmap.count("aes-encrypt") && vmap.count("a-key") && vmap.count("a-iv") && vmap.count("a-source") && vmap.count("a-dest"))
    {
        std::string key = vmap["a-key"].as<std::string>();
        std::string iv = vmap["a-iv"].as<std::string>();
        std::string source_file = vmap["a-source"].as<std::string>();
        std::string dest_file = vmap["a-dest"].as<std::string>();
        cactus::aes_cbc_encrypt_file(source_file, dest_file, key, iv);
    }

    if (vmap.count("aes-decrypt") && vmap.count("a-key") && vmap.count("a-iv") && vmap.count("a-source") && vmap.count("a-dest"))
    {
        std::string key = vmap["a-key"].as<std::string>();
        std::string iv = vmap["a-iv"].as<std::string>();
        std::string source_file = vmap["a-source"].as<std::string>();
        std::string dest_file = vmap["a-dest"].as<std::string>();
        cactus::aes_cbc_decrypt_file(source_file, dest_file, key, iv);
    }

    /*
    byte key[16] = 
    { 0x30, 0x57, 0x6E, 0x2E, 0xD3, 0x57, 0x4C, 0x11, 0x67, 0x7A, 0x43, 0xFB, 0x1A, 0x73, 0x3F, 0x1B };
    byte iv[16] = 
    { 0x8C, 0x4A, 0xD6, 0x27, 0x98, 0x83, 0x2A, 0x1F, 0xE1, 0x61, 0x49, 0xFB, 0x90, 0x1F, 0xBA, 0x8E };


    // 使用key和iv数组加密
    cactus::aes_cbc_encrypt_file("test.ahk", "test.cbc.data", key, 16, iv, 16);

    // 使用16进制字符串的key和IV解密
    cactus::aes_cbc_decrypt_file("test.cbc.data", "test.cbc.data.ahk", "30576E2ED3574C11677A43FB1A733F1B", "8C4AD62798832A1FE16149FB901FBA8E");

    cactus::aes_cbc_decrypt_file("test.cbc.data", "test.cbc.data.ahk", "MFduLtNXTBFnekP7GnM/Gw==", "jErWJ5iDKh/hYUn7kB+6jg==", true);

    cactus::aes_ecb_encrypt_file("test.ahk", "test.ecb.data", key, 16);

    cactus::aes_ecb_decrypt_file("test.ecb.data", "test.ecb.data.ahk", key, 16);
    */
  
    return 0;
}

