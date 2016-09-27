/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sat Sep 02 12:16:59 2000
 */
/* Compiler settings for D:\samples\chap5\ATLSample\ATLSample.idl:
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

#ifndef __ATLSample_h__
#define __ATLSample_h__

#ifdef __cplusplus
extern "C" {
#endif

/* Forward Declarations */

#ifndef __IAccount_FWD_DEFINED__
#define __IAccount_FWD_DEFINED__
typedef interface IAccount IAccount;
#endif  /* __IAccount_FWD_DEFINED__ */


#ifndef __Account_FWD_DEFINED__
#define __Account_FWD_DEFINED__

#ifdef __cplusplus
typedef class Account Account;
#else
typedef struct Account Account;
#endif /* __cplusplus */

#endif  /* __Account_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free(void __RPC_FAR *);

#ifndef __IAccount_INTERFACE_DEFINED__
#define __IAccount_INTERFACE_DEFINED__

/* interface IAccount */
/* [unique][helpstring][dual][uuid][object] */


EXTERN_C const IID IID_IAccount;

#if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("3B4F8E7A-80C1-11D4-A23D-02608CACEDBB")
IAccount :
public IDispatch {
public:
    virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Balance(
        /* [retval][out] */ long __RPC_FAR * pVal) = 0;

    virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Balance(
        /* [in] */ long newVal) = 0;

    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Post(
        /* [in] */ long lAmount,
        /* [retval][out] */ BSTR __RPC_FAR * pbstrResult) = 0;

};

#else   /* C style interface */

typedef struct IAccountVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *QueryInterface)(
        IAccount __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *AddRef)(
        IAccount __RPC_FAR * This);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *Release)(
        IAccount __RPC_FAR * This);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount)(
        IAccount __RPC_FAR * This,
        /* [out] */ UINT __RPC_FAR *pctinfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo)(
        IAccount __RPC_FAR * This,
        /* [in] */ UINT iTInfo,
        /* [in] */ LCID lcid,
        /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames)(
        IAccount __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
        /* [in] */ UINT cNames,
        /* [in] */ LCID lcid,
        /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);

    /* [local] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Invoke)(
        IAccount __RPC_FAR * This,
        /* [in] */ DISPID dispIdMember,
        /* [in] */ REFIID riid,
        /* [in] */ LCID lcid,
        /* [in] */ WORD wFlags,
        /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
        /* [out] */ VARIANT __RPC_FAR *pVarResult,
        /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
        /* [out] */ UINT __RPC_FAR *puArgErr);

    /* [helpstring][id][propget] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *get_Balance)(
        IAccount __RPC_FAR * This,
        /* [retval][out] */ long __RPC_FAR *pVal);

    /* [helpstring][id][propput] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *put_Balance)(
        IAccount __RPC_FAR * This,
        /* [in] */ long newVal);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Post)(
        IAccount __RPC_FAR * This,
        /* [in] */ long lAmount,
        /* [retval][out] */ BSTR __RPC_FAR *pbstrResult);

    END_INTERFACE
} IAccountVtbl;

interface IAccount {
    CONST_VTBL struct IAccountVtbl __RPC_FAR *lpVtbl;
};



#ifdef COBJMACROS


#define IAccount_QueryInterface(This,riid,ppvObject)    \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAccount_AddRef(This)   \
    (This)->lpVtbl -> AddRef(This)

#define IAccount_Release(This)  \
    (This)->lpVtbl -> Release(This)


#define IAccount_GetTypeInfoCount(This,pctinfo) \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IAccount_GetTypeInfo(This,iTInfo,lcid,ppTInfo)  \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IAccount_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)    \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IAccount_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)  \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IAccount_get_Balance(This,pVal) \
    (This)->lpVtbl -> get_Balance(This,pVal)

#define IAccount_put_Balance(This,newVal)   \
    (This)->lpVtbl -> put_Balance(This,newVal)

#define IAccount_Post(This,lAmount,pbstrResult) \
    (This)->lpVtbl -> Post(This,lAmount,pbstrResult)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAccount_get_Balance_Proxy(
    IAccount __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IAccount_get_Balance_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IAccount_put_Balance_Proxy(
    IAccount __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB IAccount_put_Balance_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAccount_Post_Proxy(
    IAccount __RPC_FAR * This,
    /* [in] */ long lAmount,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrResult);


void __RPC_STUB IAccount_Post_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __IAccount_INTERFACE_DEFINED__ */



#ifndef __ATLSAMPLELib_LIBRARY_DEFINED__
#define __ATLSAMPLELib_LIBRARY_DEFINED__

/* library ATLSAMPLELib */
/* [helpstring][version][uuid] */


EXTERN_C const IID LIBID_ATLSAMPLELib;

EXTERN_C const CLSID CLSID_Account;

#ifdef __cplusplus

class DECLSPEC_UUID("3B4F8E7B-80C1-11D4-A23D-02608CACEDBB")
    Account;
#endif
#endif /* __ATLSAMPLELib_LIBRARY_DEFINED__ */

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
