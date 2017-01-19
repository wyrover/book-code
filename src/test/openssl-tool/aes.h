#ifndef AES_HH
#define AES_HH

#include <string>
#include <vector>

//#include "types.hh"

#ifndef _BYTE
#define _BYTE
typedef unsigned char byte;
#endif


namespace AES {
  typedef std::vector<byte> aeskey;

  enum AES_MODE {
    ECB,
    CBC
  };
  
  std::vector<byte> pkcs7_pad( std::vector<byte> block, unsigned int padded_length );
  
  std::vector<byte> zero_aes_iv();
  std::vector<byte> random_aes_iv();
  
  aeskey random_aes_key();
  
  std::string decrypt_aes_ecb( std::vector<byte> encrypted, const std::string key );
  std::vector<byte> encrypt_aes_ecb( const std::vector<byte> & plaintext, const std::string key );
  
  std::string decrypt_aes_cbc( std::vector<byte> encrypted, std::vector<byte> iv,
                               const std::string key );
  std::vector<byte> encrypt_aes_cbc( const std::vector<byte> & plaintext, std::vector<byte> iv,
                                     const std::string key );
}

#endif
