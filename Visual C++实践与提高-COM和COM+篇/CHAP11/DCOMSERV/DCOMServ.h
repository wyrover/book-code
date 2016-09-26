

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 6.00.0361 */
/* at Wed Jun 29 16:24:45 2005
 */
/* Compiler settings for E:\Software\BookCode\VCCOM\CHAP11\DCOMSERV\DCOMServ.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data
    VC __declspec() decoration level:
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
    #define __REQUIRED_RPCNDR_H_VERSION__ 475
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

#ifndef __DCOMServ_h__
#define __DCOMServ_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
    #pragma once
#endif

/* Forward Declarations */

#ifndef __IQuoteSupplier_FWD_DEFINED__
    #define __IQuoteSupplier_FWD_DEFINED__
    typedef interface IQuoteSupplier IQuoteSupplier;
#endif  /* __IQuoteSupplier_FWD_DEFINED__ */


#ifndef __QuoteSupplier_FWD_DEFINED__
    #define __QuoteSupplier_FWD_DEFINED__

    #ifdef __cplusplus
        typedef class QuoteSupplier QuoteSupplier;
    #else
        typedef struct QuoteSupplier QuoteSupplier;
    #endif /* __cplusplus */

#endif  /* __QuoteSupplier_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C" {
#endif

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free(void *);

#ifndef __IQuoteSupplier_INTERFACE_DEFINED__
#define __IQuoteSupplier_INTERFACE_DEFINED__

/* interface IQuoteSupplier */
/* [unique][helpstring][dual][uuid][object] */


EXTERN_C const IID IID_IQuoteSupplier;

#if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("03A51633-854E-4942-9C28-6A3C9435B542")
IQuoteSupplier :
public IDispatch {
public:
    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetQuote(
        /* [in] */ BSTR Ticker,
        /* [retval][out] */ double * Price) = 0;

};

#else   /* C style interface */

typedef struct IQuoteSupplierVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE *QueryInterface)(
        IQuoteSupplier * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void **ppvObject);

    ULONG(STDMETHODCALLTYPE *AddRef)(
        IQuoteSupplier * This);

    ULONG(STDMETHODCALLTYPE *Release)(
        IQuoteSupplier * This);

    HRESULT(STDMETHODCALLTYPE *GetTypeInfoCount)(
        IQuoteSupplier * This,
        /* [out] */ UINT *pctinfo);

    HRESULT(STDMETHODCALLTYPE *GetTypeInfo)(
        IQuoteSupplier * This,
        /* [in] */ UINT iTInfo,
        /* [in] */ LCID lcid,
        /* [out] */ ITypeInfo **ppTInfo);

    HRESULT(STDMETHODCALLTYPE *GetIDsOfNames)(
        IQuoteSupplier * This,
        /* [in] */ REFIID riid,
        /* [size_is][in] */ LPOLESTR *rgszNames,
        /* [in] */ UINT cNames,
        /* [in] */ LCID lcid,
        /* [size_is][out] */ DISPID *rgDispId);

    /* [local] */ HRESULT(STDMETHODCALLTYPE *Invoke)(
        IQuoteSupplier * This,
        /* [in] */ DISPID dispIdMember,
        /* [in] */ REFIID riid,
        /* [in] */ LCID lcid,
        /* [in] */ WORD wFlags,
        /* [out][in] */ DISPPARAMS *pDispParams,
        /* [out] */ VARIANT *pVarResult,
        /* [out] */ EXCEPINFO *pExcepInfo,
        /* [out] */ UINT *puArgErr);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE *GetQuote)(
        IQuoteSupplier * This,
        /* [in] */ BSTR Ticker,
        /* [retval][out] */ double *Price);

    END_INTERFACE
} IQuoteSupplierVtbl;

interface IQuoteSupplier {
    CONST_VTBL struct IQuoteSupplierVtbl *lpVtbl;
};



#ifdef COBJMACROS


#define IQuoteSupplier_QueryInterface(This,riid,ppvObject)  \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IQuoteSupplier_AddRef(This) \
    (This)->lpVtbl -> AddRef(This)

#define IQuoteSupplier_Release(This)    \
    (This)->lpVtbl -> Release(This)


#define IQuoteSupplier_GetTypeInfoCount(This,pctinfo)   \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IQuoteSupplier_GetTypeInfo(This,iTInfo,lcid,ppTInfo)    \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IQuoteSupplier_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)  \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IQuoteSupplier_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)    \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IQuoteSupplier_GetQuote(This,Ticker,Price)  \
    (This)->lpVtbl -> GetQuote(This,Ticker,Price)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IQuoteSupplier_GetQuote_Proxy(
    IQuoteSupplier * This,
    /* [in] */ BSTR Ticker,
    /* [retval][out] */ double *Price);


void __RPC_STUB IQuoteSupplier_GetQuote_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __IQuoteSupplier_INTERFACE_DEFINED__ */



#ifndef __DCOMSERVLib_LIBRARY_DEFINED__
#define __DCOMSERVLib_LIBRARY_DEFINED__

/* library DCOMSERVLib */
/* [helpstring][version][uuid] */


EXTERN_C const IID LIBID_DCOMSERVLib;

EXTERN_C const CLSID CLSID_QuoteSupplier;

#ifdef __cplusplus

class DECLSPEC_UUID("53437B42-6DD0-42D9-AF06-20E5F45AC2B8")
    QuoteSupplier;
#endif
#endif /* __DCOMSERVLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(unsigned long *, unsigned long            , BSTR *);
unsigned char * __RPC_USER  BSTR_UserMarshal(unsigned long *, unsigned char *, BSTR *);
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR *);
void                      __RPC_USER  BSTR_UserFree(unsigned long *, BSTR *);

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


