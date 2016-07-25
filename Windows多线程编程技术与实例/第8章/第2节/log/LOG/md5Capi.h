// md5Capi.h: interface for the Cmd5Capi class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MD5CAPI_H__438D2BEF_6F1B_4C5C_830F_0E7B6D1FD7E2__INCLUDED_)
#define AFX_MD5CAPI_H__438D2BEF_6F1B_4C5C_830F_0E7B6D1FD7E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define _WIN32_WINNT 0x0400

class Cmd5Capi  
{
public:
	BOOL Digest(CString & csBuffer);
	CString &GetDigest(void);

	Cmd5Capi(CString & csBuffer);
	Cmd5Capi();
	virtual ~Cmd5Capi();
	CString	csDigest;
};




#endif // !defined(AFX_MD5CAPI_H__438D2BEF_6F1B_4C5C_830F_0E7B6D1FD7E2__INCLUDED_)
