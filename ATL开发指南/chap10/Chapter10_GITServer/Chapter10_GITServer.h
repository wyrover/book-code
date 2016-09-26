/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 3.01.75 */
/* at Fri Mar 20 12:55:54 1998
 */
/* Compiler settings for Chapter10_GITServer.idl:
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

#ifndef __Chapter10_GITServer_h__
#define __Chapter10_GITServer_h__

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
 * at Fri Mar 20 12:55:54 1998
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][uuid][object] */



EXTERN_C const IID IID_ICallBack;

#if defined(__cplusplus) && !defined(CINTERFACE)

interface DECLSPEC_UUID("9029D3B0-67FE-11d1-B5F9-0004ACFF171C")
ICallBack :
public IUnknown {
public:
    virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ComputationComplete(
        /* [in] */ long lResult) = 0;

    virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Error(
        /* [in] */ BSTR bstrDescription) = 0;

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
        /* [in] */ long lResult);

    /* [helpstring] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Error)(
        ICallBack __RPC_FAR * This,
        /* [in] */ BSTR bstrDescription);

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

#define ICallBack_Error(This,bstrDescription)   \
    (This)->lpVtbl -> Error(This,bstrDescription)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE ICallBack_ComputationComplete_Proxy(
    ICallBack __RPC_FAR * This,
    /* [in] */ long lResult);


void __RPC_STUB ICallBack_ComputationComplete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ICallBack_Error_Proxy(
    ICallBack __RPC_FAR * This,
    /* [in] */ BSTR bstrDescription);


void __RPC_STUB ICallBack_Error_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __ICallBack_INTERFACE_DEFINED__ */


#ifndef __IMath_INTERFACE_DEFINED__
#define __IMath_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IMath
 * at Fri Mar 20 12:55:54 1998
 * using MIDL 3.01.75
 ****************************************/
/* [object][unique][helpstring][uuid] */



EXTERN_C const IID IID_IMath;

#if defined(__cplusplus) && !defined(CINTERFACE)

interface DECLSPEC_UUID("C2C66744-C012-11D1-B619-0004ACFF171C")
IMath :
public IUnknown {
public:
    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Add(
        /* [in] */ long lOp1,
        /* [in] */ long lOp2) = 0;

    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Subtract(
        /* [in] */ long lOp1,
        /* [in] */ long lOp2) = 0;

    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Multiply(
        /* [in] */ long lOp1,
        /* [in] */ long lOp2) = 0;

    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Divide(
        /* [in] */ long lOp1,
        /* [in] */ long lOp2) = 0;

    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Advise(
        /* [in] */ ICallBack __RPC_FAR * pCallBack) = 0;

    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Unadvise(void) = 0;

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

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Add)(
        IMath __RPC_FAR * This,
        /* [in] */ long lOp1,
        /* [in] */ long lOp2);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Subtract)(
        IMath __RPC_FAR * This,
        /* [in] */ long lOp1,
        /* [in] */ long lOp2);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Multiply)(
        IMath __RPC_FAR * This,
        /* [in] */ long lOp1,
        /* [in] */ long lOp2);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Divide)(
        IMath __RPC_FAR * This,
        /* [in] */ long lOp1,
        /* [in] */ long lOp2);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Advise)(
        IMath __RPC_FAR * This,
        /* [in] */ ICallBack __RPC_FAR *pCallBack);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Unadvise)(
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

#define IMath_Unadvise(This)    \
    (This)->lpVtbl -> Unadvise(This)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMath_Add_Proxy(
    IMath __RPC_FAR * This,
    /* [in] */ long lOp1,
    /* [in] */ long lOp2);


void __RPC_STUB IMath_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMath_Subtract_Proxy(
    IMath __RPC_FAR * This,
    /* [in] */ long lOp1,
    /* [in] */ long lOp2);


void __RPC_STUB IMath_Subtract_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMath_Multiply_Proxy(
    IMath __RPC_FAR * This,
    /* [in] */ long lOp1,
    /* [in] */ long lOp2);


void __RPC_STUB IMath_Multiply_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMath_Divide_Proxy(
    IMath __RPC_FAR * This,
    /* [in] */ long lOp1,
    /* [in] */ long lOp2);


void __RPC_STUB IMath_Divide_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMath_Advise_Proxy(
    IMath __RPC_FAR * This,
    /* [in] */ ICallBack __RPC_FAR *pCallBack);


void __RPC_STUB IMath_Advise_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMath_Unadvise_Proxy(
    IMath __RPC_FAR * This);


void __RPC_STUB IMath_Unadvise_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __IMath_INTERFACE_DEFINED__ */



#ifndef __CHAPTER10_GITSERVERLib_LIBRARY_DEFINED__
#define __CHAPTER10_GITSERVERLib_LIBRARY_DEFINED__

/****************************************
 * Generated header for library: CHAPTER10_GITSERVERLib
 * at Fri Mar 20 12:55:54 1998
 * using MIDL 3.01.75
 ****************************************/
/* [helpstring][version][uuid] */



EXTERN_C const IID LIBID_CHAPTER10_GITSERVERLib;

#ifdef __cplusplus
EXTERN_C const CLSID CLSID_Math;

class DECLSPEC_UUID("C2C66745-C012-11D1-B619-0004ACFF171C")
    Math;
#endif
#endif /* __CHAPTER10_GITSERVERLib_LIBRARY_DEFINED__ */

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
