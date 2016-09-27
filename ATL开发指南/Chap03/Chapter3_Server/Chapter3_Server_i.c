/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 3.01.75 */
/* at Sun Oct 19 18:54:28 1997
 */
/* Compiler settings for Chapter3_Server.idl:
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

const IID IID_IMath = {0x8812699C, 0x1CC8, 0x11D1, {0x88, 0x3A, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};


const IID IID_IAdvancedMath = {0x6AF3DF1E, 0xC48F, 0x11D0, {0xA7, 0x69, 0xD4, 0x77, 0xA4, 0x00, 0x00, 0x00}};


const IID LIBID_CHAPTER3_SERVERLib = {0x8812698E, 0x1CC8, 0x11D1, {0x88, 0x3A, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};


const CLSID CLSID_Math = {0x8812699D, 0x1CC8, 0x11D1, {0x88, 0x3A, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};


#ifdef __cplusplus
}
#endif

