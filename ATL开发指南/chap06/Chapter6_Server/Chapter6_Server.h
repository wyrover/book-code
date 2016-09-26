/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 3.01.75 */
/* at Wed Oct 29 19:50:22 1997
 */
/* Compiler settings for Chapter6_Server.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: none
*/
//@@MIDL_FILE_HEADING(  )
#include "rpc.h"
#include "rpcndr.h"
#ifndef COM_NO_WINDOWS_H
    #include "windows.h"
    #include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __Chapter6_Server_h__
#define __Chapter6_Server_h__

#ifdef __cplusplus
extern "C" {
#endif

/* Forward Declarations */

#ifndef __IMath_FWD_DEFINED__
#define __IMath_FWD_DEFINED__
typedef interface IMath IMath;
#endif  /* __IMath_FWD_DEFINED__ */


#ifndef __IMath2_FWD_DEFINED__
#define __IMath2_FWD_DEFINED__
typedef interface IMath2 IMath2;
#endif  /* __IMath2_FWD_DEFINED__ */


#ifndef __IAdvancedMath_FWD_DEFINED__
#define __IAdvancedMath_FWD_DEFINED__
typedef interface IAdvancedMath IAdvancedMath;
#endif  /* __IAdvancedMath_FWD_DEFINED__ */


#ifndef __Math_FWD_DEFINED__
#define __Math_FWD_DEFINED__

#ifdef __cplusplus
typedef class Math Math;
#else
typedef struct Math Math;
#endif /* __cplusplus */

#endif  /* __Math_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free(void __RPC_FAR *);

#ifndef __IMath_INTERFACE_DEFINED__
#define __IMath_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IMath
 * at Wed Oct 29 19:50:22 1997
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */



EXTERN_C const IID IID_IMath;

#if defined(__cplusplus) && !defined(CINTERFACE)

interface DECLSPEC_UUID("DCA4F88E-4952-11D1-883A-444553540000")
IMath :
public IDispatch {
public:
    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Add(
        /* [in] */ VARIANT varOp1,
        /* [in] */ VARIANT varOp2,
        /* [retval][out] */ VARIANT __RPC_FAR * pvarResult) = 0;

    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Subtract(
        /* [in] */ VARIANT varOp1,
        /* [in] */ VARIANT varOp2,
        /* [retval][out] */ VARIANT __RPC_FAR * pvarResult) = 0;

    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Multiply(
        /* [in] */ VARIANT varOp1,
        /* [in] */ VARIANT varOp2,
        /* [retval][out] */ VARIANT __RPC_FAR * pvarResult) = 0;

    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Divide(
        /* [in] */ VARIANT varOp1,
        /* [in] */ VARIANT varOp2,
        /* [retval][out] */ VARIANT __RPC_FAR * pvarResult) = 0;

};

#else   /* C style interface */

typedef struct IMathVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *QueryInterface)(
        IMath __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *AddRef)(
        IMath __RPC_FAR * This);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *Release)(
        IMath __RPC_FAR * This);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount)(
        IMath __RPC_FAR * This,
        /* [out] */ UINT __RPC_FAR *pctinfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo)(
        IMath __RPC_FAR * This,
        /* [in] */ UINT iTInfo,
        /* [in] */ LCID lcid,
        /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames)(
        IMath __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
        /* [in] */ UINT cNames,
        /* [in] */ LCID lcid,
        /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);

    /* [local] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Invoke)(
        IMath __RPC_FAR * This,
        /* [in] */ DISPID dispIdMember,
        /* [in] */ REFIID riid,
        /* [in] */ LCID lcid,
        /* [in] */ WORD wFlags,
        /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
        /* [out] */ VARIANT __RPC_FAR *pVarResult,
        /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
        /* [out] */ UINT __RPC_FAR *puArgErr);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Add)(
        IMath __RPC_FAR * This,
        /* [in] */ VARIANT varOp1,
        /* [in] */ VARIANT varOp2,
        /* [retval][out] */ VARIANT __RPC_FAR *pvarResult);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Subtract)(
        IMath __RPC_FAR * This,
        /* [in] */ VARIANT varOp1,
        /* [in] */ VARIANT varOp2,
        /* [retval][out] */ VARIANT __RPC_FAR *pvarResult);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Multiply)(
        IMath __RPC_FAR * This,
        /* [in] */ VARIANT varOp1,
        /* [in] */ VARIANT varOp2,
        /* [retval][out] */ VARIANT __RPC_FAR *pvarResult);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Divide)(
        IMath __RPC_FAR * This,
        /* [in] */ VARIANT varOp1,
        /* [in] */ VARIANT varOp2,
        /* [retval][out] */ VARIANT __RPC_FAR *pvarResult);

    END_INTERFACE
} IMathVtbl;

