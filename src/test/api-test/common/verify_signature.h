#ifndef verify_signature_h__
#define verify_signature_h__

#include <Windows.h>
#include <string>


bool            verify_signature(const wchar_t* file_name);
std::wstring    get_signer(const wchar_t* file_name);

#endif // verify_signature_h__
