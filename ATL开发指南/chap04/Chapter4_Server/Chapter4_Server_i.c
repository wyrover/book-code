/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 3.01.75 */
/* at Mon Oct 06 22:26:09 1997
 */
/* Compiler settings for Chapter4_Server.idl:
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

const IID IID_IMath = {0x5FB0C22E, 0x3343, 0x11D1, {0x88, 0x3A, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};


const IID IID_IAdvancedMath = {0x984D09A1, 0x3379, 0x11d1, {0x88, 0x3A, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};


const IID IID_IComponentInfo = {0x1E405AA0, 0x3396, 0x11d1, {0x88, 0x3A, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};


const IID IID_IMath2 = {0x984D09A2, 0x3379, 0x11d1, {0x88, 0x3A, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};


const IID LIBID_CHAPTER4_SERVERLib = {0x5FB0C221, 0x3343, 0x11D1, {0x88, 0x3A, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};


const CLSID CLSID_Math = {0x5FB0C22F, 0x3343, 0x11D1, {0x88, 0x3A, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};


#ifdef __cplusplus
}
#endif

