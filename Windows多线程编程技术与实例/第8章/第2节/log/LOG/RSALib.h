/*************************************************************************

 *	Copyright (c) Siwei Century R&D All rights reserved.
 
	This is unpublished proprietary source code of Siwei Century R&D
 
	The copyright notice above does not evidence any actual or
	intended publication of such source code.

 ************************************************************************/

/*************************************************************************

 * 	Module Name:	RSALib.h
 * 	Description:	Header for RSA sign/verify library
 * 	Notes:		None
 * 	Author:		crying
 * 	Date:		2004/03/22
 * 	Reference:	None

 ************************************************************************/

/*************************************************************************

 * 	Modification history:
  
   	=====================
   	Date:		   
   	By:			   
   	Reason:		   

 ************************************************************************/

#include <conio.h>
#include <windows.h>


/////////=================================================================
////	Define
#define	RSALIB_ERR_SUCCESS	0x00
#define	RSALIB_ERR_PARAMETER	0x01

#define	RSALIB_ERR_FILEOPEN	0x11
#define	RSALIB_ERR_FILEWRITE	0x12
#define	RSALIB_ERR_FILEREAD	0x13

#define	RSALIB_ERR_GENPRIME	0xA1

#define	RSALIB_LEY_LENGTH	1024	// use 1024 bit key default
#define	RSALIB_DA_MD5		5	// use MD5 DA default

/////////=================================================================
////	Interface
////
////	If the operation succeeds, returns RSALIB_ERR_SUCCESS
////	else, returns RSALIB_ERR_XXX(error code)

#ifdef __cplusplus
extern "C" {
#endif


/*
Sign a file
	prikey_file:	path and file name of the private key
	sign_file:	path and file name of the file to sign
	sig_file:	path and file name of the signature
*/
int APIENTRY DoSign(char *prikey_file, char *sign_file, char *sig_file);

/*
Verify a file
	pubkey_file:	path and name of the public key
	verify_file:	path and file name of the file to verify
	sig_file:	path and file name of the signature
*/
int APIENTRY DoVerify(char *pubkey_file, char *verify_file, char *sig_file);

/*
Verify a file
	pubkey_file:	path and name of the public key
	verify_file:	path and name of the private key
*/
int APIENTRY DoGenerateKeyfile(char *pubkey_file, char *prikey_file);

#ifdef __cplusplus
}
#endif




