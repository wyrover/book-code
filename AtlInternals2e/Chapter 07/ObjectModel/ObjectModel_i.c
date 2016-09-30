/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Mon Nov 23 01:11:40 1998
 */
/* Compiler settings for D:\ATLBook\src\Chapter 08\ObjectModel\ObjectModel.idl:
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

const IID IID_IDocument = {0xCB693B6C, 0x80E2, 0x11D2, {0x98, 0x7D, 0x00, 0x60, 0x08, 0x23, 0xCF, 0xFB}};


const IID IID_IDocuments = {0xCB693B6E, 0x80E2, 0x11D2, {0x98, 0x7D, 0x00, 0x60, 0x08, 0x23, 0xCF, 0xFB}};


const IID IID_IApplication = {0xCB693B70, 0x80E2, 0x11D2, {0x98, 0x7D, 0x00, 0x60, 0x08, 0x23, 0xCF, 0xFB}};


const IID LIBID_OBJECTMODELLib = {0xCB693B60, 0x80E2, 0x11D2, {0x98, 0x7D, 0x00, 0x60, 0x08, 0x23, 0xCF, 0xFB}};


const CLSID CLSID_Document = {0xCB693B6D, 0x80E2, 0x11D2, {0x98, 0x7D, 0x00, 0x60, 0x08, 0x23, 0xCF, 0xFB}};


const CLSID CLSID_Documents = {0xCB693B6F, 0x80E2, 0x11D2, {0x98, 0x7D, 0x00, 0x60, 0x08, 0x23, 0xCF, 0xFB}};


const CLSID CLSID_Application = {0xCB693B71, 0x80E2, 0x11D2, {0x98, 0x7D, 0x00, 0x60, 0x08, 0x23, 0xCF, 0xFB}};


#ifdef __cplusplus
}
#endif

