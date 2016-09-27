/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 3.01.75 */
/* at Tue Nov 04 17:55:33 1997
 */
/* Compiler settings for Chapter7_CallBackServer.idl:
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

#ifndef __Chapter7_CallBackServer_h__
#define __Chapter7_CallBackServer_h__

#ifdef __cplusplus
extern "C" {
#endif

/* Forward Declarations */

#ifndef __ICallBack_FWD_DEFINED__
#define __ICallBack_FWD_DEFINED__
typedef interface ICallBack ICallBack;
#endif  /* __ICallBack_FWD_DEFINED__ */


#ifndef __IMath_FWD_DEFINED__
#define __IMath_FWD_DEFINED__
typedef interface IMath IMath;
#endif  /* __IMath_FWD_DEFINED__ */


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

#ifndef __ICallBack_INTERFACE_DEFINED__
#define __ICallBack_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: ICallBack
 * at Tue Nov 04 17:55:33 1997
 * using MIDL 3.01.75
 ****************************************/
/* [helpstring][uuid][object] */



EXTERN_C const IID IID_ICallBack;

#if defined(__cplusplus) && !defined(CINTERFACE)

interface DECLSPEC_UUID("48CD3740-50A3-11d1-B5EC-0004ACFF171C")
ICallBack :
public IUnknown {
public:
    virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ComputationComplete(
        long lResult) = 0;

};

#else   /* C style interface */

typedef struct ICallBackVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *QueryInterface)(
        ICallBack __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *AddRef)(
        ICallBack __RPC_FAR * This);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *Release)(
        ICallBack __RPC_FAR * This);

    /* [helpstring] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *ComputationComplete)(
        ICallBack __RPC_FAR * This,
        long lResult);

    END_INTERFACE
} ICallBackVtbl;

interface ICallBack {
    CONST_VTBL struct ICallBackVtbl __RPC_FAR *lpVtbl;
};



#ifdef COBJMACROS


#define ICallBack_QueryInterface(This,riid,ppvObject)   \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICallBack_AddRef(This)  \
    (This)->lpVtbl -> AddRef(This)

#define ICallBack_Release(This) \
    (This)->lpVtbl -> Release(This)


#define ICallBack_ComputationComplete(This,lResult) \
    (This)->lpVtbl -> ComputationComplete(This,lResult)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE ICallBack_ComputationComplete_Proxy(
    ICallBack __RPC_FAR * This,
    long lResult);


void __RPC_STUB ICallBack_ComputationComplete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __ICallBack_INTERFACE_DEFINED__ */


#ifndef __IMath_INTERFACE_DEFINED__
#define __IMath_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IMath
 * at Tue Nov 04 17:55:33 1997
 * using MIDL 3.01.75
 ****************************************/
/* [object][unique][helpstring][uuid] */



EXTERN_C const IID IID_IMath;

#if defined(__cplusplus) && !defined(CINTERFACE)

interface DECLSPEC_UUID("DB5CCD8E-5489-11D1-883A-444553540000")
IMath :
public IUnknown {
public:
    virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Add(
        /* [in] */ long lOp1,
        /* [in] */ long lOp2) = 0;

    virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Subtract(
        /* [in] */ long lOp1,
        /* [in] */ long lOp2) = 0;

    virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Multiply(
        /* [in] */ long lOp1,
        /* [in] */ long lOp2) = 0;

    virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Divide(
        /* [in] */ long lOp1,
        /* [in] */ long lOp2) = 0;

    virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Advise(
        /* [in] */ ICallBack __RPC_FAR * pCallBack) = 0;

    virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE UnAdvise(void) = 0;

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

    /* [helpstring] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Add)(
        IMath __RPC_FAR * This,
        /* [in] */ long lOp1,
        /* [in] */ long lOp2);

    /* [helpstring] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Subtract)(
        IMath __RPC_FAR * This,
        /* [in] */ long lOp1,
        /* [in] */ long lOp2);

    /* [helpstring] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Multiply)(
        IMath __RPC_FAR * This,
        /* [in] */ long lOp1,
        /* [in] */ long lOp2);

    /* [helpstring] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Divide)(
        IMath __RPC_FAR * This,
        /* [in] */ long lOp1,
        /* [in] */ long lOp2);

    /* [helpstring] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Advise)(
        IMath __RPC_FAR * This,
        /* [in] */ ICallBack __RPC_FAR *pCallBack);

    /* [helpstring] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *UnAdvise)(
        IMath __RPC_FAR * This);

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


#define IMath_Add(This,lOp1,lOp2)   \
    (This)->lpVtbl -> Add(This,lOp1,lOp2)

#define IMath_Subtract(This,lOp1,lOp2)  \
    (This)->lpVtbl -> Subtract(This,lOp1,lOp2)

#define IMath_Multiply(This,lOp1,lOp2)  \
    (This)->lpVtbl -> Multiply(This,lOp1,lOp2)

#define IMath_Divide(This,lOp1,lOp2)    \
    (This)->lpVtbl -> Divide(This,lOp1,lOp2)

#define IMath_Advise(This,pCallBack)    \
    (This)->lpVtbl -> Advise(This,pCallBack)

#define IMath_UnAdvise(This)    \
    (This)->lpVtbl -> UnAdvise(This)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMath_Add_Proxy(
    IMath __RPC_FAR * This,
    /* [in] */ long lOp1,
    /* [in] */ long lOp2);


void __RPC_STUB IMath_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMath_Subtract_Proxy(
    IMath __RPC_FAR * This,
    /* [in] */ long lOp1,
    /* [in] */ long lOp2);


void __RPC_STUB IMath_Subtract_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMath_Multiply_Proxy(
    IMath __RPC_FAR * This,
    /* [in] */ long lOp1,
    /* [in] */ long lOp2);


void __RPC_STUB IMath_Multiply_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMath_Divide_Proxy(
    IMath __RPC_FAR * This,
    /* [in] */ long lOp1,
    /* [in] */ long lOp2);


void __RPC_STUB IMath_Divide_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMath_Advise_Proxy(
    IMath __RPC_FAR * This,
    /* [in] */ ICallBack __RPC_FAR *pCallBack);


void __RPC_STUB IMath_Advise_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMath_UnAdvise_Proxy(
    IMath __RPC_FAR * This);


void __RPC_STUB IMath_UnAdvise_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __IMath_INTERFACE_DEFINED__ */



#ifndef __CHAPTER7_CALLBACKSERVERLib_LIBRARY_DEFINED__
#define __CHAPTER7_CALLBACKSERVERLib_LIBRARY_DEFINED__

/****************************************
 * Generated header for library: CHAPTER7_CALLBACKSERVERLib
 * at Tue Nov 04 17:55:33 1997
 * using MIDL 3.01.75
 ****************************************/
/* [helpstring][version][uuid] */



EXTERN_C const IID LIBID_CHAPTER7_CALLBACKSERVERLib;

#ifdef __cplusplus
EXTERN_C const CLSID CLSID_Math;

class DECLSPEC_UUID("DB5CCD8F-5489-11D1-883A-444553540000")
    Math;
#endif
#endif /* __CHAPTER7_CALLBACKSERVERLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
