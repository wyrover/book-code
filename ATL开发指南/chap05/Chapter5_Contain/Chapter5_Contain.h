/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 3.01.75 */
/* at Thu Oct 09 21:36:54 1997
 */
/* Compiler settings for Chapter5_Contain.idl:
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

#ifndef __Chapter5_Contain_h__
#define __Chapter5_Contain_h__

#ifdef __cplusplus
extern "C" {
#endif

/* Forward Declarations */

#ifndef __IMath_FWD_DEFINED__
#define __IMath_FWD_DEFINED__
typedef interface IMath IMath;
#endif  /* __IMath_FWD_DEFINED__ */


#ifndef __IAdvancedMath_FWD_DEFINED__
#define __IAdvancedMath_FWD_DEFINED__
typedef interface IAdvancedMath IAdvancedMath;
#endif  /* __IAdvancedMath_FWD_DEFINED__ */


#ifndef __AdvancedMath_FWD_DEFINED__
#define __AdvancedMath_FWD_DEFINED__

#ifdef __cplusplus
typedef class AdvancedMath AdvancedMath;
#else
typedef struct AdvancedMath AdvancedMath;
#endif /* __cplusplus */

#endif  /* __AdvancedMath_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free(void __RPC_FAR *);

#ifndef __IMath_INTERFACE_DEFINED__
#define __IMath_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IMath
 * at Thu Oct 09 21:36:54 1997
 * using MIDL 3.01.75
 ****************************************/
/* [object][unique][helpstring][uuid] */



EXTERN_C const IID IID_IMath;

#if defined(__cplusplus) && !defined(CINTERFACE)

interface DECLSPEC_UUID("380F79EA-4098-11D1-883A-444553540000")
IMath :
public IUnknown {
public:
    virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Add(
        long lOp1,
        long lOp2,
        long __RPC_FAR * plResult) = 0;

    virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Subtract(
        long lOp1,
        long lOp2,
        long __RPC_FAR * plResult) = 0;

    virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Multiply(
        long lOp1,
        long lOp2,
        long __RPC_FAR * plResult) = 0;

    virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Divide(
        long lOp1,
        long lOp2,
        long __RPC_FAR * plResult) = 0;

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
        long lOp1,
        long lOp2,
        long __RPC_FAR *plResult);

    /* [helpstring] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Subtract)(
        IMath __RPC_FAR * This,
        long lOp1,
        long lOp2,
        long __RPC_FAR *plResult);

    /* [helpstring] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Multiply)(
        IMath __RPC_FAR * This,
        long lOp1,
        long lOp2,
        long __RPC_FAR *plResult);

    /* [helpstring] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Divide)(
        IMath __RPC_FAR * This,
        long lOp1,
        long lOp2,
        long __RPC_FAR *plResult);

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


#define IMath_Add(This,lOp1,lOp2,plResult)  \
    (This)->lpVtbl -> Add(This,lOp1,lOp2,plResult)

#define IMath_Subtract(This,lOp1,lOp2,plResult) \
    (This)->lpVtbl -> Subtract(This,lOp1,lOp2,plResult)

#define IMath_Multiply(This,lOp1,lOp2,plResult) \
    (This)->lpVtbl -> Multiply(This,lOp1,lOp2,plResult)

#define IMath_Divide(This,lOp1,lOp2,plResult)   \
    (This)->lpVtbl -> Divide(This,lOp1,lOp2,plResult)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMath_Add_Proxy(
    IMath __RPC_FAR * This,
    long lOp1,
    long lOp2,
    long __RPC_FAR *plResult);


void __RPC_STUB IMath_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMath_Subtract_Proxy(
    IMath __RPC_FAR * This,
    long lOp1,
    long lOp2,
    long __RPC_FAR *plResult);


void __RPC_STUB IMath_Subtract_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMath_Multiply_Proxy(
    IMath __RPC_FAR * This,
    long lOp1,
    long lOp2,
    long __RPC_FAR *plResult);


void __RPC_STUB IMath_Multiply_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMath_Divide_Proxy(
    IMath __RPC_FAR * This,
    long lOp1,
    long lOp2,
    long __RPC_FAR *plResult);


void __RPC_STUB IMath_Divide_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __IMath_INTERFACE_DEFINED__ */


#ifndef __IAdvancedMath_INTERFACE_DEFINED__
#define __IAdvancedMath_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IAdvancedMath
 * at Thu Oct 09 21:36:54 1997
 * using MIDL 3.01.75
 ****************************************/
/* [object][unique][helpstring][uuid] */



EXTERN_C const IID IID_IAdvancedMath;

#if defined(__cplusplus) && !defined(CINTERFACE)

interface DECLSPEC_UUID("380F79F9-4098-11D1-883A-444553540000")
IAdvancedMath :
public IUnknown {
public:
    virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Factorial(
        short sOp,
        long __RPC_FAR * plResult) = 0;

    virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Fibonacci(
        short sOp,
        long __RPC_FAR * plResult) = 0;

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

    /* [helpstring] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Factorial)(
        IAdvancedMath __RPC_FAR * This,
        short sOp,
        long __RPC_FAR *plResult);

    /* [helpstring] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Fibonacci)(
        IAdvancedMath __RPC_FAR * This,
        short sOp,
        long __RPC_FAR *plResult);

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


#define IAdvancedMath_Factorial(This,sOp,plResult)  \
    (This)->lpVtbl -> Factorial(This,sOp,plResult)

#define IAdvancedMath_Fibonacci(This,sOp,plResult)  \
    (This)->lpVtbl -> Fibonacci(This,sOp,plResult)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAdvancedMath_Factorial_Proxy(
    IAdvancedMath __RPC_FAR * This,
    short sOp,
    long __RPC_FAR *plResult);


void __RPC_STUB IAdvancedMath_Factorial_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAdvancedMath_Fibonacci_Proxy(
    IAdvancedMath __RPC_FAR * This,
    short sOp,
    long __RPC_FAR *plResult);


void __RPC_STUB IAdvancedMath_Fibonacci_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __IAdvancedMath_INTERFACE_DEFINED__ */



#ifndef __CHAPTER5_CONTAINLib_LIBRARY_DEFINED__
#define __CHAPTER5_CONTAINLib_LIBRARY_DEFINED__

/****************************************
 * Generated header for library: CHAPTER5_CONTAINLib
 * at Thu Oct 09 21:36:54 1997
 * using MIDL 3.01.75
 ****************************************/
/* [helpstring][version][uuid] */



EXTERN_C const IID LIBID_CHAPTER5_CONTAINLib;

#ifdef __cplusplus
EXTERN_C const CLSID CLSID_AdvancedMath;

class DECLSPEC_UUID("380F7A0B-4098-11D1-883A-444553540000")
    AdvancedMath;
#endif
#endif /* __CHAPTER5_CONTAINLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
