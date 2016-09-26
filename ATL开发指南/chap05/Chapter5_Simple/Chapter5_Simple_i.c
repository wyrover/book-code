/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 3.01.75 */
/* at Thu Oct 09 20:10:47 1997
 */
/* Compiler settings for Chapter5_Simple.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: none
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

const IID IID_IMath = {0x380F79EA, 0x4098, 0x11D1, {0x88, 0x3A, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};


const IID LIBID_CHAPTER5_SIMPLELib = {0x380F79DD, 0x4098, 0x11D1, {0x88, 0x3A, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};


const CLSID CLSID_SimpleMath = {0x380F79EB, 0x4098, 0x11D1, {0x88, 0x3A, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};


#ifdef __cplusplus
}
#endif

