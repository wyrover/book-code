#ifndef dccrypt_h__
#define dccrypt_h__

namespace dcs {

	enum aes_encrypt_mode {
		AES_ENC_ECB,
		AES_ENC_CBC,
		AES_ENC_CFB,
		AES_ENC_OFB,
	};
	//ECB,CBC, CFB, OFB
	void *  aes_create(const unsigned char * key, int key_bytes = 16, aes_encrypt_mode mode = AES_ENC_CBC);
	int     aes_destroy(void * aes);
	int     pkcs7_padding_size(int ibuffer, unsigned char align);
	int     pkcs7_unpadding_size(const unsigned char * buffer, int ibuffer, unsigned char align);
	int     pkcs7_padding(unsigned char * buffer, int ibuffer, unsigned char align);
	int     aes_encrypt(void * aes_, unsigned char * buffer, const unsigned char * data, int idata);
	int     aes_decrypt(void * aes_, unsigned char * buffer, const unsigned char * data, int idata);


}

#endif // dccrypt_h__