interface IMath {
    CONST_VTBL struct IMathVtbl __RPC_FAR *lpVtbl;
};



#ifdef COBJMACROS


#define IMath_QueryInterface(This,riid,ppvObject)   \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMath_AddRef(This)  \
    (This)->lpVtbl -> AddRef(This)

#define IMath_Release(This) \
    (This)->lpVtbl -> Release(This)


#define IMath_GetTypeInfoCount(This,pctinfo)    \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMath_GetTypeInfo(This,iTInfo,lcid,ppTInfo) \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMath_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)   \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMath_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IMath_Add(This,varOp1,varOp2,pvarResult)    \
    (This)->lpVtbl -> Add(This,varOp1,varOp2,pvarResult)

#define IMath_Subtract(This,varOp1,varOp2,pvarResult)   \
    (This)->lpVtbl -> Subtract(This,varOp1,varOp2,pvarResult)

#define IMath_Multiply(This,varOp1,varOp2,pvarResult)   \
    (This)->lpVtbl -> Multiply(This,varOp1,varOp2,pvarResult)

#define IMath_Divide(This,varOp1,varOp2,pvarResult) \
    (This)->lpVtbl -> Divide(This,varOp1,varOp2,pvarResult)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMath_Add_Proxy(
    IMath __RPC_FAR * This,
    /* [in] */ VARIANT varOp1,
    /* [in] */ VARIANT varOp2,
    /* [retval][out] */ VARIANT __RPC_FAR *pvarResult);


void __RPC_STUB IMath_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMath_Subtract_Proxy(
    IMath __RPC_FAR * This,
    /* [in] */ VARIANT varOp1,
    /* [in] */ VARIANT varOp2,
    /* [retval][out] */ VARIANT __RPC_FAR *pvarResult);


void __RPC_STUB IMath_Subtract_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMath_Multiply_Proxy(
    IMath __RPC_FAR * This,
    /* [in] */ VARIANT varOp1,
    /* [in] */ VARIANT varOp2,
    /* [retval][out] */ VARIANT __RPC_FAR *pvarResult);


void __RPC_STUB IMath_Multiply_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMath_Divide_Proxy(
    IMath __RPC_FAR * This,
    /* [in] */ VARIANT varOp1,
    /* [in] */ VARIANT varOp2,
    /* [retval][out] */ VARIANT __RPC_FAR *pvarResult);


void __RPC_STUB IMath_Divide_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __IMath_INTERFACE_DEFINED__ */


#ifndef __IMath2_INTERFACE_DEFINED__
#define __IMath2_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IMath2
 * at Wed Oct 29 19:50:22 1997
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */



EXTERN_C const IID IID_IMath2;

#if defined(__cplusplus) && !defined(CINTERFACE)

interface DECLSPEC_UUID("9F21BD41-4E25-11d1-883A-444553540000")
IMath2 :
public IDispatch {
public:
    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Add(
        /* [in] */ VARIANT varOp1,
        /* [in] */ VARIANT varOp2,
        /* [retval][out] */ VARIANT __RPC_FAR * pvarResult) = 0;

    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Subtract(
        /* [in] */ VARIANT varOp1,
        /* [in] */ VARIANT varOp2,
        /* [retval][out] */ VARIANT __RPC_FAR * pvarResult) = 0;

    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Multiply(
        /* [in] */ VARIANT varOp1,
        /* [in] */ VARIANT varOp2,
        /* [retval][out] */ VARIANT __RPC_FAR * pvarResult) = 0;

    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Divide(
        /* [in] */ VARIANT varOp1,
        /* [in] */ VARIANT varOp2,
        /* [retval][out] */ VARIANT __RPC_FAR * pvarResult) = 0;

    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Sum(
        /* [in] */ VARIANT varOp1,
        /* [retval][out] */ long __RPC_FAR * plResult) = 0;

};

#else   /* C style interface */

