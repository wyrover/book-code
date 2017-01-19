#ifndef dcrsa_h__
#define dcrsa_h__

#include <string>

namespace dcs {

	void * rsa_from_perm_pubkey(const char * file);
	void * rsa_from_perm_prvkey(const char * file);
	void * rsa_from_der_pubkey(const char * buff, int ibuff);
	void * rsa_from_der_prvkey(const char * buff, int ibuff);
	void   rsa_free(void * rsa);

	enum RSASignAlgoType {
		RSA_SIGN_SHA1,
		RSA_SIGN_SHA256,
	};

	bool    rsa_sign(std::string & signature, void * rsa, RSASignAlgoType meth, const char * buff, int ibuff);
	bool    rsa_verify(const std::string & signature, void * rsa, RSASignAlgoType meth, const char * buff, int ibuff);

}

#endif // dcrsa_h__
