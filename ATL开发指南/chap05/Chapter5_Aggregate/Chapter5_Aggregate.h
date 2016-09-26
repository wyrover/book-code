/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Fri Dec 11 10:51:37 1998
 */
/* Compiler settings for Chapter5_Aggregate.idl:
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

#ifndef __Chapter5_Aggregate_h__
#define __Chapter5_Aggregate_h__

#ifdef __cplusplus
extern "C" {
#endif

/* Forward Declarations */

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

#ifndef __IAdvancedMath_INTERFACE_DEFINED__
#define __IAdvancedMath_INTERFACE_DEFINED__

/* interface IAdvancedMath */
/* [object][unique][helpstring][uuid] */


EXTERN_C const IID IID_IAdvancedMath;

#if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("380F79F9-4098-11D1-883A-444553540000")
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



#ifndef __CHAPTER5_AGGREGATELib_LIBRARY_DEFINED__
#define __CHAPTER5_AGGREGATELib_LIBRARY_DEFINED__

/* library CHAPTER5_AGGREGATELib */
/* [helpstring][version][uuid] */


EXTERN_C const IID LIBID_CHAPTER5_AGGREGATELib;

EXTERN_C const CLSID CLSID_AdvancedMath;

#ifdef __cplusplus

class DECLSPEC_UUID("380F79FA-4098-11D1-883A-444553540000")
    AdvancedMath;
#endif
#endif /* __CHAPTER5_AGGREGATELib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
