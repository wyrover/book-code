/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Fri Nov 05 14:54:02 1999
 */
/* Compiler settings for D:\My Documents\ATL_OLEDB_Prov\ATL_OLEDB_Prov.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
    #define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
    #error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
    #include "windows.h"
    #include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __ATL_OLEDB_Prov_h__
#define __ATL_OLEDB_Prov_h__

#ifdef __cplusplus
extern "C" {
#endif

/* Forward Declarations */

#ifndef __IDBCarusoeProvSource_FWD_DEFINED__
#define __IDBCarusoeProvSource_FWD_DEFINED__
typedef interface IDBCarusoeProvSource IDBCarusoeProvSource;
#endif  /* __IDBCarusoeProvSource_FWD_DEFINED__ */


#ifndef __CarusoeProv_FWD_DEFINED__
#define __CarusoeProv_FWD_DEFINED__

#ifdef __cplusplus
typedef class CarusoeProv CarusoeProv;
#else
typedef struct CarusoeProv CarusoeProv;
#endif /* __cplusplus */

#endif  /* __CarusoeProv_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free(void __RPC_FAR *);

#ifndef __IDBCarusoeProvSource_INTERFACE_DEFINED__
#define __IDBCarusoeProvSource_INTERFACE_DEFINED__

/* interface IDBCarusoeProvSource */
/* [unique][helpstring][uuid][object] */


EXTERN_C const IID IID_IDBCarusoeProvSource;

#if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("8DFCA8E0-791B-11d3-9D83-D79826594133")
IDBCarusoeProvSource :
public IUnknown {
public:
    virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetServerFileName(
        /* [out] */ BSTR __RPC_FAR * pBSTRFileName) = 0;

};

#else   /* C style interface */

typedef struct IDBCarusoeProvSourceVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *QueryInterface)(
        IDBCarusoeProvSource __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *AddRef)(
        IDBCarusoeProvSource __RPC_FAR * This);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *Release)(
        IDBCarusoeProvSource __RPC_FAR * This);

    /* [helpstring] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetServerFileName)(
        IDBCarusoeProvSource __RPC_FAR * This,
        /* [out] */ BSTR __RPC_FAR *pBSTRFileName);

    END_INTERFACE
} IDBCarusoeProvSourceVtbl;

interface IDBCarusoeProvSource {
    CONST_VTBL struct IDBCarusoeProvSourceVtbl __RPC_FAR *lpVtbl;
};



#ifdef COBJMACROS


#define IDBCarusoeProvSource_QueryInterface(This,riid,ppvObject)    \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDBCarusoeProvSource_AddRef(This)   \
    (This)->lpVtbl -> AddRef(This)

#define IDBCarusoeProvSource_Release(This)  \
    (This)->lpVtbl -> Release(This)


#define IDBCarusoeProvSource_GetServerFileName(This,pBSTRFileName)  \
    (This)->lpVtbl -> GetServerFileName(This,pBSTRFileName)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IDBCarusoeProvSource_GetServerFileName_Proxy(
    IDBCarusoeProvSource __RPC_FAR * This,
    /* [out] */ BSTR __RPC_FAR *pBSTRFileName);


void __RPC_STUB IDBCarusoeProvSource_GetServerFileName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __IDBCarusoeProvSource_INTERFACE_DEFINED__ */



#ifndef __ATL_OLEDB_PROVLib_LIBRARY_DEFINED__
#define __ATL_OLEDB_PROVLib_LIBRARY_DEFINED__

/* library ATL_OLEDB_PROVLib */
/* [helpstring][version][uuid] */


EXTERN_C const IID LIBID_ATL_OLEDB_PROVLib;

EXTERN_C const CLSID CLSID_CarusoeProv;

#ifdef __cplusplus

class DECLSPEC_UUID("4DAF1613-783B-11D3-9D83-DA68CA737833")
    CarusoeProv;
#endif
#endif /* __ATL_OLEDB_PROVLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR *);
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR *);
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR *);
void                      __RPC_USER  BSTR_UserFree(unsigned long __RPC_FAR *, BSTR __RPC_FAR *);

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