typedef struct IMath2Vtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *QueryInterface)(
        IMath2 __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *AddRef)(
        IMath2 __RPC_FAR * This);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *Release)(
        IMath2 __RPC_FAR * This);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount)(
        IMath2 __RPC_FAR * This,
        /* [out] */ UINT __RPC_FAR *pctinfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo)(
        IMath2 __RPC_FAR * This,
        /* [in] */ UINT iTInfo,
        /* [in] */ LCID lcid,
        /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames)(
        IMath2 __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
        /* [in] */ UINT cNames,
        /* [in] */ LCID lcid,
        /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);

    /* [local] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Invoke)(
        IMath2 __RPC_FAR * This,
        /* [in] */ DISPID dispIdMember,
        /* [in] */ REFIID riid,
        /* [in] */ LCID lcid,
        /* [in] */ WORD wFlags,
        /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
        /* [out] */ VARIANT __RPC_FAR *pVarResult,
        /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
        /* [out] */ UINT __RPC_FAR *puArgErr);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Add)(
        IMath2 __RPC_FAR * This,
        /* [in] */ VARIANT varOp1,
        /* [in] */ VARIANT varOp2,
        /* [retval][out] */ VARIANT __RPC_FAR *pvarResult);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Subtract)(
        IMath2 __RPC_FAR * This,
        /* [in] */ VARIANT varOp1,
        /* [in] */ VARIANT varOp2,
        /* [retval][out] */ VARIANT __RPC_FAR *pvarResult);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Multiply)(
        IMath2 __RPC_FAR * This,
        /* [in] */ VARIANT varOp1,
        /* [in] */ VARIANT varOp2,
        /* [retval][out] */ VARIANT __RPC_FAR *pvarResult);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Divide)(
        IMath2 __RPC_FAR * This,
        /* [in] */ VARIANT varOp1,
        /* [in] */ VARIANT varOp2,
        /* [retval][out] */ VARIANT __RPC_FAR *pvarResult);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Sum)(
        IMath2 __RPC_FAR * This,
        /* [in] */ VARIANT varOp1,
        /* [retval][out] */ long __RPC_FAR *plResult);

    END_INTERFACE
} IMath2Vtbl;

interface IMath2 {
    CONST_VTBL struct IMath2Vtbl __RPC_FAR *lpVtbl;
};



#ifdef COBJMACROS


#define IMath2_QueryInterface(This,riid,ppvObject)  \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMath2_AddRef(This) \
    (This)->lpVtbl -> AddRef(This)

#define IMath2_Release(This)    \
    (This)->lpVtbl -> Release(This)


#define IMath2_GetTypeInfoCount(This,pctinfo)   \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMath2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)    \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMath2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)  \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMath2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)    \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IMath2_Add(This,varOp1,varOp2,pvarResult)   \
    (This)->lpVtbl -> Add(This,varOp1,varOp2,pvarResult)

#define IMath2_Subtract(This,varOp1,varOp2,pvarResult)  \
    (This)->lpVtbl -> Subtract(This,varOp1,varOp2,pvarResult)

#define IMath2_Multiply(This,varOp1,varOp2,pvarResult)  \
    (This)->lpVtbl -> Multiply(This,varOp1,varOp2,pvarResult)

#define IMath2_Divide(This,varOp1,varOp2,pvarResult)    \
    (This)->lpVtbl -> Divide(This,varOp1,varOp2,pvarResult)

#define IMath2_Sum(This,varOp1,plResult)    \
    (This)->lpVtbl -> Sum(This,varOp1,plResult)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMath2_Add_Proxy(
    IMath2 __RPC_FAR * This,
    /* [in] */ VARIANT varOp1,
    /* [in] */ VARIANT varOp2,
    /* [retval][out] */ VARIANT __RPC_FAR *pvarResult);


void __RPC_STUB IMath2_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMath2_Subtract_Proxy(
    IMath2 __RPC_FAR * This,
    /* [in] */ VARIANT varOp1,
    /* [in] */ VARIANT varOp2,
    /* [retval][out] */ VARIANT __RPC_FAR *pvarResult);


void __RPC_STUB IMath2_Subtract_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMath2_Multiply_Proxy(
    IMath2 __RPC_FAR * This,
    /* [in] */ VARIANT varOp1,
    /* [in] */ VARIANT varOp2,
    /* [retval][out] */ VARIANT __RPC_FAR *pvarResult);


void __RPC_STUB IMath2_Multiply_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMath2_Divide_Proxy(
    IMath2 __RPC_FAR * This,
    /* [in] */ VARIANT varOp1,
    /* [in] */ VARIANT varOp2,
    /* [retval][out] */ VARIANT __RPC_FAR *pvarResult);


void __RPC_STUB IMath2_Divide_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMath2_Sum_Proxy(
    IMath2 __RPC_FAR * This,
    /* [in] */ VARIANT varOp1,
    /* [retval][out] */ long __RPC_FAR *plResult);


void __RPC_STUB IMath2_Sum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __IMath2_INTERFACE_DEFINED__ */


#ifndef __IAdvancedMath_INTERFACE_DEFINED__
#define __IAdvancedMath_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IAdvancedMath
 * at Wed Oct 29 19:50:22 1997
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][dual][uuid][object] */



