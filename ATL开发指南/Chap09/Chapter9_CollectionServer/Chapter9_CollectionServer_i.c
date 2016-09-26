/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Wed Feb 23 21:51:23 2000
 */
/* Compiler settings for C:\projects\Books\atl\2ed\Examples\Chap09\Chapter9_CollectionServer\Chapter9_CollectionServer.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C" {
#endif


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID {
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_IMovie = {0xF579C5C1, 0x3777, 0x11d2, {0x88, 0x3A, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};


const IID IID_IMovieCollection = {0xF579C5C4, 0x3777, 0x11d2, {0x88, 0x3A, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};


const IID IID_IApplication = {0x56E642CF, 0xEA5F, 0x11D3, {0x83, 0xD5, 0x00, 0x60, 0x08, 0x1A, 0xEB, 0x5F}};


const IID LIBID_CHAPTER9_COLLECTIONSERVERLib = {0x56E642C3, 0xEA5F, 0x11D3, {0x83, 0xD5, 0x00, 0x60, 0x08, 0x1A, 0xEB, 0x5F}};


const CLSID CLSID_Application = {0x56E642D0, 0xEA5F, 0x11D3, {0x83, 0xD5, 0x00, 0x60, 0x08, 0x1A, 0xEB, 0x5F}};


#ifdef __cplusplus
}
#endif

