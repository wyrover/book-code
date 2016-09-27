/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Mon Aug 28 14:47:08 2000
 */
/* Compiler settings for D:\Samples\step5\Bank.idl:
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

#ifndef __Bank_h__
#define __Bank_h__

#ifdef __cplusplus
extern "C" {
#endif

/* Forward Declarations */

#ifndef __IAccount_FWD_DEFINED__
#define __IAccount_FWD_DEFINED__
typedef interface IAccount IAccount;
#endif  /* __IAccount_FWD_DEFINED__ */


#ifndef __IMoveMoney_FWD_DEFINED__
#define __IMoveMoney_FWD_DEFINED__
typedef interface IMoveMoney IMoveMoney;
#endif  /* __IMoveMoney_FWD_DEFINED__ */


#ifndef __IReceipt_FWD_DEFINED__
#define __IReceipt_FWD_DEFINED__
typedef interface IReceipt IReceipt;
#endif  /* __IReceipt_FWD_DEFINED__ */


#ifndef __Account_FWD_DEFINED__
#define __Account_FWD_DEFINED__

#ifdef __cplusplus
typedef class Account Account;
#else
typedef struct Account Account;
#endif /* __cplusplus */

#endif  /* __Account_FWD_DEFINED__ */


#ifndef __MoveMoney_FWD_DEFINED__
#define __MoveMoney_FWD_DEFINED__

#ifdef __cplusplus
typedef class MoveMoney MoveMoney;
#else
typedef struct MoveMoney MoveMoney;
#endif /* __cplusplus */

#endif  /* __MoveMoney_FWD_DEFINED__ */


#ifndef __Receipt_FWD_DEFINED__
#define __Receipt_FWD_DEFINED__

#ifdef __cplusplus
typedef class Receipt Receipt;
#else
typedef struct Receipt Receipt;
#endif /* __cplusplus */

#endif  /* __Receipt_FWD_DEFINED__ */


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

MIDL_INTERFACE("5939F230-7B5F-11D4-8BEC-348FBB000000")
IAccount :
public IDispatch {
public:
    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Post(
        /* [in] */ long lAccount,
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

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Post)(
        IAccount __RPC_FAR * This,
        /* [in] */ long lAccount,
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


#define IAccount_Post(This,lAccount,lAmount,pbstrResult)    \
    (This)->lpVtbl -> Post(This,lAccount,lAmount,pbstrResult)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAccount_Post_Proxy(
    IAccount __RPC_FAR * This,
    /* [in] */ long lAccount,
    /* [in] */ long lAmount,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrResult);


void __RPC_STUB IAccount_Post_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __IAccount_INTERFACE_DEFINED__ */


#ifndef __IMoveMoney_INTERFACE_DEFINED__
#define __IMoveMoney_INTERFACE_DEFINED__

/* interface IMoveMoney */
/* [unique][helpstring][dual][uuid][object] */


EXTERN_C const IID IID_IMoveMoney;

#if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("8D7F75C3-7BCE-11D4-8BF0-F6B5E8000000")
IMoveMoney :
public IDispatch {
public:
    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Perform(
        /* [in] */ long lPrimeAccount,
        /* [in] */ long lSecondAccount,
        /* [in] */ long lAmount,
        /* [in] */ long lTranType,
        /* [retval][out] */ BSTR __RPC_FAR * pbstrResult) = 0;

};

#else   /* C style interface */

typedef struct IMoveMoneyVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *QueryInterface)(
        IMoveMoney __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *AddRef)(
        IMoveMoney __RPC_FAR * This);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *Release)(
        IMoveMoney __RPC_FAR * This);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount)(
        IMoveMoney __RPC_FAR * This,
        /* [out] */ UINT __RPC_FAR *pctinfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo)(
        IMoveMoney __RPC_FAR * This,
        /* [in] */ UINT iTInfo,
        /* [in] */ LCID lcid,
        /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames)(
        IMoveMoney __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
        /* [in] */ UINT cNames,
        /* [in] */ LCID lcid,
        /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);

    /* [local] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Invoke)(
        IMoveMoney __RPC_FAR * This,
        /* [in] */ DISPID dispIdMember,
        /* [in] */ REFIID riid,
        /* [in] */ LCID lcid,
        /* [in] */ WORD wFlags,
        /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
        /* [out] */ VARIANT __RPC_FAR *pVarResult,
        /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
        /* [out] */ UINT __RPC_FAR *puArgErr);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Perform)(
        IMoveMoney __RPC_FAR * This,
        /* [in] */ long lPrimeAccount,
        /* [in] */ long lSecondAccount,
        /* [in] */ long lAmount,
        /* [in] */ long lTranType,
        /* [retval][out] */ BSTR __RPC_FAR *pbstrResult);

    END_INTERFACE
} IMoveMoneyVtbl;

interface IMoveMoney {
    CONST_VTBL struct IMoveMoneyVtbl __RPC_FAR *lpVtbl;
};



#ifdef COBJMACROS