EXTERN_C const IID IID_IAdvancedMath;

#if defined(__cplusplus) && !defined(CINTERFACE)

interface DECLSPEC_UUID("9F21BD45-4E25-11d1-883A-444553540000")
IAdvancedMath :
public IDispatch {
public:
    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Factorial(
        /* [in] */ short sOp,
        /* [retval][out] */ long __RPC_FAR * plResult) = 0;

    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Fibonacci(
        /* [in] */ short sOp,
        /* [retval][out] */ long __RPC_FAR * plResult) = 0;

};

#else   /* C style interface */

typedef struct IAdvancedMathVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *QueryInterface)(
        IAdvancedMath __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *AddRef)(
        IAdvancedMath __RPC_FAR * This);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *Release)(
        IAdvancedMath __RPC_FAR * This);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount)(
        IAdvancedMath __RPC_FAR * This,
        /* [out] */ UINT __RPC_FAR *pctinfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo)(
        IAdvancedMath __RPC_FAR * This,
        /* [in] */ UINT iTInfo,
        /* [in] */ LCID lcid,
        /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames)(
        IAdvancedMath __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
        /* [in] */ UINT cNames,
        /* [in] */ LCID lcid,
        /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);

    /* [local] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Invoke)(
        IAdvancedMath __RPC_FAR * This,
        /* [in] */ DISPID dispIdMember,
        /* [in] */ REFIID riid,
        /* [in] */ LCID lcid,
        /* [in] */ WORD wFlags,
        /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
        /* [out] */ VARIANT __RPC_FAR *pVarResult,
        /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
        /* [out] */ UINT __RPC_FAR *puArgErr);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Factorial)(
        IAdvancedMath __RPC_FAR * This,
        /* [in] */ short sOp,
        /* [retval][out] */ long __RPC_FAR *plResult);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Fibonacci)(
        IAdvancedMath __RPC_FAR * This,
        /* [in] */ short sOp,
        /* [retval][out] */ long __RPC_FAR *plResult);

    END_INTERFACE
} IAdvancedMathVtbl;

interface IAdvancedMath {
    CONST_VTBL struct IAdvancedMathVtbl __RPC_FAR *lpVtbl;
};



#ifdef COBJMACROS


#define IAdvancedMath_QueryInterface(This,riid,ppvObject)   \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAdvancedMath_AddRef(This)  \
    (This)->lpVtbl -> AddRef(This)

#define IAdvancedMath_Release(This) \
    (This)->lpVtbl -> Release(This)


#define IAdvancedMath_GetTypeInfoCount(This,pctinfo)    \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IAdvancedMath_GetTypeInfo(This,iTInfo,lcid,ppTInfo) \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IAdvancedMath_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)   \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IAdvancedMath_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IAdvancedMath_Factorial(This,sOp,plResult)  \
    (This)->lpVtbl -> Factorial(This,sOp,plResult)

#define IAdvancedMath_Fibonacci(This,sOp,plResult)  \
    (This)->lpVtbl -> Fibonacci(This,sOp,plResult)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdvancedMath_Factorial_Proxy(
    IAdvancedMath __RPC_FAR * This,
    /* [in] */ short sOp,
    /* [retval][out] */ long __RPC_FAR *plResult);


void __RPC_STUB IAdvancedMath_Factorial_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdvancedMath_Fibonacci_Proxy(
    IAdvancedMath __RPC_FAR * This,
    /* [in] */ short sOp,
    /* [retval][out] */ long __RPC_FAR *plResult);


void __RPC_STUB IAdvancedMath_Fibonacci_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __IAdvancedMath_INTERFACE_DEFINED__ */



#ifndef __CHAPTER6_SERVERLib_LIBRARY_DEFINED__
#define __CHAPTER6_SERVERLib_LIBRARY_DEFINED__

/****************************************
 * Generated header for library: CHAPTER6_SERVERLib
 * at Wed Oct 29 19:50:22 1997
 * using MIDL 3.01.75
 ****************************************/
/* [helpstring][version][uuid] */



EXTERN_C const IID LIBID_CHAPTER6_SERVERLib;

#ifdef __cplusplus
EXTERN_C const CLSID CLSID_Math;

class DECLSPEC_UUID("DCA4F88F-4952-11D1-883A-444553540000")
    Math;
#endif
#endif /* __CHAPTER6_SERVERLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  VARIANT_UserSize(unsigned long __RPC_FAR *, unsigned long            , VARIANT __RPC_FAR *);
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserMarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR *);
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR *);
void                      __RPC_USER  VARIANT_UserFree(unsigned long __RPC_FAR *, VARIANT __RPC_FAR *);

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
