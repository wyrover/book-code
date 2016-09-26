/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 3.01.75 */
/* at Sun Nov 30 02:10:33 1997
 */
/* Compiler settings for Chapter10_AptServer.idl:
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

const IID IID_IMath = {0x55219411, 0x6956, 0x11D1, {0x80, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};


const IID LIBID_CHAPTER10_APTSERVERLib = {0x55219404, 0x6956, 0x11D1, {0x80, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};


const CLSID CLSID_Math = {0x55219412, 0x6956, 0x11D1, {0x80, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};


#ifdef __cplusplus
}
#endif