#define IMoveMoney_QueryInterface(This,riid,ppvObject)  \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMoveMoney_AddRef(This) \
    (This)->lpVtbl -> AddRef(This)

#define IMoveMoney_Release(This)    \
    (This)->lpVtbl -> Release(This)


#define IMoveMoney_GetTypeInfoCount(This,pctinfo)   \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMoveMoney_GetTypeInfo(This,iTInfo,lcid,ppTInfo)    \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMoveMoney_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)  \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMoveMoney_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)    \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IMoveMoney_Perform(This,lPrimeAccount,lSecondAccount,lAmount,lTranType,pbstrResult) \
    (This)->lpVtbl -> Perform(This,lPrimeAccount,lSecondAccount,lAmount,lTranType,pbstrResult)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMoveMoney_Perform_Proxy(
    IMoveMoney __RPC_FAR * This,
    /* [in] */ long lPrimeAccount,
    /* [in] */ long lSecondAccount,
    /* [in] */ long lAmount,
    /* [in] */ long lTranType,
    /* [retval][out] */ BSTR __RPC_FAR *pbstrResult);


void __RPC_STUB IMoveMoney_Perform_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __IMoveMoney_INTERFACE_DEFINED__ */


#ifndef __IReceipt_INTERFACE_DEFINED__
#define __IReceipt_INTERFACE_DEFINED__

/* interface IReceipt */
/* [unique][helpstring][dual][uuid][object] */


EXTERN_C const IID IID_IReceipt;

#if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("FA1CAAD3-7CAB-11D4-8BF1-02A54B000000")
IReceipt :
public IDispatch {
public:
    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetNextReceipt(
        /* [retval][out] */ long __RPC_FAR * plReceiptNo) = 0;

};

#else   /* C style interface */

typedef struct IReceiptVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *QueryInterface)(
        IReceipt __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *AddRef)(
        IReceipt __RPC_FAR * This);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *Release)(
        IReceipt __RPC_FAR * This);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount)(
        IReceipt __RPC_FAR * This,
        /* [out] */ UINT __RPC_FAR *pctinfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo)(
        IReceipt __RPC_FAR * This,
        /* [in] */ UINT iTInfo,
        /* [in] */ LCID lcid,
        /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames)(
        IReceipt __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
        /* [in] */ UINT cNames,
        /* [in] */ LCID lcid,
        /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);

    /* [local] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Invoke)(
        IReceipt __RPC_FAR * This,
        /* [in] */ DISPID dispIdMember,
        /* [in] */ REFIID riid,
        /* [in] */ LCID lcid,
        /* [in] */ WORD wFlags,
        /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
        /* [out] */ VARIANT __RPC_FAR *pVarResult,
        /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
        /* [out] */ UINT __RPC_FAR *puArgErr);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetNextReceipt)(
        IReceipt __RPC_FAR * This,
        /* [retval][out] */ long __RPC_FAR *plReceiptNo);

    END_INTERFACE
} IReceiptVtbl;

interface IReceipt {
    CONST_VTBL struct IReceiptVtbl __RPC_FAR *lpVtbl;
};



#ifdef COBJMACROS


#define IReceipt_QueryInterface(This,riid,ppvObject)    \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IReceipt_AddRef(This)   \
    (This)->lpVtbl -> AddRef(This)

#define IReceipt_Release(This)  \
    (This)->lpVtbl -> Release(This)


#define IReceipt_GetTypeInfoCount(This,pctinfo) \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IReceipt_GetTypeInfo(This,iTInfo,lcid,ppTInfo)  \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IReceipt_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)    \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IReceipt_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)  \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IReceipt_GetNextReceipt(This,plReceiptNo)   \
    (This)->lpVtbl -> GetNextReceipt(This,plReceiptNo)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IReceipt_GetNextReceipt_Proxy(
    IReceipt __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *plReceiptNo);


void __RPC_STUB IReceipt_GetNextReceipt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __IReceipt_INTERFACE_DEFINED__ */



#ifndef __BANKLib_LIBRARY_DEFINED__
#define __BANKLib_LIBRARY_DEFINED__

/* library BANKLib */
/* [helpstring][version][uuid] */


EXTERN_C const IID LIBID_BANKLib;

EXTERN_C const CLSID CLSID_Account;

#ifdef __cplusplus

class DECLSPEC_UUID("5939F232-7B5F-11D4-8BEC-348FBB000000")
    Account;
#endif

EXTERN_C const CLSID CLSID_MoveMoney;

#ifdef __cplusplus

class DECLSPEC_UUID("8D7F75C4-7BCE-11D4-8BF0-F6B5E8000000")
    MoveMoney;
#endif

EXTERN_C const CLSID CLSID_Receipt;

#ifdef __cplusplus

class DECLSPEC_UUID("FA1CAAD4-7CAB-11D4-8BF1-02A54B000000")
    Receipt;
#endif
#endif /* __BANKLib_LIBRARY_DEFINED__ */

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
