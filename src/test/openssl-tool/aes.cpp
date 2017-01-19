#include "aes.h"

#include <cassert>
#include <iostream>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <cstring>
#include <vector>

#include "encoding.hh"
#include "xor.hh"

using namespace std;

vector<byte> AES::pkcs7_pad( vector<byte> block, unsigned int padded_length ) {
  if ( block.size() == padded_length ) return block;
  
  vector<byte> ret = block;
  unsigned int pad = padded_length - block.size();
  for ( unsigned int i = 0; i < pad; i++ ) {
    ret.push_back( pad );
  }

  return ret;
}

vector<byte> AES::zero_aes_iv() {
  vector<byte> res;
  for ( unsigned int i = 0; i < AES_BLOCK_SIZE; i++ ) res.push_back( 0 );
  return res;
}

vector<byte> AES::random_aes_iv() {
  byte res_buf[AES_BLOCK_SIZE];

  RAND_bytes( res_buf, AES_BLOCK_SIZE );

  vector<byte> res;
  res.insert( res.end(), res_buf, res_buf + AES_BLOCK_SIZE );
  return res;
}

AES::aeskey AES::random_aes_key() {
  byte res_buf[AES_BLOCK_SIZE];

  RAND_bytes( res_buf, AES_BLOCK_SIZE );

  vector<byte> res;
  res.insert( res.end(), res_buf, res_buf + AES_BLOCK_SIZE );
  return res;
}

string AES::decrypt_aes_ecb( vector<byte> encrypted, const string key ) {
  string res = "";
  
  for ( unsigned int i = 0; i < encrypted.size(); i += AES_BLOCK_SIZE ) {
    vector<byte> byte_vec( encrypted.begin() + i, encrypted.begin() + i + AES_BLOCK_SIZE );

    unsigned char* enc_buf = new unsigned char[byte_vec.size()];
    for ( unsigned int i = 0; i < byte_vec.size(); i++ ) {
      enc_buf[i] = byte_vec[i];
    }
    
    unsigned char* dec_buf = new unsigned char[byte_vec.size()];
    char* key_buf = new char[key.size()];
    strcpy( key_buf, key.c_str() );
    
    AES_KEY aes_key;
    AES_set_decrypt_key( reinterpret_cast<unsigned char*>( key_buf ), 128, &aes_key );
    AES_decrypt( enc_buf, dec_buf, &aes_key );

    delete[] enc_buf;
    delete[] key_buf;

    string dec_str( reinterpret_cast<char*>( dec_buf ), AES_BLOCK_SIZE );
    res += dec_str;
    delete[] dec_buf;
  }

  return res;
}

vector<byte> AES::encrypt_aes_ecb( const vector<byte> & plaintext, const string key ) {
  vector<byte> res;

  for ( unsigned int i = 0; i < plaintext.size(); i += AES_BLOCK_SIZE ) {
    vector<byte> byte_vec( plaintext.begin() + i, plaintext.begin() + i + AES_BLOCK_SIZE );

    unsigned char* enc_buf = new unsigned char[byte_vec.size()];
    for ( unsigned int i = 0; i < byte_vec.size(); i++ ) {
      enc_buf[i] = byte_vec[i];
    }
    
    unsigned char* dec_buf = new unsigned char[byte_vec.size()];
    char* key_buf = new char[key.size()];
    strcpy( key_buf, key.c_str() );
    
    AES_KEY aes_key;
    AES_set_encrypt_key( reinterpret_cast<unsigned char*>( key_buf ), 128, &aes_key );
    AES_encrypt( enc_buf, dec_buf, &aes_key );

    delete[] enc_buf;
    delete[] key_buf;

    res.insert( res.end(), dec_buf, dec_buf + AES_BLOCK_SIZE );
    delete[] dec_buf;
  }

  return res;
}

string AES::decrypt_aes_cbc( vector<byte> encrypted, vector<byte> iv, const string key ) {
  string res = "";
  vector<byte> next_xor_block = iv;
  for ( unsigned int i = 0; i < encrypted.size(); i += AES_BLOCK_SIZE ) {
    vector<byte> block( encrypted.begin() + i, encrypted.begin() + i + AES_BLOCK_SIZE );
    auto dec_block = Encoding::ascii_to_bytes( AES::decrypt_aes_ecb( block, key ) );
    res += Encoding::bytes_to_ascii( XOR::xor_bytes( next_xor_block, dec_block ) );
    next_xor_block = block;
  }

  return res;
}

vector<byte> AES::encrypt_aes_cbc( const vector<byte> & plaintext, vector<byte> iv, const string key ) {
  vector<byte> res;

  vector<byte> next_xor_block = iv;
  for ( unsigned int i = 0; i < plaintext.size(); i += AES_BLOCK_SIZE ) {
    vector<byte> block( plaintext.begin() + i, plaintext.begin() + i + AES_BLOCK_SIZE );
    auto xor_block = XOR::xor_bytes( next_xor_block, block );
    auto enc_block = AES::encrypt_aes_ecb( xor_block, key );
    res.insert( res.end(), enc_block.begin(), enc_block.end() );
    next_xor_block = enc_block;
  }

  return res;
}